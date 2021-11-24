#define MAZ_LOG_VERBOSE
#include "CTransform2DComponent.h"


namespace maz
{

CTransform2DComponent::CTransform2DComponent(CReference<CEntity>& arOwner)
    : CComponentBase(arOwner)
    , mWorldTransformDirty(true)
    , mModelMatrix(1.0f)
{
    MAZ_LOGGER_VERBOSE("Called");
}


CTransform2DComponent::~CTransform2DComponent()
{
    MAZ_LOGGER_VERBOSE("Called");
}


const TVec2& CTransform2DComponent::GetTranslation() const
{
    return mLocalTransform.GetTranslation();
}


void CTransform2DComponent::SetTranslation(const TVec2& arTranslation)
{
    if (mLocalTransform.GetTranslation() != arTranslation)
    {
        mLocalTransform.SetTranslation(arTranslation);
        mWorldTransformDirty = true;
    }
}


float CTransform2DComponent::GetRotation() const
{
    return mLocalTransform.GetRotation();
}


void CTransform2DComponent::SetRotation(float aRotation)
{
    if (mLocalTransform.GetRotation() != aRotation)
    {
        mLocalTransform.SetRotation(aRotation);
        mWorldTransformDirty = true;
    }
}


const TVec2& CTransform2DComponent::GetScale() const
{
    return mLocalTransform.GetScale();
}


void CTransform2DComponent::SetScale(const TVec2& arScale)
{
    if (mLocalTransform.GetScale() != arScale)
    {
        mLocalTransform.SetScale(arScale);
        mWorldTransformDirty = true;
    }
}


void CTransform2DComponent::SetParentTransform(CReference<CTransform2DComponent>& arParentTransform)
{
    // Setting the current parent as parent is an acceptable request, which results in the transform being moved to the end of the parent's hierarchy

    // Clear current hierarchy information
    if (mParent)
    {
        mParent->removeChildTransform(GetThis());
    }

    // Update new hierarchy info
    if (arParentTransform)
    {
        arParentTransform->addChildTransform(GetThis());
    }
}


const CReference<CTransform2DComponent>& CTransform2DComponent::GetNextSiblingTransform() const
{
    return mNextSibling;
}


const CReference<CTransform2DComponent>& CTransform2DComponent::GetFirstChildTransform() const
{
    return mFirstChild;
}


const TMat4x4& CTransform2DComponent::GetModelMatrix() const
{
    if (mWorldTransformDirty)
    {
        //const_cast<CTransform2DComponent*>(this)->RebuildModelMatrix();
    }
    return mModelMatrix;
}


const CReference<CTransform2DComponent>& CTransform2DComponent::GetParentTransform() const
{
    return mParent;
}


const STransform2D& CTransform2DComponent::GetLocalTransform() const
{
    return mLocalTransform;
}


const STransform2D& CTransform2DComponent::GetWorldTransform() const
{
    return mWorldTransform;
}


bool CTransform2DComponent::IsWorldTransformDirty() const
{
    return mWorldTransformDirty;
}


bool CTransform2DComponent::addChildTransform(CReference<CTransform2DComponent>& arChildTransform)
{
    if (mNumChildren == 0)
    {
        mFirstChild = arChildTransform;
    }
    else
    {
        CReference<CTransform2DComponent> lLastChild = mFirstChild;

        for (uint16 lChildIdx = 0; lChildIdx < mNumChildren - 1; ++lChildIdx)
        {
            lLastChild = lLastChild->mNextSibling;
        }

        MAZ_ASSERT(!lLastChild->mNextSibling, "The last child already has a nextSibling!");
        lLastChild->mNextSibling = arChildTransform;
    }

    arChildTransform->mParent = GetThis();
    ++mNumChildren;

    return true;
}


bool CTransform2DComponent::removeChildTransform(CReference<CTransform2DComponent>& arChildTransform)
{
    MAZ_ASSERT(mFirstChild, "Attempting to remove a child from a CTransform2DComponent which has no children!");
    CReference<CTransform2DComponent> lPreviousChild;
    CReference<CTransform2DComponent> lCurrentChild = mFirstChild;

    bool lFound = false;
    for (uint16 childIdx = 0; (childIdx < mNumChildren) && !lFound; ++childIdx)
    {
        lFound = (lCurrentChild == arChildTransform);
        if (lFound)
        {
            // Reconnect siblings chain
            if (lPreviousChild)
            {
                lPreviousChild->mNextSibling = lCurrentChild->mNextSibling;
            }
            else
            {
                MAZ_ASSERT(lCurrentChild == mFirstChild, "Child transform to remove is not the mFirstChild, but there is not previousChild!");
                mFirstChild = arChildTransform->mNextSibling;
            }
            
            // Update child counter
            --mNumChildren;

            // Clean up removed child
            arChildTransform->mParent = CReference<CTransform2DComponent>();
            arChildTransform->mNextSibling = CReference<CTransform2DComponent>();
            arChildTransform->mWorldTransformDirty = true;
        }

        // Move forward in children hierarchy
        lPreviousChild = lCurrentChild;
        lCurrentChild = lCurrentChild->mNextSibling;
    }

    return lFound;
}


void CTransform2DComponent::SetWorldTransform(const STransform2D& arTransform)
{
    mWorldTransform = arTransform;
}


void CTransform2DComponent::RebuildModelMatrix()
{
    mModelMatrix = TMat4x4(1.0f);
    mModelMatrix = glm::translate(mModelMatrix, TVec3(mWorldTransform.GetTranslation(), 0.0f));
    mModelMatrix = mModelMatrix * glm::toMat4(TQuat(TVec3(0.0f, 0.0f, glm::radians(mWorldTransform.GetRotation()))));
    mModelMatrix = glm::scale(mModelMatrix, TVec3(mWorldTransform.GetScale(), 1.0f));

    mWorldTransformDirty = false;
}

} // maz

#define MAZ_LOG_VERBOSE
#include "CTransform2DComponent.h"


namespace maz
{

CTransform2DComponent::CTransform2DComponent(CReference<CGameObject>& aOwner)
    : CComponentBase(aOwner)
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


void CTransform2DComponent::SetTranslation(const TVec2& aTranslation)
{
    if (mLocalTransform.GetTranslation() != aTranslation)
    {
        mLocalTransform.SetTranslation(aTranslation);
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


void CTransform2DComponent::SetScale(const TVec2& aScale)
{
    if (mLocalTransform.GetScale() != aScale)
    {
        mLocalTransform.SetScale(aScale);
        mWorldTransformDirty = true;
    }
}


void CTransform2DComponent::SetParentTransform(CReference<CTransform2DComponent>& aParentTransform)
{
    // Setting the current parent as parent is an acceptable request, which results in the transform being moved to the end of the parent's hierarchy

    // Clear current hierarchy information
    if (mParent)
    {
        mParent->removeChildTransform(GetThis());
    }

    // Update new hierarchy info
    if (aParentTransform)
    {
        aParentTransform->addChildTransform(GetThis());
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


bool CTransform2DComponent::addChildTransform(CReference<CTransform2DComponent>& aChildTransform)
{
    if (mNumChildren == 0)
    {
        mFirstChild = aChildTransform;
    }
    else
    {
        CReference<CTransform2DComponent> lastChild = mFirstChild;

        for (uint16 childIdx = 0; childIdx < mNumChildren - 1; ++childIdx)
        {
            lastChild = lastChild->mNextSibling;
        }

        MAZ_ASSERT(!lastChild->mNextSibling, "The last child already has a nextSibling!");
        lastChild->mNextSibling = aChildTransform;
    }

    aChildTransform->mParent = GetThis();
    ++mNumChildren;

    return true;
}


bool CTransform2DComponent::removeChildTransform(CReference<CTransform2DComponent>& aChildTransform)
{
    MAZ_ASSERT(mFirstChild, "Attempting to remove a child from a CTransform2DComponent which has no children!");
    CReference<CTransform2DComponent> previousChild;
    CReference<CTransform2DComponent> currentChild = mFirstChild;

    bool found = false;
    for (uint16 childIdx = 0; (childIdx < mNumChildren) && !found; ++childIdx)
    {
        found = (currentChild == aChildTransform);
        if (found)
        {
            // Reconnect siblings chain
            if (previousChild)
            {
                previousChild->mNextSibling = currentChild->mNextSibling;
            }
            else
            {
                MAZ_ASSERT(currentChild == mFirstChild, "Child transform to remove is not the mFirstChild, but there is not previousChild!");
                mFirstChild = aChildTransform->mNextSibling;
            }
            
            // Update child counter
            --mNumChildren;

            // Clean up removed child
            aChildTransform->mParent = CReference<CTransform2DComponent>();
            aChildTransform->mNextSibling = CReference<CTransform2DComponent>();
            aChildTransform->mWorldTransformDirty = true;
        }

        // Move forward in children hierarchy
        previousChild = currentChild;
        currentChild = currentChild->mNextSibling;
    }

    return found;
}


void CTransform2DComponent::SetWorldTransform(const STransform2D& aTransform)
{
    mWorldTransform = aTransform;
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

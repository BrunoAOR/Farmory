#define MAZ_LOG_VERBOSE
#include "CTransform2DComponent.h"


namespace maz
{

CTransform2DComponent::CTransform2DComponent(CReference<CGameObject>& aOwner)
    : CComponentBase(aOwner)
    , mLocalTransformDirty(true)
    , mModelMatrix(1.0f)
{
    MAZ_LOGGER_VERBOSE("Called");
    RebuildModelMatrix();
}


CTransform2DComponent::~CTransform2DComponent()
{
    MAZ_LOGGER_VERBOSE("Called");
}


const TVec2& CTransform2DComponent::GetTranslation() const
{
    return mLocalTransform.mTranslation;
}


void CTransform2DComponent::SetTranslation(const TVec2& aTranslation)
{
    mLocalTransform.mTranslation = aTranslation;
    mLocalTransformDirty = true;
}


float CTransform2DComponent::GetRotation() const
{
    return mLocalTransform.mRotation;
}


void CTransform2DComponent::SetRotation(float aRotation)
{
    mLocalTransform.mRotation = aRotation;
    while (mLocalTransform.mRotation >= 360.0f)
    {
        mLocalTransform.mRotation -= 360.0f;
    }
    while (mLocalTransform.mRotation < 0.0f)
    {
        mLocalTransform.mRotation += 360.0f;
    }
    mLocalTransformDirty = true;
}


const TVec2& CTransform2DComponent::GetScale() const
{
    return mLocalTransform.mScale;
}


void CTransform2DComponent::SetScale(const TVec2& aScale)
{
    mLocalTransform.mScale = aScale;
    mLocalTransformDirty = true;
}

void CTransform2DComponent::SetParentTransform(CReference<CTransform2DComponent>& aParentTransform)
{
    // Setting the current parent as parent is an acceptable request, which results in the transform being moved to the end of the parent's hierarchy

    // Clear current hierarchy information
    if (mParent)
    {
        mParent->removeChildTransform(GetThis());
    }

    // Update new hiararchy info
    if (aParentTransform)
    {
        aParentTransform->addChildTransform(GetThis());
    }
}


const TMat4x4& CTransform2DComponent::GetModelMatrix() const
{
    if (mLocalTransformDirty)
    {
        const_cast<CTransform2DComponent*>(this)->RebuildModelMatrix();
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


STransform2D& CTransform2DComponent::GetWorldTransform()
{
    return mWorldTransform;
}

bool CTransform2DComponent::IsLocalTransformDirty() const
{
    return mLocalTransformDirty;
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
            aChildTransform->mLocalTransformDirty = true;
        }

        // Move forward in children hierarchy
        previousChild = currentChild;
        currentChild = currentChild->mNextSibling;
    }

    return found;
}


void CTransform2DComponent::RebuildModelMatrix()
{
    mModelMatrix = TMat4x4(1.0f);
    mModelMatrix = glm::translate(mModelMatrix, TVec3(mLocalTransform.mTranslation, 0.0f));
    mModelMatrix = mModelMatrix * glm::toMat4(TQuat(TVec3(0.0f, 0.0f, glm::radians(mLocalTransform.mRotation))));
    mModelMatrix = glm::scale(mModelMatrix, TVec3(mLocalTransform.mScale, 1.0f));

    mLocalTransformDirty = false;
}

} // maz

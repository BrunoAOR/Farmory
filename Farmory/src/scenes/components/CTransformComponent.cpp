#define MAZ_LOG_VERBOSE
#include "CTransformComponent.h"


namespace maz
{

CTransformComponent::CTransformComponent(CReference<CGameObject>& aOwner)
    : IComponent(aOwner)
    , mTranslation(0.0f, 0.0f)
    , mRotation(0.0f)
    , mScale(1.0f, 1.0f)
    , mModelMatrix(1.0f)
{
    MAZ_LOGGER_VERBOSE("CTransformComponent::CTransformComponent called");
    rebuildModelMatrix();
}


CTransformComponent::~CTransformComponent()
{
    MAZ_LOGGER_VERBOSE("CTransformComponent::~CTransformComponent called");
}


const TVec2& CTransformComponent::GetTranslation() const
{
    return mTranslation;
}


void CTransformComponent::SetTranslation(const TVec2& aTranslation)
{
    mTranslation = aTranslation;
    mModelMatrixDirty = true;
}


float CTransformComponent::GetRotation() const
{
    return mRotation;
}


void CTransformComponent::SetRotation(float aRotation)
{
    mRotation = aRotation;
    while (mRotation >= 360.0f)
    {
        mRotation -= 360.0f;
    }
    while (mRotation < 0.0f)
    {
        mRotation += 360.0f;
    }
    mModelMatrixDirty = true;
}


const TVec2& CTransformComponent::GetScale() const
{
    return mScale;
}


void CTransformComponent::SetScale(const TVec2& aScale)
{
    mScale = aScale;
    mModelMatrixDirty = true;
}


const TMat4x4& CTransformComponent::GetModelMatrix() const
{
    if (mModelMatrixDirty)
    {
        const_cast<CTransformComponent*>(this)->mModelMatrixDirty = false;
        const_cast<CTransformComponent*>(this)->rebuildModelMatrix();
    }
    return mModelMatrix;
}


void CTransformComponent::rebuildModelMatrix()
{
    mModelMatrix = TMat4x4(1.0f);
    mModelMatrix = glm::translate(mModelMatrix, TVec3(mTranslation, 0.0f));
    mModelMatrix = mModelMatrix * glm::toMat4(TQuat(TVec3(0.0f, 0.0f, glm::radians(mRotation))));
    mModelMatrix = glm::scale(mModelMatrix, TVec3(mScale, 1.0f));
}

} // maz

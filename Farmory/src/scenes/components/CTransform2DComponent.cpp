#define MAZ_LOG_VERBOSE
#include "CTransform2DComponent.h"


namespace maz
{

CTransform2DComponent::CTransform2DComponent(CReference<CGameObject>& aOwner)
    : IComponent(aOwner)
    , mTranslation(0.0f, 0.0f)
    , mRotation(0.0f)
    , mScale(1.0f, 1.0f)
    , mModelMatrix(1.0f)
{
    MAZ_LOGGER_VERBOSE("CTransform2DComponent::CTransform2DComponent called");
    rebuildModelMatrix();
}


CTransform2DComponent::~CTransform2DComponent()
{
    MAZ_LOGGER_VERBOSE("CTransform2DComponent::~CTransform2DComponent called");
}


const TVec2& CTransform2DComponent::GetTranslation() const
{
    return mTranslation;
}


void CTransform2DComponent::SetTranslation(const TVec2& aTranslation)
{
    mTranslation = aTranslation;
    mModelMatrixDirty = true;
}


float CTransform2DComponent::GetRotation() const
{
    return mRotation;
}


void CTransform2DComponent::SetRotation(float aRotation)
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


const TVec2& CTransform2DComponent::GetScale() const
{
    return mScale;
}


void CTransform2DComponent::SetScale(const TVec2& aScale)
{
    mScale = aScale;
    mModelMatrixDirty = true;
}


const TMat4x4& CTransform2DComponent::GetModelMatrix() const
{
    if (mModelMatrixDirty)
    {
        const_cast<CTransform2DComponent*>(this)->mModelMatrixDirty = false;
        const_cast<CTransform2DComponent*>(this)->rebuildModelMatrix();
    }
    return mModelMatrix;
}


void CTransform2DComponent::rebuildModelMatrix()
{
    mModelMatrix = TMat4x4(1.0f);
    mModelMatrix = glm::translate(mModelMatrix, TVec3(mTranslation, 0.0f));
    mModelMatrix = mModelMatrix * glm::toMat4(TQuat(TVec3(0.0f, 0.0f, glm::radians(mRotation))));
    mModelMatrix = glm::scale(mModelMatrix, TVec3(mScale, 1.0f));
}

} // maz

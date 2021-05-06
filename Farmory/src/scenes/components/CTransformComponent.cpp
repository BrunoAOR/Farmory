#define MAZ_LOG_VERBOSE
#include "CTransformComponent.h"


namespace maz
{

CTransformComponent::CTransformComponent(CReference<CGameObject>& aOwner)
    : IComponent(aOwner)
    , mTranslation(0.0f, 0.0f, 0.0f)
    , mRotation(glm::identity<glm::quat>())
    , mScale(1.0f, 1.0f, 1.0f)
    , mModelMatrix(1.0f)
{
    MAZ_LOGGER_VERBOSE("CTransformComponent::CTransformComponent called");
    RebuildModelMatrix();
}


CTransformComponent::~CTransformComponent()
{
    MAZ_LOGGER_VERBOSE("CTransformComponent::~CTransformComponent called");
}


void CTransformComponent::RebuildModelMatrix()
{
    mModelMatrix = glm::mat4(1.0f);
    mModelMatrix = glm::translate(mModelMatrix, mTranslation);
    mModelMatrix = mModelMatrix * glm::toMat4(mRotation);
    mModelMatrix = glm::scale(mModelMatrix, mScale);
}

} // maz

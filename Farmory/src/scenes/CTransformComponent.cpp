#define MAZ_LOG_VERBOSE
#include "CTransformComponent.h"


namespace maz
{

CTransformComponent::CTransformComponent(CReference<CGameObject>& aOwner)
    : IComponent(aOwner)
{
    MAZ_LOGGER_VERBOSE("CTransformComponent::CTransformComponent called");
    a = 1.0f;
    b = 2.0f;
    c = 3.0f;
}


CTransformComponent::~CTransformComponent()
{
    MAZ_LOGGER_VERBOSE("CTransformComponent::~CTransformComponent called");
}

} // maz

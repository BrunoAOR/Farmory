#include "CTransformComponent.h"

namespace maz
{

CTransformComponent::CTransformComponent(CGameObject& aOwner)
    : IComponent(aOwner)
{
    a = 1.0f;
    b = 2.0f;
    c = 3.0f;
}

} // maz

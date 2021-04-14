#include "IComponent.h"


namespace maz
{

IComponent::IComponent(CGameObject& aOwner)
    : mOwner(aOwner)
{}


const CGameObject& IComponent::GetOwner() const
{
    return mOwner;
}


CGameObject& IComponent::GetOwner()
{
    return mOwner;
}

} // maz

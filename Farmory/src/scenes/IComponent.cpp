#include "IComponent.h"


namespace maz
{

IComponent::IComponent(CReference<CGameObject>& aOwner)
    : mOwner(aOwner)
{}


const CReference<CGameObject> IComponent::GetOwner() const
{
    return mOwner;
}


CReference<CGameObject> IComponent::GetOwner()
{
    return mOwner;
}

} // maz

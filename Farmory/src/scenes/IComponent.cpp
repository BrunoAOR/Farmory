#define MAZ_LOG_VERBOSE
#include "IComponent.h"


namespace maz
{

IComponent::IComponent(CReference<CGameObject>& aOwner)
    : mOwner(aOwner)
{
    MAZ_LOGGER_VERBOSE("IComponent::IComponent called");
}


IComponent::~IComponent()
{
    MAZ_LOGGER_VERBOSE("IComponent::~IComponent called");
}


const CReference<CGameObject> IComponent::GetOwner() const
{
    return mOwner;
}


CReference<CGameObject> IComponent::GetOwner()
{
    return mOwner;
}

} // maz

#define MAZ_LOG_VERBOSE
#include "IComponent.h"


namespace maz
{

IComponent::IComponent(CReference<CEntity>& aOwner)
    : mOwner(aOwner)
{
    MAZ_LOGGER_VERBOSE("Called");
}


IComponent::~IComponent()
{
    MAZ_LOGGER_VERBOSE("Called");
}


const CReference<CEntity> IComponent::GetOwner() const
{
    return mOwner;
}


CReference<CEntity> IComponent::GetOwner()
{
    return mOwner;
}

} // maz

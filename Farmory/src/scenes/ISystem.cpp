#include "ISystem.h"


namespace maz
{

ISystem::ISystem()
{}


void ISystem::AddComponentToSignature(EComponentType aComponentType)
{
    mSignature.AddComponent(aComponentType);
}


void ISystem::RemoveComponentFromSignature(EComponentType aComponentType)
{
    mSignature.RemoveComponent(aComponentType);
}

} // maz

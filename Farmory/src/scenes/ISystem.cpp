#include "ISystem.h"


namespace maz
{

ISystem::ISystem()
{}


ISystem::ISystem(SComponentsSignature&& arrSignature)
    : mSignature(arrSignature)
{}

} // maz

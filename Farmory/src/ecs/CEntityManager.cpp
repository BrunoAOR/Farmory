#include <maz/globals.h>
#include "CEntityManager.h"


namespace maz
{
namespace ecs
{

CEntityManager::CEntityManager()
    : mHighestEntityId(0)
{ ; }


CEntityManager::~CEntityManager() { ; }


TEntityId CEntityManager::CreateEntity()
{
    return mHighestEntityId++;
}

bool CEntityManager::DestroyEntity(TEntityId aEntityId)
{
    MAZ_UNUSED_VAR(aEntityId);
    return true;
}

} // ecs
} // maz

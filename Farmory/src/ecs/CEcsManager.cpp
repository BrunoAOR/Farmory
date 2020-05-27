#include <maz/globals.h>
#include "CEcsManager.h"
#include <ecs/CComponentManager.h>
#include <ecs/CSystem.h>


namespace maz
{
namespace ecs
{

CEcsManager::CEcsManager() { ; }
CEcsManager::~CEcsManager() { ; }


bool CEcsManager::Init()
{
    bool lOk = true;

    // Component Managers
    for (size_t i = 0, iMax = mComponentManagers.size(); (i < iMax) && lOk; ++i)
    {
        lOk = lOk && mComponentManagers[i]->Init();
    }

    // Systems
    for (size_t i = 0, iMax = mSystems.size(); (i < iMax) && lOk; ++i)
    {
        lOk = lOk && mSystems[i]->Init();
    }

    MAZ_ASSERT(lOk, "[CEcsManager]::Init");
    return lOk;
}


void CEcsManager::End()
{
    // Component Managers
    for (size_t i = 0, iMax = mComponentManagers.size(); i < iMax; ++i)
    {
        mComponentManagers[i]->End();
        MAZ_DELETE(mComponentManagers[i]);
    }
    mComponentManagers.clear();
    
    // Systems
    for (size_t i = 0, iMax = mSystems.size(); i < iMax; ++i)
    {
        mSystems[i]->End();
        MAZ_DELETE(mSystems[i]);
    }
    mSystems.clear();
}


void CEcsManager::Update()
{
    for (size_t i = 0, l = mSystems.size(); i < l; ++i)
    {
        mSystems[i]->Update();
    }
}


TEntityId CEcsManager::CreateEntity()
{
    const TEntityId lId = mEntityManager.CreateEntity();
    MAZ_ASSERT(lId != kInvalidId, "[CEcsManager]::CreateEntity - Creation failed and an invalid Id will be returned.")
    return lId;
}


bool CEcsManager::DestroyEntity(TEntityId aEntityId)
{
    MAZ_ASSERT(aEntityId != kInvalidId, "[CEcsManager]::DestroyEntity - An invalid TEntityId was passed as an argument. Operation will not proceed!");
    bool lOk = true;
    lOk = lOk && (kInvalidId != aEntityId);
    
    if (lOk)
    {
        DO STUFF ABOUT ENTITY DESTRUCTION;
    }
    
    return lOk;
}

} // ecs
} // maz

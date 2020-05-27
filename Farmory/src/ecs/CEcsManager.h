#ifndef _H_C_ECS_MANAGER_
#define _H_C_ECS_MANAGER_

#include <ecs/CEntityManager.h>
#include <ecs/CComponentManager.h>
#include <vector>


namespace maz
{
namespace ecs
{

class CSystemBase;
class CComponentManagerBase;

class CEcsManager
{
public:
    CEcsManager();
    ~CEcsManager();

    // Pre Initialization
    template< typename CompType >
    bool RegisterComponent();
    template< typename SystType >
    bool RegisterSystem();

    // Lifecycle
    bool Init();
    void End();
    void Update();

    // Entity creation/destruction
    TEntityId CreateEntity();
    bool DestroyEntity(TEntityId aEntityId);
    
    // Components addition/removal
    template< typename CompType >
    CompType* AddComponent(TEntityId aEntityId);
    template< typename CompType >
    bool RemoveComponent(TEntityId aEntityId);

private:
    void ProcessComponentsRemoval();
    void ProcessComponentAddition();

private:
    CEntityManager mEntityManager;
    std::vector< CComponentManagerBase* > mComponentManagers;
    std::vector< CSystemBase* > mSystems;
};

} // ecs
} // maz


namespace maz
{
namespace ecs
{

template< typename CompType >
inline bool CEcsManager::RegisterComponent()
{
    bool lOk = true;
    uint32 lComponentId = CompType::GetComponentId();
    lOk = lOk && ( (lComponentId >= mComponentManagers.size()) || (mComponentManagers[lComponentId] == nullptr) );
    MAZ_ASSERT(((lComponentId >= mComponentManagers.size()) || (mComponentManagers[lComponentId] == nullptr)), "[CEcsManager]::RegisterComponent - Attempting to register a component which has already been registered. Operation will not proceed!");
    if (lOk)
    {
        while (!(lComponentId < mComponentManagers.size()))
        {
            mComponentManagers.push_back(nullptr);
        }

        CComponentManager< CompType >* lComponentManager = MAZ_NEW((CComponentManager< CompType >));
        lOk = lOk && (lComponentManager != nullptr);
        if (lOk)
        {
            mComponentManagers[lComponentId] = lComponentManager;
        }
    }

    return lOk;
}


template< typename SystType >
inline bool CEcsManager::RegisterSystem()
{
    bool lOk = true;
    uint32 lSystemId = SystType::GetSystemId();
    lOk = lOk && ((lSystemId >= mSystems.size()) || (mSystems[lSystemId] == nullptr));
    MAZ_ASSERT(((lSystemId >= mSystems.size()) || (mSystems[lSystemId] == nullptr)), "[CEcsManager]::RegisterSystem - Attempting to register a system which has already been registered. Operation will not proceed!");
    if (lOk)
    {
        while (!(lSystemId < mSystems.size()))
        {
            mSystems.push_back(nullptr);
        }

        SystType* lSystem = MAZ_NEW(SystType);
        lOk = lOk && (lSystem != nullptr);
        mSystems[lSystemId] = lSystem;
    }

    MAZ_ASSERT(lOk, "[ CEcsManager]::RegisterSystem - Failed!");
    return lOk;
}


template<typename CompType>
inline CompType* CEcsManager::AddComponent(TEntityId aEntityId)
{
    MAZ_ASSERT(aEntityId != kInvalidId, "[CEcsManager]::AddComponent - An invalid TEntityId was passed as an argument. Operation will not proceed!");
    CompType* lComponent = nullptr;
    uint32 lComponentId = CompType::GetComponentId();
    
    if ((aEntityId != kInvalidId) && (lComponentId < mComponentManagers.size()))
    {
        lComponent = static_cast< CComponentManager< CompType > >(mComponentManagers[lComponent])->AddComponent(aEntityId);
    }

    MAZ_ASSERT(lComponent != nullptr, "[ CEcsManager]::AddComponent - Failed!");
    return lComponent;
}


template<typename CompType>
inline bool CEcsManager::RemoveComponent(TEntityId aEntityId)
{
    MAZ_ASSERT(aEntityId != kInvalidId, "[CEcsManager]::RemoveComponent - An invalid TEntityId was passed as an argument. Operation will not proceed!");
    bool lOk = true;
    uint32 lComponentId = CompType::GetComponentId();
    lOk = lOk && (aEntityId != kInvalidId);
    lOk = lOk && (lComponentId < mComponentManagers.size());

    lOk = lOk && static_cast<CComponentManager< CompType >>(mComponentManagers[lComponentId])->RemoveComponent(aEntityId);

    MAZ_ASSERT(lOk, "[ CEcsManager]::AddComponent - RemoveComponent!");
    return lOk;
}

} // ecs
} // maz

#endif // !_H_C_ECS_MANAGER_

#ifndef _H_C_COMPONENT_MANAGER_
#define _H_C_COMPONENT_MANAGER_

#include <ecs/CComponentManagerBase.h>
#include <ecs/TEntityId.h>
#include <array>
#include <map>


namespace maz
{
namespace ecs
{

const uint32 kMaxComponents = 512;

template< typename CompType >
class CComponentManager
    : public CComponentManagerBase
{
public:
    CComponentManager();

    bool Init();
    void End();

    CompType* GetComponent(TEntityId aEntityId);
    CompType* AddComponent(TEntityId aEntityId);
    bool RemoveComponent(TEntityId aEntityId);

private:
    using TComponentIndex = uint32;
    using TComponentsMap = std::map< TEntityId, TComponentIndex >;
    using TComponentArray = std::array< CompType, kMaxComponents >;

private:
    TEntityId GetEntityIdByComponentIndex(TComponentIndex aComponentIndex);

private:
    TComponentsMap  mComponentsMap;
    TComponentArray mComponents;
    uint32 mComponentsCount;
};


template< typename CompType >
inline CComponentManager< CompType >::CComponentManager()
    : mComponentsCount(0)
{ ; }


template<typename CompType>
inline bool CComponentManager<CompType>::Init()
{
    mComponents.fill(CompType());
    return true;
}


template<typename CompType>
inline void CComponentManager<CompType>::End()
{
    // Nothing to do here.
}


template<typename CompType>
inline CompType* CComponentManager<CompType>::GetComponent(TEntityId aEntityId)
{
    CompType* lComponent = nullptr;

    if (mComponentsMap.count(aEntityId) != 0)
    {
        lComponent = mComponentsMap[aEntityId];
    }

    return lComponent;
}


template< typename CompType >
inline CompType* CComponentManager< CompType >::AddComponent(TEntityId aEntityId)
{
    CompType* lComponent = nullptr;
    MAZ_ASSERT((mComponentsMap.count(aEntityId) == 0), "[CComponentManager]::AddComponent - Trying to add a component to entity with ID %u, but this entity already has a component of that type!", aEntityId.mId);
    if ((mComponentsMap.count(aEntityId) == 0) && (mComponentsCount < kMaxComponents - 1))
    {
        mComponentsMap[aEntityId] = mComponentsCount;
        lComponent = &mComponents[mComponentsCount++];
    }
}


template< typename CompType >
inline bool CComponentManager< CompType >::RemoveComponent(TEntityId aEntityId)
{
    bool lOk = mComponentsMap.count(aEntityId);
    if (lOk)
    {
        // Update components array
        TComponentIndex lRemoveIndex = mComponentsMap[aEntityId];
        TComponentIndex lLastIndex = mComponentsCount - 1;
        mComponents[lRemoveIndex] = mComponents[lLastIndex];
        mComponentsMap.erase(aEntityId);
        --mComponentsCount;

        // Update components map
        TEntityId lEntityWithMovedComponent = GetEntityIdByComponentIndex(lLastIndex);
        mComponentsMap[lEntityWithMovedComponent] = lRemoveIndex;
    }
    return lOk;
}


template<typename CompType>
inline TEntityId CComponentManager<CompType>::GetEntityIdByComponentIndex(TComponentIndex aComponentIndex)
{
    TEntityId lEntityId;

    for (TComponentsMap::const_iterator lIt = mComponentsMap.k.begin(), lEnd = mComponentsMap.end(); (lIt != lEnd) && !lEntityId.IsValid(); ++lIt)
    {
        if (lIt->second == aComponentIndex)
        {
            lEntityId = lIt->first;
        }
    }

    return lEntityId;
}

} // ecs
} // maz

#endif // !_H_C_COMPONENT_MANAGER_


#include "CComponentsManager.h"
#include <scenes/CEntity.h>


namespace maz
{

CComponentsManager::CComponentsManager()
{
    mComponentManagers.fill(nullptr);
}


void CComponentsManager::updateComponentIdForEntity(CReference<CEntity> aEntity, EComponentType aComponentType, uint16 aId)
{
    if (aEntity)
    {
        aEntity->updateComponentId(aComponentType, aId);
    }
}


void CComponentsManager::Shutdown()
{
    for (uint16 i = 0, iCount = static_cast<uint16>(mComponentManagers.size()); i < iCount; ++i)
    {
        if (mComponentManagers[i] != nullptr)
        {
            mComponentManagers[i]->Shutdown(this);
            MAZ_DELETE(mComponentManagers[i]);
            mComponentManagers[i] = nullptr;
        }
    }
}


void CComponentsManager::RefreshComponents()
{
    for (uint16 i = 0, iCount = static_cast<uint16>(mComponentManagers.size()); i < iCount; ++i)
    {
        if (mComponentManagers[i] != nullptr)
        {
            mComponentManagers[i]->RefreshComponents(this);
        }
    }
}


bool CComponentsManager::RemoveComponent(EComponentType aComponentType, const uint16 aComponentIndex)
{
    CComponentManagerBase* lpComponentManager = mComponentManagers[EnumToNumber(aComponentType)];
    MAZ_ASSERT(lpComponentManager != nullptr
        , "Component manager of the desired type (%hu) has not been created been registered!", EnumToNumber(aComponentType));
    return lpComponentManager->RemoveComponent(aComponentIndex);
}

} // maz

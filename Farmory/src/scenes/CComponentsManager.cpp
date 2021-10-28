#include "CComponentsManager.h"
#include <scenes/CGameObject.h>


namespace maz
{

CComponentsManager::CComponentsManager()
{
    mComponentManagers.fill(nullptr);
}


void CComponentsManager::updateComponentIdForGameObject(CReference<CGameObject> aGameObject, EComponentType aComponentType, uint16 aId)
{
    if (aGameObject)
    {
        aGameObject->updateComponentId(aComponentType, aId);
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
    CComponentManagerBase* componentManager = mComponentManagers[EnumToNumber(aComponentType)];
    MAZ_ASSERT(componentManager != nullptr
        , "Component manager of the desired type (%hu) has not been created been registered!", EnumToNumber(aComponentType));
    return componentManager->RemoveComponent(aComponentIndex);
}


} // maz

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
    aGameObject->updateComponentId(aComponentType, aId);
}


void CComponentsManager::RefreshComponents()
{
    for (uint16 i = 0, iCount = static_cast<uint16>(mComponentManagers.size()); i < iCount; ++i)
    {
        MAZ_ASSERT(mComponentManagers[i] != nullptr
            , "[CComponentsManager]::RefreshComponents - Component of the desired type (%hu) has already been registered!", i);
        mComponentManagers[i]->RefreshComponents(this);
    }
}


} // maz

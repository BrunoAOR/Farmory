#include "CSystemsManager.h"
#include <scenes/CEntitiesManager.h>
#include <scenes/SComponentsSignature.h>


namespace maz
{

CSystemsManager::CSystemsManager(CEntitiesManager& arEntitiesManager)
    : mrEntitiesManager(arEntitiesManager)
    , mSystemsCount(0)
{
    mSystems.fill(nullptr);
}


CSystemsManager::~CSystemsManager()
{
    MAZ_ASSERT(mSystemsCount == 0, "Destructor called while there are systems created. Forgot to call Shutdown?");
}


void CSystemsManager::Shutdown()
{
    for (ISystem* lpSystem : mSystems)
    {
        if (lpSystem != nullptr)
        {
            lpSystem->Shutdown();
            MAZ_DELETE(lpSystem);
            --mSystemsCount;
        }
    }
    mSystems.fill(nullptr);
    MAZ_ASSERT(mSystemsCount == 0, "There are still systems created even after Shutdown. This should not have happened!");
}


void CSystemsManager::RefreshSystems()
{
    // Remove Entities from systems
    for (uint16 i = 0; i < mSystemsCount; ++i)
    {
        ISystem* lpSystem = mSystems[i];
        const SComponentsSignature& lSystemSignature = lpSystem->GetSignature();
        for (uint16 j = 0, jCount = static_cast<uint16>(lpSystem->mEntities.size()); j < jCount; ++j)
        {
            CReference<CEntity> lEntity = lpSystem->mEntities[j];
            if (!lEntity)
            {
                // Entity has been invalidated and shall be removed from the System
                lpSystem->mEntities[j] = CReference<CEntity>();
            }
            else if (lEntity->IsSignatureDirty())
            {
                MAZ_ASSERT(lSystemSignature.IsSubsetOf(lEntity->GetPreviousSignature()), "Somehow, an Entity used by this system did not have a matching Signature in the frame!");
                if (!lSystemSignature.IsSubsetOf(lEntity->GetSignature()))
                {
                    // So, the previous Entity signature did match the System but the current one doesn't, we remove the Entity
                    lpSystem->mEntities[j] = CReference<CEntity>();
                }
            }
        }
    }

    // Add Entities to systems
    CEntitiesManager::CModifiedEntitiesIterator lModifiedEntitiesIterator = mrEntitiesManager.GetModifiedEntitiesIterator();
    while (lModifiedEntitiesIterator)
    {
        CReference<CEntity> lEntity = lModifiedEntitiesIterator.Get();
        MAZ_ASSERT(lEntity->IsSignatureDirty(), "An Entity contained in CEntitiesManager::CModifiedEntitiesIterator returns false when calling IsSignatureDirty!");
        for (uint16 i = 0; i < mSystemsCount; ++i)
        {
            ISystem* lpSystem = mSystems[i];
            const SComponentsSignature& lrSystemSignature = lpSystem->GetSignature();
            if (!lrSystemSignature.IsSubsetOf(lEntity->GetPreviousSignature())
                && lrSystemSignature.IsSubsetOf(lEntity->GetSignature()))
            {
                // So, the previous Entity signature did NOT match the System but the current one DOES, we add the Entity in the first-most available slot
                bool lAdded = false;
                for (uint16 j = 0, jCount = static_cast<uint16>(lpSystem->mEntities.size()); (j < jCount) && !lAdded; ++j)
                {
                    lAdded = !(lpSystem->mEntities[j]);
                    if (lAdded)
                    {
                        lpSystem->mEntities[j] = lEntity;
                    }
                }
                MAZ_ASSERT(lAdded, "Failed to add an Entity with matching signature to one of the systems!");
            }
        }
        ++lModifiedEntitiesIterator;
    }

    // Update systems
    for (uint16 i = 0; i < mSystemsCount; ++i)
    {
        mSystems[i]->Update();
    }
}

} // maz

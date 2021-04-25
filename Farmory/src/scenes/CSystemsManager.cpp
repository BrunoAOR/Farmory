#include "CSystemsManager.h"
#include <scenes/CGameObjectsManager.h>
#include <scenes/SComponentsSignature.h>


namespace maz
{

CSystemsManager::CSystemsManager(CGameObjectsManager& aGameObjectsManager)
    : mGameObjectsManager(aGameObjectsManager)
    , mSystemsCount(0)
{
    mSystems.fill(nullptr);
}


CSystemsManager::~CSystemsManager()
{
    MAZ_ASSERT(mSystemsCount == 0, "CSystemsManager::~CSystemsManager - Destructor called while there are systems created. Forgot to call Shutdown?");
}


void CSystemsManager::Shutdown()
{
    for (ISystem* system : mSystems)
    {
        if (system != nullptr)
        {
            system->Shutdown();
            MAZ_DELETE(system);
            --mSystemsCount;
        }
    }
    mSystems.fill(nullptr);
    MAZ_ASSERT(mSystemsCount == 0, "CSystemsManager::Shutdown - There are still systems created even after Shutdown. This should not have happened!");
}


void CSystemsManager::RefreshSystems()
{
    // Remove GameObjects from systems
    for (uint16 i = 0; i < mSystemsCount; ++i)
    {
        ISystem* system = mSystems[i];
        const SComponentsSignature& systemSignature = system->GetSignature();
        for (uint16 j = 0, jCount = static_cast<uint16>(system->mGameObjects.size()); j < jCount; ++j)
        {
            CReference<CGameObject> gameObject = system->mGameObjects[j];
            if (gameObject && gameObject->IsSignatureDirty())
            {
                if (systemSignature.IsSubsetOf(gameObject->GetPreviousSignature())
                    && !systemSignature.IsSubsetOf(gameObject->GetSignature()))
                {
                    // So, the previous GameObject signature did match the System but the current one doesn't, we rremove the gameobject
                    system->mGameObjects[j] = CReference<CGameObject>();
                }
            }
        }
    }

    // Add GameObjects to systems
    CGameObjectsManager::CModifiedGameObjectsIterator modifiedGameObjectsIterator = mGameObjectsManager.GetModifiedGameObjectsIterator();
    while (modifiedGameObjectsIterator)
    {
        CReference<CGameObject> gameObject = modifiedGameObjectsIterator.Get();
        MAZ_ASSERT(gameObject->IsSignatureDirty(), "CSystemsManager::RefreshSystems - A gameobject contained in CGameObjectsManager::CModifiedGameObjectsIterator returns false when calling IsSignatureDirty!");
        for (uint16 i = 0; i < mSystemsCount; ++i)
        {
            ISystem* system = mSystems[i];
            const SComponentsSignature& systemSignature = system->GetSignature();
            if (!systemSignature.IsSubsetOf(gameObject->GetPreviousSignature())
                && systemSignature.IsSubsetOf(gameObject->GetSignature()))
            {
                // So, the previous GameObject signature did NOT match the System but the current one DOES, we add the gameobject in the first-most available slot
                bool added = false;
                for (uint16 j = 0, jCount = static_cast<uint16>(system->mGameObjects.size()); (j < jCount) && !added; ++j)
                {
                    added = !(system->mGameObjects[j]);
                    if (added)
                    {
                        system->mGameObjects[j] = gameObject;
                    }
                }
            }
        }
        ++modifiedGameObjectsIterator;
    }


    // Update systems
    for (uint16 i = 0; i < mSystemsCount; ++i)
    {
        mSystems[i]->Update();
    }
}

} // maz

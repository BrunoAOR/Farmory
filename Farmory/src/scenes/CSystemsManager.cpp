#include "CSystemsManager.h"
#include <scenes/CGameObjectsManager.h>
#include <scenes/SComponentsSignature.h>


namespace maz
{

CSystemsManager::CSystemsManager(CGameObjectsManager& arGameObjectsManager)
    : mrGameObjectsManager(arGameObjectsManager)
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
    // Remove GameObjects from systems
    for (uint16 i = 0; i < mSystemsCount; ++i)
    {
        ISystem* lpSystem = mSystems[i];
        const SComponentsSignature& lSystemSignature = lpSystem->GetSignature();
        for (uint16 j = 0, jCount = static_cast<uint16>(lpSystem->mGameObjects.size()); j < jCount; ++j)
        {
            CReference<CGameObject> lGameObject = lpSystem->mGameObjects[j];
            if (!lGameObject)
            {
                // GameObject has been invalidated and shall be removed from the System
                lpSystem->mGameObjects[j] = CReference<CGameObject>();
            }
            else if (lGameObject->IsSignatureDirty())
            {
                MAZ_ASSERT(lSystemSignature.IsSubsetOf(lGameObject->GetPreviousSignature()), "Somehow, a gameObject used by this system did not have a matching Signature in the frame!");
                if (!lSystemSignature.IsSubsetOf(lGameObject->GetSignature()))
                {
                    // So, the previous GameObject signature did match the System but the current one doesn't, we remove the gameobject
                    lpSystem->mGameObjects[j] = CReference<CGameObject>();
                }
            }
        }
    }

    // Add GameObjects to systems
    CGameObjectsManager::CModifiedGameObjectsIterator lModifiedGameObjectsIterator = mrGameObjectsManager.GetModifiedGameObjectsIterator();
    while (lModifiedGameObjectsIterator)
    {
        CReference<CGameObject> lGameObject = lModifiedGameObjectsIterator.Get();
        MAZ_ASSERT(lGameObject->IsSignatureDirty(), "A gameobject contained in CGameObjectsManager::CModifiedGameObjectsIterator returns false when calling IsSignatureDirty!");
        for (uint16 i = 0; i < mSystemsCount; ++i)
        {
            ISystem* lpSystem = mSystems[i];
            const SComponentsSignature& lrSystemSignature = lpSystem->GetSignature();
            if (!lrSystemSignature.IsSubsetOf(lGameObject->GetPreviousSignature())
                && lrSystemSignature.IsSubsetOf(lGameObject->GetSignature()))
            {
                // So, the previous GameObject signature did NOT match the System but the current one DOES, we add the gameobject in the first-most available slot
                bool lAdded = false;
                for (uint16 j = 0, jCount = static_cast<uint16>(lpSystem->mGameObjects.size()); (j < jCount) && !lAdded; ++j)
                {
                    lAdded = !(lpSystem->mGameObjects[j]);
                    if (lAdded)
                    {
                        lpSystem->mGameObjects[j] = lGameObject;
                    }
                }
                MAZ_ASSERT(lAdded, "Failed to add a GameObject with matching signature to one of the systems!");
            }
        }
        ++lModifiedGameObjectsIterator;
    }

    // Update systems
    for (uint16 i = 0; i < mSystemsCount; ++i)
    {
        mSystems[i]->Update();
    }
}

} // maz

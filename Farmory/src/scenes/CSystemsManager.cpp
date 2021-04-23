#include "CSystemsManager.h"


namespace maz
{

CSystemsManager::CSystemsManager()
    : mSystemsCount(0)
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
    TODO;
    // I should be adding and removing gameobjects from the systems here based on wether the gameobjects signatures match the systems signature
    // This is something that can't be done by each system at the ISystem level through a non-virtual ISystem::Refresh call, because systems only know about their current GameObjects.
    // The CSystemManager will probably need access to the CGameObjectsManager to access the full list of GameObjects and go through them one by one checking if they match systems.
    // To speed things up, GameObjects should have a flag indicating if their component list has changed in this frame (set by CComponentsManager perhaps?)

    for (ISystem* system : mSystems)
    {
        system->Update();
    }
}

} // maz

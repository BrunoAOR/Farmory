#include "CGameObjectsManager.h"


namespace maz
{

CGameObjectsManager::CGameObjectsManager(CComponentsManager& aComponentsManager)
    : mComponentsManager(aComponentsManager)
{}


void CGameObjectsManager::Shutdown()
{
    typename CGameObjectsBuffer::CBufferIterator iterator = mGameObjectsBuffer.GetIterator(CGameObjectsBuffer::EIteratorFlags::NONE);

    while (iterator)
    {
        // Remove all components
        for (uint16 componentId = 0; componentId < MAZ_ENUM_COUNT(EComponentType); ++componentId)
        {
            iterator.Get()->RemoveComponent(static_cast<EComponentType>(componentId));
        }
        ++iterator;
    }
    mGameObjectsBuffer.Clear();
}


void CGameObjectsManager::RefreshGameObjects()
{
    typename CGameObjectsBuffer::CBufferIterator iterator = mGameObjectsBuffer.GetIterator(static_cast<CGameObjectsBuffer::EIteratorFlags>(CGameObjectsBuffer::EIteratorFlags::PROCESS_ADD_PENDING | CGameObjectsBuffer::EIteratorFlags::PROCESS_REMOVE_PENDING));

    while (iterator)
    {
        if (iterator.HasIteratorFlag(CGameObjectsBuffer::EIteratorFlags::PROCESS_ADD_PENDING))
        {
            // Nothing to do here other than clearing the flag
            iterator.ClearIteratorFlag(CGameObjectsBuffer::EIteratorFlags::PROCESS_ADD_PENDING);
        }
        else if (iterator.HasIteratorFlag(CGameObjectsBuffer::EIteratorFlags::PROCESS_REMOVE_PENDING))
        {
            // Remove all components
            for (uint16 componentId = 0; componentId < MAZ_ENUM_COUNT(EComponentType); ++componentId)
            {
                iterator.Get()->RemoveComponent(static_cast<EComponentType>(componentId));
            }
            bool removed = iterator.RemoveElement();
            MAZ_ASSERT(removed, "CGameObjectsManager::RefreshGameObjects - Failed to remove Gameobject with id %hhu that was flagged for removal!", iterator.GetId());
            iterator.ClearIteratorFlag(CGameObjectsBuffer::EIteratorFlags::PROCESS_REMOVE_PENDING);
        }
        ++iterator;
    }
}


CReference<CGameObject> CGameObjectsManager::CreateGameObject(CGameObject* aParent, const CFixedString32& aName)
{
    CReference<CGameObject> lGameObject;
    const uint16 gameObjectId = mGameObjectsBuffer.GetNextAvailableId();
    if (gameObjectId != kInvalidElementId)
    {
        const uint16 actualGameObjectId = mGameObjectsBuffer.AddElement(gameObjectId, mComponentsManager, aParent, aName);
        MAZ_ASSERT(gameObjectId == actualGameObjectId, "CGameObjectsManager::CreateGameObject - The next available id in the GameObjectsBuffer does not match the actual id used for the creation of the gameobject!");
        lGameObject = mGameObjectsBuffer.GetElement(actualGameObjectId);
        lGameObject->mThis = lGameObject;
    }

    return lGameObject;
}


bool CGameObjectsManager::RequestDestroyGameObject(const uint16 aGameObjectId)
{
    return mGameObjectsBuffer.FlagElementForRemoval(aGameObjectId);
}

} // maz

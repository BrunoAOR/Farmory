#include "CGameObjectsManager.h"


namespace maz
{

CGameObjectsManager::CGameObjectsManager(CComponentsManager& arComponentsManager)
    : mrComponentsManager(arComponentsManager)
{}


void CGameObjectsManager::Shutdown()
{
    typename CGameObjectsBuffer::CBufferIterator lIt = mGameObjectsBuffer.GetIterator(CGameObjectsBuffer::EIteratorFlags::ANY);

    while (lIt)
    {
        // Remove all components
        for (uint16 lComponentId = 0; lComponentId < EnumCount<EComponentType>(); ++lComponentId)
        {
            lIt.Get()->RemoveComponent(static_cast<EComponentType>(lComponentId));
        }
        ++lIt;
    }
    mGameObjectsBuffer.Clear();
}


void CGameObjectsManager::RefreshGameObjects()
{
    typename CGameObjectsBuffer::CBufferIterator lIt = mGameObjectsBuffer.GetIterator(CGameObjectsBuffer::EIteratorFlags::ANY);

    while (lIt)
    {
        if (lIt.HasIteratorFlag(CGameObjectsBuffer::EIteratorFlags::PROCESS_ADD_PENDING))
        {
            // Nothing to do here other than clearing the flag
            lIt.ClearIteratorFlag(CGameObjectsBuffer::EIteratorFlags::PROCESS_ADD_PENDING);
        }
        else if (lIt.HasIteratorFlag(CGameObjectsBuffer::EIteratorFlags::PROCESS_REMOVE_PENDING))
        {
            // Remove all components
            for (uint16 lComponentId = 0; lComponentId < EnumCount<EComponentType>(); ++lComponentId)
            {
                lIt.Get()->RemoveComponent(static_cast<EComponentType>(lComponentId));
            }
            bool lRemoved = lIt.RemoveElement();
            MAZ_ASSERT(lRemoved, "Failed to remove Gameobject with id %hhu that was flagged for removal!", lIt.GetId());
            lIt.ClearIteratorFlag(CGameObjectsBuffer::EIteratorFlags::PROCESS_REMOVE_PENDING);
        }
        else
        {
            // Clean up signature state in preparation for updates coming from the ComponentsManager
            CReference<CGameObject> lGameObject = lIt.Get();
            lGameObject->mIsSignatureDirty = false;
            lGameObject->mPreviousSignature = lGameObject->mSignature;
        }
        ++lIt;
    }
}


CReference<CGameObject> CGameObjectsManager::CreateGameObject(const CFixedString32& arName)
{
    CReference<CGameObject> lGameObject;
    const uint16 lGameObjectId = mGameObjectsBuffer.GetNextAvailableId();
    if (lGameObjectId != kInvalidElementId)
    {
        const uint16 lActualGameObjectId = mGameObjectsBuffer.AddElement(lGameObjectId, mrComponentsManager, arName);
        MAZ_ASSERT(lGameObjectId == lActualGameObjectId, "The next available id in the GameObjectsBuffer does not match the actual id used for the creation of the gameobject!");
        lGameObject = mGameObjectsBuffer.GetElement(lActualGameObjectId);
        lGameObject->mThis = lGameObject;
    }

    return lGameObject;
}


bool CGameObjectsManager::RequestDestroyGameObject(const uint16 aGameObjectId)
{
    return mGameObjectsBuffer.FlagElementForRemoval(aGameObjectId);
}


CGameObjectsManager::CModifiedGameObjectsIterator CGameObjectsManager::GetModifiedGameObjectsIterator()
{
    const typename CGameObjectsBuffer::CBufferIterator lIt = mGameObjectsBuffer.GetIterator(CGameObjectsBuffer::EIteratorFlags::ANY);
    return CModifiedGameObjectsIterator(lIt);
}


CGameObjectsManager::CModifiedGameObjectsIterator::operator bool()
{
    return static_cast<bool>(mInternalIterator);
}


void CGameObjectsManager::CModifiedGameObjectsIterator::operator++()
{
    ++mInternalIterator;
    while (mInternalIterator && !mInternalIterator.Get()->mIsSignatureDirty)
    {
        ++mInternalIterator;
    }
}


CReference<CGameObject> CGameObjectsManager::CModifiedGameObjectsIterator::Get()
{
    return mInternalIterator.Get();
}


CGameObjectsManager::CModifiedGameObjectsIterator::CModifiedGameObjectsIterator(const CGameObjectsBuffer::CBufferIterator& arIterator)
    : mInternalIterator(arIterator)
{
    while (mInternalIterator && !mInternalIterator.Get()->mIsSignatureDirty)
    {
        ++mInternalIterator;
    }
}

} // maz

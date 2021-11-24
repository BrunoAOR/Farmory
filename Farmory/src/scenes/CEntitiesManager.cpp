#include "CEntitiesManager.h"


namespace maz
{

CEntitiesManager::CEntitiesManager(CComponentsManager& arComponentsManager)
    : mrComponentsManager(arComponentsManager)
{}


void CEntitiesManager::Shutdown()
{
    typename CEntitiesBuffer::CBufferIterator lIt = mEntitiesBuffer.GetIterator(CEntitiesBuffer::EIteratorFlags::ANY);

    while (lIt)
    {
        // Remove all components
        for (uint16 lComponentId = 0; lComponentId < EnumCount<EComponentType>(); ++lComponentId)
        {
            lIt.Get()->RemoveComponent(static_cast<EComponentType>(lComponentId));
        }
        ++lIt;
    }
    mEntitiesBuffer.Clear();
}


void CEntitiesManager::RefreshEntities()
{
    typename CEntitiesBuffer::CBufferIterator lIt = mEntitiesBuffer.GetIterator(CEntitiesBuffer::EIteratorFlags::ANY);

    while (lIt)
    {
        if (lIt.HasIteratorFlag(CEntitiesBuffer::EIteratorFlags::PROCESS_ADD_PENDING))
        {
            // Nothing to do here other than clearing the flag
            lIt.ClearIteratorFlag(CEntitiesBuffer::EIteratorFlags::PROCESS_ADD_PENDING);
        }
        else if (lIt.HasIteratorFlag(CEntitiesBuffer::EIteratorFlags::PROCESS_REMOVE_PENDING))
        {
            // Remove all components
            for (uint16 lComponentId = 0; lComponentId < EnumCount<EComponentType>(); ++lComponentId)
            {
                lIt.Get()->RemoveComponent(static_cast<EComponentType>(lComponentId));
            }
            bool lRemoved = lIt.RemoveElement();
            MAZ_ASSERT(lRemoved, "Failed to remove Entity with id %hhu that was flagged for removal!", lIt.GetId());
            lIt.ClearIteratorFlag(CEntitiesBuffer::EIteratorFlags::PROCESS_REMOVE_PENDING);
        }
        else
        {
            // Clean up signature state in preparation for updates coming from the ComponentsManager
            CReference<CEntity> lEntity = lIt.Get();
            lEntity->mIsSignatureDirty = false;
            lEntity->mPreviousSignature = lEntity->mSignature;
        }
        ++lIt;
    }
}


CReference<CEntity> CEntitiesManager::CreateEntity(const CFixedString32& arName)
{
    CReference<CEntity> lEntity;
    const uint16 lEntityId = mEntitiesBuffer.GetNextAvailableId();
    if (lEntityId != kInvalidElementId)
    {
        const uint16 lActualEntityId = mEntitiesBuffer.AddElement(lEntityId, mrComponentsManager, arName);
        MAZ_ASSERT(lEntityId == lActualEntityId, "The next available id in the EntitiesBuffer does not match the actual id used for the creation of the entity!");
        lEntity = mEntitiesBuffer.GetElement(lActualEntityId);
        lEntity->mThis = lEntity;
    }

    return lEntity;
}


bool CEntitiesManager::RequestDestroyEntity(const uint16 aEntityId)
{
    return mEntitiesBuffer.FlagElementForRemoval(aEntityId);
}


CEntitiesManager::CModifiedEntitiesIterator CEntitiesManager::GetModifiedEntitiesIterator()
{
    const typename CEntitiesBuffer::CBufferIterator lIt = mEntitiesBuffer.GetIterator(CEntitiesBuffer::EIteratorFlags::ANY);
    return CModifiedEntitiesIterator(lIt);
}


CEntitiesManager::CModifiedEntitiesIterator::operator bool()
{
    return static_cast<bool>(mInternalIterator);
}


void CEntitiesManager::CModifiedEntitiesIterator::operator++()
{
    ++mInternalIterator;
    while (mInternalIterator && !mInternalIterator.Get()->mIsSignatureDirty)
    {
        ++mInternalIterator;
    }
}


CReference<CEntity> CEntitiesManager::CModifiedEntitiesIterator::Get()
{
    return mInternalIterator.Get();
}


CEntitiesManager::CModifiedEntitiesIterator::CModifiedEntitiesIterator(const CEntitiesBuffer::CBufferIterator& arIterator)
    : mInternalIterator(arIterator)
{
    while (mInternalIterator && !mInternalIterator.Get()->mIsSignatureDirty)
    {
        ++mInternalIterator;
    }
}

} // maz

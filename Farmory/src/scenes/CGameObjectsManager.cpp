#include "CGameObjectsManager.h"


namespace maz
{

CGameObjectsManager::CGameObjectsManager(CComponentsManager& aComponentsManager)
    : mComponentsManager(aComponentsManager)
{
    for (size_t i = 0, iCount = mGameObjects.size(); i < iCount; ++i)
    {
        mGameObjects[i] = std::move(CReferenceHolder<CGameObject>());
    }
    std::memset(mGameObjectsBuffer, 0, sizeof(CGameObject) * kMaxGameObjects);
    mGameObjectsBufferUseFlag.fill(false);
}


void CGameObjectsManager::Shutdown()
{
    for (uint16 i = 0, iCount = static_cast<uint16>(mGameObjectsBufferUseFlag.size()); (i < iCount); ++i)
    {
        if (mGameObjectsBufferUseFlag[i])
        {
            DestroyGameOjbect(i);
        }
    }
}

void CGameObjectsManager::RefreshGameObjects()
{
    for (size_t i = 0, iCount = mGameObjectsBufferUseFlag.size(); (i < iCount); ++i)
    {
        if (mGameObjectsBufferUseFlag[i])
        {
            MAZ_ASSERT(false, "[CGameObjectsManager]::RefreshGameObjects - NOT IMPLEMENTED!");
            ///index = static_cast<uint16>(i);
        }
    }
}


CReference<CGameObject> CGameObjectsManager::CreateGameObject(CGameObject* aParent, const CFixedString32& aName)
{
    CReference<CGameObject> lGameObject;
    uint16 index = kInvalidGameObjectId;
    for (size_t i = 0, iCount = mGameObjectsBufferUseFlag.size(); (i < iCount) && (index == kInvalidGameObjectId); ++i)
    {
        if (!mGameObjectsBufferUseFlag[i])
        {
            mGameObjectsBufferUseFlag[i] = true;
            index = static_cast<uint16>(i);
        }
    }
    MAZ_ASSERT(index != kInvalidGameObjectId, "[CGameObjectsManager]::CreateGameObject - Failed to find an available slot for GameObject!");
    if (index != kInvalidGameObjectId)
    {
        mGameObjects[index] = CReferenceHolder<CGameObject>(MAZ_PLACEMENT_NEW(&(mGameObjectsBuffer[sizeof(CGameObject) * index]), CGameObject, index, mComponentsManager, aParent, aName));
        lGameObject = mGameObjects[index].GetReference();
        lGameObject->mThis = lGameObject;
    }

    return lGameObject;
}

bool CGameObjectsManager::DestroyGameOjbect(uint16 aGameObjectId)
{
    bool lOk = mGameObjectsBufferUseFlag[aGameObjectId];

    if (lOk)
    {
        for (uint16 componentId = 0; componentId < MAZ_ENUM_COUNT(EComponentType); ++componentId)
        {
            mGameObjects[aGameObjectId]->RemoveComponent(static_cast<EComponentType>(componentId));
        }
        mGameObjects[aGameObjectId].~CReferenceHolder<CGameObject>();
        reinterpret_cast<CGameObject*>(&(mGameObjectsBuffer[sizeof(CGameObject) * aGameObjectId]))->~CGameObject();
        mGameObjectsBufferUseFlag[aGameObjectId] = false;
    }

    return lOk;
}

} // maz

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
        mGameObjects[aGameObjectId].~CReferenceHolder<CGameObject>();
        reinterpret_cast<CGameObject*>(&(mGameObjectsBuffer[sizeof(CGameObject) * aGameObjectId]))->~CGameObject();
        mGameObjectsBufferUseFlag[aGameObjectId] = false;
    }

    return lOk;
}

} // maz

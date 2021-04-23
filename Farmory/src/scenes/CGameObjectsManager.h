#ifndef _H_C_GAME_OBJECTS_MANAGER_
#define _H_C_GAME_OBJECTS_MANAGER_

#include <maz/globals.h>
#include <array>
#include <maz/CReferenceMaster.h>
#include <scenes/CGameObject.h>

namespace maz
{

class CGameObjectsManager
{
    friend class CGameObjectsIterator;
public:
    CGameObjectsManager(CComponentsManager& aComponentsManager);

    void Shutdown();
    void RefreshGameObjects();
    CReference<CGameObject> CreateGameObject(CGameObject* aParent, const CFixedString32& aName);
    bool DestroyGameOjbect(uint16 aGameObjectId);


private:
    CComponentsManager& mComponentsManager;

    std::array<CReferenceHolder<CGameObject>, kMaxGameObjectsCount> mGameObjects;
    uint8 mGameObjectsBuffer[sizeof(CGameObject) * kMaxGameObjectsCount];
    std::array<bool, kMaxGameObjectsCount> mGameObjectsBufferUseFlag;


public:
    class CGameObjectsIterator
    {
        
    public:
        operator bool()
        {
            return static_cast<bool>(mGameObject);
        }

        CGameObjectsIterator operator++()
        {
            for (uint16 i = mCurrentIndex + 1; i < kMaxGameObjectsCount; ++i)
            {
                TODO; //- after implementing CGameObjectsManager::RefreshGameObjects
            }
        }

    private:
        friend class CGameObjectManager;
        CGameObjectsIterator(CGameObjectsManager* aGameObjectsManager, uint16 aCurrentIndex)
            : mManager(aGameObjectsManager)
            , mCurrentIndex(aCurrentIndex)
        {
            MAZ_ASSERT(mManager != nullptr, "CGameObjectsIterator::CGameObjectsIterator - Attempting to construct a CGameObjectsIterator without providing a valid CGameObjectsManager pointer!");
            mGameObject = mManager->mGameObjects[mCurrentIndex].GetReference();
        }

        CGameObjectsManager* mManager;
        uint16 mCurrentIndex;
        CReference<CGameObject> mGameObject;
    };
};

} // maz

#endif // !_H_C_GAME_OBJECTS_MANAGER_

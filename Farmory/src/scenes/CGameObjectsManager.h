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
};

} // maz

#endif // !_H_C_GAME_OBJECTS_MANAGER_

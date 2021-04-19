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
public:
    CGameObjectsManager(CComponentsManager& aComponentsManager);

    void Shutdown();
    void RefreshGameObjects();
    CReference<CGameObject> CreateGameObject(CGameObject* aParent, const CFixedString32& aName);
    bool DestroyGameOjbect(uint16 aGameObjectId);

private:
    CComponentsManager& mComponentsManager;

    std::array<CReferenceHolder<CGameObject>, kMaxGameObjects> mGameObjects;
    uint8 mGameObjectsBuffer[sizeof(CGameObject) * kMaxGameObjects];
    std::array<bool, kMaxGameObjects> mGameObjectsBufferUseFlag;
};

} // maz

#endif // !_H_C_GAME_OBJECTS_MANAGER_

#ifndef _H_C_GAME_OBJECTS_MANAGER_
#define _H_C_GAME_OBJECTS_MANAGER_

#include <maz/globals.h>
#include <array>
#include <maz/CReferenceMasterBuffer.h>
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
    bool RequestDestroyGameObject(const uint16 aGameObjectId);


private:
    CComponentsManager& mComponentsManager;
    using CGameObjectsBuffer = CReferenceHolderBuffer<CGameObject, kMaxGameObjectsCount>;
    CGameObjectsBuffer mGameObjectsBuffer;
};

} // maz

#endif // !_H_C_GAME_OBJECTS_MANAGER_

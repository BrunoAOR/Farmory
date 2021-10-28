#ifndef _H_C_GAME_OBJECTS_MANAGER_
#define _H_C_GAME_OBJECTS_MANAGER_

#include <maz/globals.h>
#include <array>
#include <maz/CReferenceBuffer.h>
#include <scenes/CGameObject.h>

namespace maz
{

class CGameObjectsManager
{
    using CGameObjectsBuffer = CReferenceBuffer<CGameObject, kMaxGameObjectsCount>;
public:
    class CModifiedGameObjectsIterator
    {
        friend class CGameObjectsManager;
    public:
        operator bool();
        void operator ++();
        CReference<CGameObject> Get();

    private:
        CModifiedGameObjectsIterator(const CGameObjectsBuffer::CBufferIterator& aIterator);

    private:
        CGameObjectsBuffer::CBufferIterator mInternalIterator;
    };


public:
    CGameObjectsManager(CComponentsManager& aComponentsManager);

    void Shutdown();
    void RefreshGameObjects();
    CReference<CGameObject> CreateGameObject(const CFixedString32& aName);
    bool RequestDestroyGameObject(const uint16 aGameObjectId);
    CModifiedGameObjectsIterator GetModifiedGameObjectsIterator();

private:
    CComponentsManager& mComponentsManager;
    CGameObjectsBuffer mGameObjectsBuffer;
};

} // maz

#endif // !_H_C_GAME_OBJECTS_MANAGER_

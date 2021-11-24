#ifndef _H_C_ENTITIES_MANAGER_
#define _H_C_ENTITIES_MANAGER_

#include <maz/globals.h>
#include <array>
#include <maz/CReferenceBuffer.h>
#include <scenes/CGameObject.h>


namespace maz
{

class CEntitiesManager
{
    using CEntitiesBuffer = CReferenceBuffer<CEntity, kMaxEntitiesCount>;

public:
    class CModifiedEntitiesIterator
    {
        friend class CEntitiesManager;
    public:
        operator bool();
        void operator ++();
        CReference<CEntity> Get();

    private:
        CModifiedEntitiesIterator(const CEntitiesBuffer::CBufferIterator& arIterator);

    private:
        CEntitiesBuffer::CBufferIterator mInternalIterator;
    };


public:
    CEntitiesManager(CComponentsManager& arComponentsManager);

    void Shutdown();
    void RefreshEntities();
    CReference<CEntity> CreateEntity(const CFixedString32& arName);
    bool RequestDestroyEntity(const uint16 aEntityId);
    CModifiedEntitiesIterator GetModifiedEntitiesIterator();

private:
    CComponentsManager& mrComponentsManager;
    CEntitiesBuffer mEntitiesBuffer;
};

} // maz

#endif // !_H_C_ENTITIES_MANAGER_

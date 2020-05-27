#ifndef _H_C_ENTITY_HANDLE_
#define _H_C_ENTITY_HANDLE_

#include <ecs/CEcsManager.h>
#include <ecs/TEntityId.h>
#include <maz/types.h>

namespace maz
{
namespace ecs
{


struct CEntityHandle
{
public:
    CEntityHandle() : mId(kInvalidId), mEcsManager(nullptr) { ; }
    CEntityHandle(TEntityId aEntityId, CEcsManager* aEcsManager) : mId(aEntityId), mEcsManager(aEcsManager) { ; }
    bool IsValid() { return (mId != kInvalidId) && (mEcsManager != nullptr); }

    void Destroy();
    template< typename CompType >
    CompType* AddComponent();
    template< typename CompType >
    bool RemoveComponent();


private:
    TEntityId mId;
    CEcsManager* mEcsManager;

    friend class CEcsManager;
};


template<typename CompType>
inline CompType* CEntityHandle::AddComponent()
{
    return mEcsManager->AddComponent< CompType >(this);
}


template<typename CompType>
inline bool CEntityHandle::RemoveComponent()
{
    return mEcsManager->RemoveComponent< CompType >(this);
}

} // ecs
} // maz

#endif // !_H_C_ENTITY_HANDLE_


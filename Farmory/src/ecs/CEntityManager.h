#ifndef _H_C_ENTITY_MANAGER_
#define _H_C_ENTITY_MANAGER_

#include <ecs/TEntityId.h>


namespace maz
{
namespace ecs
{

class CEntityManager
{
public:
    CEntityManager();
    ~CEntityManager();

    TEntityId CreateEntity();
    bool DestroyEntity(TEntityId aEntityId);

private:
    TEntityId mHighestEntityId;
};

} // ecs
} // maz

#endif // !_H_C_ENTITY_MANAGER_

#ifndef _H_C_COMPONENT_MANAGER_BASE_
#define _H_C_COMPONENT_MANAGER_BASE_


namespace maz
{
namespace ecs
{

class CComponentManagerBase
{
public:
    virtual ~CComponentManagerBase() { ; }
    virtual bool Init() = 0;
    virtual void End() = 0;
};

} // ecs
} // maz

#endif // !_H_C_COMPONENT_MANAGER_BASE_


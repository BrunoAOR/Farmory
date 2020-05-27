#ifndef _H_C_COMPONENT_BASE_
#define _H_C_COMPONENT_BASE_


namespace maz
{
namespace ecs
{
namespace impl {
extern uint32 sNextComponentId;
} // impl

template< typename CompType >
class CComponent
{
public:
    CComponent();
    virtual ~CComponent() { ; }

    static uint32 GetComponentId()
    {
        static uint32 sId = impl::sNextComponentId++;
        return sId;
    }
};

} // ecs
} // maz

#endif // !_H_C_COMPONENT_BASE_


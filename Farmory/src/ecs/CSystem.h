#ifndef _H_C_SYSTEM_
#define _H_C_SYSTEM_

#include <maz/globals.h>


namespace maz
{
namespace ecs
{
namespace impl {
extern uint32 sNextSystemId;
} // impl

class CSystemBase
{
public:
    CSystemBase();
    virtual ~CSystemBase();

    virtual bool Init() = 0;
    virtual void End() = 0;
    virtual void Update() = 0;
};

template< typename CompType>
class CSystem
    : public CSystemBase
{
public:
    CSystem();
    virtual ~CSystem();

    static uint32 GetSystemId()
    {
        static uint32 sId = impl::sNextSystemId++;
        return sId;
    }
};

} // ecs
} // maz

#endif // !_H_C_SYSTEM_

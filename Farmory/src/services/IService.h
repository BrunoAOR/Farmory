#ifndef _H_I_SERVICE_
#define _H_I_SERVICE_

#include <maz/types.h>


namespace maz
{

namespace service_impl
{
extern uint16 s_nextServiceId;
} // service_impl


class IService
{
public:
    IService() { ; }
    virtual ~IService() = 0 { ; }

    virtual bool Init() = 0;
    virtual void End() = 0;
    virtual bool IsOk() const = 0;

    virtual void PreUpdate() {}
    virtual void Update() {}
    virtual void PostUpdate() {}

    virtual uint16 GetId() = 0;
};


template<typename SYSTEM_CLASS>
class CServiceBase
    : public IService
{
public:
    virtual uint16 GetId() override
    {
        return GetSystemId();
    }

    static uint16 GetSystemId()
    {
        static uint16 lServiceId = service_impl::s_nextServiceId++;
        return lServiceId;
    }
};

} // maz

#endif // !_H_I_SERVICE_

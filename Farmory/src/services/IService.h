#ifndef _H_I_SERVICE_
#define _H_I_SERVICE_

namespace maz
{

class IService
{
public:
    IService() { ; }
    virtual ~IService() = 0 { ; }

    virtual bool Init() = 0;
    virtual void End() = 0;
    virtual bool IsOk() const = 0;

    virtual void Update() = 0;
};

} // maz

#endif // !_H_I_SERVICE_

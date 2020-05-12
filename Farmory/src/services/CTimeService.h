#ifndef _H_C_TIME_SERVICE_
#define _H_C_TIME_SERVICE_

#include <services/IService.h>


namespace maz
{

class CTimeService
    : public IService
{
public:
    CTimeService();
    virtual ~CTimeService();

    virtual bool Init();
    virtual void End();
    virtual bool IsOk() const;

    virtual void Update();

    float GetDeltaTime() const;

private:
    float mLastFrameTime;
    float mDeltaTime;
};

} // maz

#endif // !_H_C_TIME_SERVICE_

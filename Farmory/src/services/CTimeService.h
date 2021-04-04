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

    virtual bool Init() override;
    virtual void End() override;
    virtual bool IsOk() const override;

    virtual void PreUpdate() override;

    float GetDeltaTime() const;

private:
    float mLastFrameTime;
    float mDeltaTime;
};

} // maz

#endif // !_H_C_TIME_SERVICE_

#ifndef _H_C_RENDER_SERVICE_
#define _H_C_RENDER_SERVICE_

#include <services/IService.h>


struct GLFWwindow;

namespace maz
{

class CRenderService
    : public IService
{
public:
    CRenderService();
    virtual ~CRenderService();

    virtual bool Init() override;
    virtual void End() override;
    virtual bool IsOk() const override;

    virtual void PreUpdate() override;
    virtual void Update() override;
    virtual void PostUpdate() override;

private:
    void OnFramebufferResized(int aWidth, int aHeight);

private:
    GLFWwindow* mpWindow;
};

} // maz

#endif // !_H_C_RENDER_SERVICE_

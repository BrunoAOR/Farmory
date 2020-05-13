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

    virtual bool Init();
    virtual void End();
    virtual bool IsOk() const;

    virtual void Update();

private:
    void OnFramebufferResized(int aWidth, int aHeight);

private:
    GLFWwindow* mWindow;
};

} // maz

#endif // !_H_C_RENDER_SERVICE_

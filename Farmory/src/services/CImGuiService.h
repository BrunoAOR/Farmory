#ifndef _H_C_IMGUI_SERVICE_
#define _H_C_IMGUI_SERVICE_

#include <services/IService.h>


struct GLFWwindow;
namespace maz
{
class CImGuiService
    : public IService
{
public:
    CImGuiService();
    virtual ~CImGuiService();

    virtual bool Init() override;
    virtual void End() override;
    virtual bool IsOk() const override;

    virtual void PreUpdate() override;
    virtual void Update() override;
    virtual void PostUpdate() override;
};

} // maz

#endif // !_H_C_IMGUI_SERVICE_

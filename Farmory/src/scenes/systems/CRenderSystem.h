#ifndef _H_C_RENDER_SYSTEM_
#define _H_C_RENDER_SYSTEM_

#include <scenes/ISystem.h>


namespace maz
{

class CRenderSystem
    : public ISystem
{
public:
    CRenderSystem();
    virtual ~CRenderSystem();
    virtual void Update() override;
};

} // maz

#endif // !_H_C_RENDER_SYSTEM_s

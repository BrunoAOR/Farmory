#ifndef _H_C_RENDER_SYSTEM_
#define _H_C_RENDER_SYSTEM_

#include <scenes/ISystem.h>
#include <scenes/components/CTransform2DComponent.h>
#include <scenes/components/CSpriteComponent.h>


namespace maz
{

class CRenderSystem
    : public CSystemBase<CTransform2DComponent, CSpriteComponent>
{
public:
    CRenderSystem();
    virtual ~CRenderSystem();
    virtual void Update() override;
};

} // maz

#endif // !_H_C_RENDER_SYSTEM_s

#ifndef _H_C_TRANSFORM_HIERARCHY_SYSTEM_
#define _H_C_TRANSFORM_HIERARCHY_SYSTEM_

#include <scenes/ISystem.h>
#include <scenes/components/CTransform2DComponent.h>


namespace maz
{

class CTransformHierarchySystem
    : public CSystemBase<CTransform2DComponent>
{
public:
    CTransformHierarchySystem();
    virtual ~CTransformHierarchySystem();
    virtual void Update() override;
};

} // maz

#endif // !_H_C_TRANSFORM_HIERARCHY_SYSTEM_

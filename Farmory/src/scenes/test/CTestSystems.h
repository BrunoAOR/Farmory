#ifndef _H_C_TEST_SYSTEMS_
#define _H_C_TEST_SYSTEMS_

#include <scenes/ISystem.h>
#include <scenes/components/CTransform2DComponent.h>
#include <scenes/components/CSpriteComponent.h>
#include <scenes/test/CTestComponents.h>


namespace maz
{

class CSystemTransformA
    : public CSystemBase<CTransform2DComponent, CTestComponentA>
{
public:
    CSystemTransformA();
    virtual ~CSystemTransformA();
    virtual void Update() override;
};


class CSystemTransformB
    : public CSystemBase<CTransform2DComponent, CTestComponentB>
{
public:
    CSystemTransformB();
    virtual ~CSystemTransformB();
    virtual void Update() override;
};


class CMotionSystemTest
    : public CSystemBase<CTransform2DComponent, CSpriteComponent, CTestComponentA>
{
public:
    CMotionSystemTest();
    virtual ~CMotionSystemTest();
    virtual void Update() override;
};

} // maz

#endif // !_H_C_TEST_SYSTEMS_

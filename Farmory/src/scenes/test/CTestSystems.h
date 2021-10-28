#ifndef _H_C_TEST_SYSTEMS_
#define _H_C_TEST_SYSTEMS_

#include <scenes/ISystem.h>


namespace maz
{

class CSystemTransformA
    : public ISystem
{
public:
    CSystemTransformA();
    virtual ~CSystemTransformA();
    virtual void Update() override;
};

class CSystemTransformB
    : public ISystem
{
public:
    CSystemTransformB();
    virtual ~CSystemTransformB();
    virtual void Update() override;
};

class CMotionSystemTest
    : public ISystem
{
public:
    CMotionSystemTest();
    virtual ~CMotionSystemTest();
    virtual void Update() override;
};

} // maz

#endif // !_H_C_TEST_SYSTEMS_

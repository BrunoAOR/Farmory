#ifndef _H_C_TEST_COMPONENTS_
#define _H_C_TEST_COMPONENTS_

#include <maz/globals.h>
#include <scenes/IComponent.h>
#include <scenes/EComponentType.h>

namespace maz
{

class CGameObject;
class CTestComponentA
    : public IComponent
{
public:
    CTestComponentA(CReference<CGameObject>& aOwner) : IComponent(aOwner) { MAZ_LOGGER("CTestComponentA::CTestComponentA called"); }
    ~CTestComponentA() { MAZ_LOGGER("CTestComponentA::~CTestComponentA called"); }
    static constexpr EComponentType GetType() { return EComponentType::TestCompA; }
};

class CTestComponentB
    : public IComponent
{
public:
    CTestComponentB(CReference<CGameObject>& aOwner) : IComponent(aOwner) { MAZ_LOGGER("CTestComponentB::CTestComponentB called"); }
    ~CTestComponentB() { MAZ_LOGGER("CTestComponentB::~CTestComponentB called"); }
    static constexpr EComponentType GetType() { return EComponentType::TestCompB; }
};

} // maz

#endif // !_H_C_TEST_COMPONENTS_

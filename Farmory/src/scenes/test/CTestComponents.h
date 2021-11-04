#ifndef _H_C_TEST_COMPONENTS_
#define _H_C_TEST_COMPONENTS_

#include <maz/globals.h>
#include <scenes/IComponent.h>
#include <scenes/EComponentType.h>


namespace maz
{

class CGameObject;


class CTestComponentA
    : public CComponentBase<CTestComponentA>
{
public:
    CTestComponentA(CReference<CGameObject>& aOwner) : CComponentBase(aOwner) { MAZ_LOGGER("Called"); }
    ~CTestComponentA() { MAZ_LOGGER("Called"); }
    static constexpr EComponentType GetType() { return EComponentType::TestCompA; }
};


class CTestComponentB
    : public CComponentBase<CTestComponentB>
{
public:
    CTestComponentB(CReference<CGameObject>& aOwner) : CComponentBase(aOwner) { MAZ_LOGGER("Called"); }
    ~CTestComponentB() { MAZ_LOGGER("Called"); }
    static constexpr EComponentType GetType() { return EComponentType::TestCompB; }
};

} // maz

#endif // !_H_C_TEST_COMPONENTS_

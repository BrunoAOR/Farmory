#ifndef _H_E_COMPONENT_TYPE_
#define _H_E_COMPONENT_TYPE_

#include <maz/globals.h>


namespace maz
{

enum class EComponentType : uint8_t
{
    Transform,
    TestCompA,
    TestCompB,

    _Max = TestCompB
};

} // maz

#endif // !_H_E_COMPONENT_TYPE_

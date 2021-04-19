#ifndef _H_CONSTANTS_
#define _H_CONSTANTS_

#include <maz/types.h>

namespace maz
{

const uint16 kMaxGameObjects = 1024u;
const uint16 kInvalidGameObjectId = MAX_UINT16;
const uint16 kMaxComponentTypes = 256u;
const uint16 kMaxComponentsPerType = 1024u;
const uint16 kInvalidComponentId = MAX_UINT16;
static_assert(kMaxComponentsPerType - 1 < kInvalidComponentId);

} // maz

#endif // !_H_CONSTANTS_

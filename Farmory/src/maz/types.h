#ifndef _H_TYPES_
#define _H_TYPES_


namespace maz
{

// Integer types
using int8 = char;
using int16 = short;
using int32 = int;
using int64 = long long int;

const int8  MIN_INT8 = -127 - 1;
const int8  MAX_INT8 = 127;
const int16 MIN_INT16 = -32767 - 1;
const int16 MAX_INT16 = 32767;
const int32 MIN_INT32 = -2147483647 - 1;
const int32 MAX_INT32 = 2147483647;
const int64 MIN_INT64 = -9223372036854775807 - 1;
const int64 MAX_INT64 = 9223372036854775807;

using uint8 = unsigned char;
using uint16 = unsigned short;
using uint32 = unsigned int;
using uint64 = unsigned long long int;

const uint8  MAX_UINT8 = 255;
const uint16 MAX_UINT16 = 65535;
const uint32 MAX_UINT32 = 4294967295;
const uint64 MAX_UINT64 = 18446744073709551615;

// Float types
const float  MAX_FLOAT  = 3.402823466e+38F;
const double MAX_DOUBLE = 1.7976931348623158e+308;

const float  EPSILON_FLOAT  = 1.192092896e-07F;
const double EPSILON_DOUBLE = 2.2204460492503131e-016;

} // maz

#endif // !_H_TYPES_

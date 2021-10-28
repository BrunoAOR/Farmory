#ifndef _H_MATH_
#define _H_MATH_

#include <maz/types.h>
#include <external/glm/glm.hpp>
#include <external/glm/gtc/matrix_transform.hpp>
#include <external/glm/gtc/type_ptr.hpp>
#include <external/glm/gtx/quaternion.hpp>


namespace maz
{

using TVec2   = glm::vec2;
using TVec3   = glm::vec3;
using TMat4x4 = glm::mat4;
using TQuat   = glm::quat;

inline bool IsApproxEq(float aLhs, float aRhs, float aMaxDiff = EPSILON_FLOAT)
{
    return std::abs(aRhs - aLhs) <= aMaxDiff;
}

inline bool IsZero(const TVec2& aVec)
{
    return (IsApproxEq(aVec.x, 0) && IsApproxEq(aVec.y, 0));
}

inline bool IsZero(const TVec3& aVec)
{
    return (IsApproxEq(aVec.x, 0) && IsApproxEq(aVec.y, 0) && IsApproxEq(aVec.z, 0));
}

template<typename T>
inline T Min(T aLhs, T aRhs)
{
    return ((aLhs < aRhs) ? aLhs : aRhs);
}

template<typename T>
inline T Max(T aLhs, T aRhs)
{
    return ((aLhs > aRhs) ? aLhs : aRhs);
}

} // maz

#endif // !_H_MATH_


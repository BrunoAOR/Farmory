#ifndef _H_C_TRANSFORM_COMPONENT_
#define _H_C_TRANSFORM_COMPONENT_

#include <scenes/IComponent.h>
#include <scenes/EComponentType.h>

namespace maz
{

class CGameObject;
class CTransformComponent
    : public IComponent
{
public:
    CTransformComponent(CReference<CGameObject>& aOwner);
    ~CTransformComponent();

    static constexpr EComponentType GetType()
    {
        return EComponentType::Transform;
    }

    float a, b, c;

private:
};

} // maz

#endif // !_H_C_TRANSFORM_COMPONENT_

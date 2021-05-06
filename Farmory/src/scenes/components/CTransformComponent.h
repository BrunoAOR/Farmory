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

    const glm::mat4& GetModelMatrix() const { return mModelMatrix; }

public:
    glm::vec3 mTranslation;
    glm::quat mRotation;
    glm::vec3 mScale;

public:
    void RebuildModelMatrix();

private:
    glm::mat4 mModelMatrix;

};

} // maz

#endif // !_H_C_TRANSFORM_COMPONENT_

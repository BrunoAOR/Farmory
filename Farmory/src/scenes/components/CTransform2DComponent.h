#ifndef _H_C_TRANSFORM_2D_COMPONENT_
#define _H_C_TRANSFORM_2D_COMPONENT_

#include <scenes/IComponent.h>
#include <scenes/EComponentType.h>

namespace maz
{

class CGameObject;
class CTransform2DComponent
    : public IComponent
{
public:
    CTransform2DComponent(CReference<CGameObject>& aOwner);
    ~CTransform2DComponent();

    static constexpr EComponentType GetType()
    {
        return EComponentType::Transform;
    }

    const TVec2& GetTranslation() const;
    void SetTranslation(const TVec2& aPosition);

    // Note that CW rotation is negative and CCW rotation is positive
    float GetRotation() const;
    void SetRotation(float aRotation);

    const TVec2& GetScale() const;
    void SetScale(const TVec2& aScale);

    const TMat4x4& GetModelMatrix() const;


private:
    void rebuildModelMatrix();


private:
    TVec2 mTranslation;
    float mRotation;
    TVec2 mScale;

    TMat4x4 mModelMatrix;
    bool mModelMatrixDirty;
};

} // maz

#endif // !_H_C_TRANSFORM_2D_COMPONENT_

#ifndef _H_C_TRANSFORM_2D_COMPONENT_
#define _H_C_TRANSFORM_2D_COMPONENT_

#include <scenes/IComponent.h>
#include <scenes/EComponentType.h>

namespace maz
{

class CGameObject;

struct STransform2D
{
    TVec2 mTranslation;
    float mRotation;
    TVec2 mScale;

    STransform2D()
        : mTranslation(0.0f, 0.0f)
        , mRotation(0.0f)
        , mScale(1.0f, 1.0f)
    {}
};

class CTransform2DComponent
    : public CComponentBase<CTransform2DComponent>
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

    void SetParentTransform(CReference<CTransform2DComponent>& aParentTransform);

    const TMat4x4& GetModelMatrix() const;
    const CReference<CTransform2DComponent>& GetParentTransform() const;

    const STransform2D& GetLocalTransform() const;
    STransform2D& GetWorldTransform(); 
    bool IsLocalTransformDirty() const;

    void RebuildModelMatrix(); // Should only be accessed by CTransformHierarchySystem

private:
    bool addChildTransform(CReference<CTransform2DComponent>& aChildTransform);
    bool removeChildTransform(CReference<CTransform2DComponent>& aChildTransform);



private:
    STransform2D mLocalTransform;
    STransform2D mWorldTransform;

    bool mLocalTransformDirty;
    TMat4x4 mModelMatrix;

    CReference<CTransform2DComponent> mParent;
    CReference<CTransform2DComponent> mNextSibling;
    CReference<CTransform2DComponent> mFirstChild;
    uint16 mNumChildren;
};

} // maz

#endif // !_H_C_TRANSFORM_2D_COMPONENT_

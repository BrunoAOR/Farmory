#ifndef _H_C_TRANSFORM_2D_COMPONENT_
#define _H_C_TRANSFORM_2D_COMPONENT_

#include <scenes/IComponent.h>
#include <scenes/EComponentType.h>


namespace maz
{

class CGameObject;


struct STransform2D
{
    STransform2D()
        : mTranslation(0.0f, 0.0f)
        , mRotation(0.0f)
        , mScale(1.0f, 1.0f)
    {}

    const TVec2& GetTranslation() const { return mTranslation; }
    void SetTranslation(const TVec2& arTranslation) { mTranslation = arTranslation; }

    float GetRotation() const { return mRotation; }
    void SetRotation(const float aRotation)
    {
        mRotation = aRotation;
        while (mRotation >= 360.0f)
        {
            mRotation -= 360.0f;
        }
        while (mRotation < 0.0f)
        {
            mRotation += 360.0f;
        }
    }

    const TVec2& GetScale() const { return mScale; }
    void SetScale(const TVec2& arScale) { mScale = arScale; }

private:
    TVec2 mTranslation;
    float mRotation;
    TVec2 mScale;
};


class CTransform2DComponent
    : public CComponentBase<CTransform2DComponent>
{
public:
    CTransform2DComponent(CReference<CGameObject>& arOwner);
    ~CTransform2DComponent();

    static constexpr EComponentType GetType()
    {
        return EComponentType::Transform;
    }

    const TVec2& GetTranslation() const;
    void SetTranslation(const TVec2& arPosition);

    // Note that CW rotation is negative and CCW rotation is positive
    float GetRotation() const;
    void SetRotation(float aRotation);

    const TVec2& GetScale() const;
    void SetScale(const TVec2& arScale);

    const CReference<CTransform2DComponent>& GetParentTransform() const;
    void SetParentTransform(CReference<CTransform2DComponent>& arParentTransform);

    const CReference<CTransform2DComponent>& GetNextSiblingTransform() const;
    const CReference<CTransform2DComponent>& GetFirstChildTransform() const;

    const TMat4x4& GetModelMatrix() const;

    const STransform2D& GetLocalTransform() const;
    const STransform2D& GetWorldTransform() const;
    bool IsWorldTransformDirty() const;

    // The following public methods should only be accessed by CTransformHierarchySystem
    void SetWorldTransform(const STransform2D& arTransform);
    void RebuildModelMatrix();


private:
    bool addChildTransform(CReference<CTransform2DComponent>& arChildTransform);
    bool removeChildTransform(CReference<CTransform2DComponent>& arChildTransform);

private:
    STransform2D mLocalTransform;
    STransform2D mWorldTransform;

    bool mWorldTransformDirty;
    TMat4x4 mModelMatrix;

    CReference<CTransform2DComponent> mParent;
    CReference<CTransform2DComponent> mNextSibling;
    CReference<CTransform2DComponent> mFirstChild;
    uint16 mNumChildren;
};

} // maz

#endif // !_H_C_TRANSFORM_2D_COMPONENT_

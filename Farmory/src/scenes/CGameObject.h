#ifndef _H_C_GAME_OBJECT_
#define _H_C_GAME_OBJECT_

#include <maz/globals.h>
#include <maz/CFixedSizeString.h>
#include <maz/CReference.h>
#include <scenes/EComponentType.h>
#include <scenes/IComponent.h>
#include <array>
#include <scenes/CComponentsManager.h>
#include <scenes/SComponentsSignature.h>

namespace maz
{
class CGameObject final
{
private:
    friend class CGameObjectsManager;
    friend class CComponentsManager;
    
    void updateComponentId(EComponentType aComponentType, uint16 aComponentId);

public:
    CGameObject(uint16 aId, CComponentsManager& aComponentsManager, const CFixedString32& aName);
    ~CGameObject();

    uint16 GetId() const;
    const CFixedString32& GetName() const;

    template<typename COMPONENT_CLASS>
    CReference<COMPONENT_CLASS> AddComponent();
    template<typename COMPONENT_CLASS>
    bool RemoveComponent();
    bool RemoveComponent(EComponentType aComponentType);
    template<typename COMPONENT_CLASS>
    bool HasComponent() const;
    template<typename COMPONENT_CLASS>
    CReference<COMPONENT_CLASS> GetComponent();

    bool IsSignatureDirty() const;
    const SComponentsSignature& GetPreviousSignature() const;
    const SComponentsSignature& GetSignature() const;

private:
    const uint16_t mId;
    CComponentsManager& mComponentsManager;
    CReference<CGameObject> mThis;

    SComponentsSignature mPreviousSignature;
    SComponentsSignature mSignature;
    bool mIsSignatureDirty;

    CFixedString32 mName;

    std::array<uint16, MAZ_ENUM_COUNT(EComponentType)> mComponents;
};


template<typename COMPONENT_CLASS>
inline CReference<COMPONENT_CLASS> CGameObject::AddComponent()
{
    CReference<COMPONENT_CLASS> component;
    if (mComponents[EnumToNumber(COMPONENT_CLASS::GetType())] == kInvalidComponentId)
    {
        const uint16 newComponentId = mComponentsManager.AddComponent<COMPONENT_CLASS>(mThis);
        MAZ_ASSERT(newComponentId != kInvalidComponentId, "Failed to add component of desired type!");
        // Even though the component has been created and we store its ID, the GameObject signature is NOT modified until the ComponentsManager is updated.
        // This means that any systems that would now include this GameObject, will only do so in the next frame.
        mComponents[EnumToNumber(COMPONENT_CLASS::GetType())] = newComponentId;
        component = mComponentsManager.GetComponent<COMPONENT_CLASS>(newComponentId);
    }

    return component;
}


template<typename COMPONENT_CLASS>
inline bool CGameObject::RemoveComponent()
{
    bool lOk = true;
    const uint16 componentId = mComponents[EnumToNumber(COMPONENT_CLASS::GetType())];
    lOk = (componentId != kInvalidComponentId);
    lOk = lOk && mComponentsManager.RemoveComponent<COMPONENT_CLASS>(componentId);
    // Even though the component has been flagged for removal (if it was present), its ID is NOT cleared from the gameObject until the ComponentsManager is updated

    return lOk;
}


template<typename COMPONENT_CLASS>
inline bool CGameObject::HasComponent() const
{
    return (mComponents[EnumToNumber(COMPONENT_CLASS::GetType())] != kInvalidComponentId);
}


template<typename COMPONENT_CLASS>
inline CReference<COMPONENT_CLASS> CGameObject::GetComponent()
{
    CReference<COMPONENT_CLASS> component;
    const uint16 componentId = mComponents[EnumToNumber(COMPONENT_CLASS::GetType())];
    if (componentId != kInvalidComponentId)
    {
        component = mComponentsManager.GetComponent<COMPONENT_CLASS>(componentId);
    }

    return component;
}

} // maz

#endif // !_H_C_GAME_OBJECT_

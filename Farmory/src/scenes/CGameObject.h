#ifndef _H_C_GAME_OBJECT_
#define _H_C_GAME_OBJECT_

#include <maz/globals.h>
#include <maz/CFixedSizeString.h>
#include <maz/CReference.h>
#include <scenes/EComponentType.h>
#include <scenes/IComponent.h>
#include <array>
#include <scenes/ComponentsManager.h>

namespace maz
{
class CGameObject final
{
private:
    friend class CGameObjectsManager;
    friend class CComponentsManager;

    CGameObject(uint16 aId, CComponentsManager& aComponentsManager, CGameObject* aParent, const CFixedString32& aName);
    ~CGameObject();
    void updateComponentId(EComponentType aComponentType, uint16 aId);

public:
    template<typename COMPONENT_CLASS>
    CReference<COMPONENT_CLASS> AddComponent();
    template<typename COMPONENT_CLASS>
    bool RemoveComponent();
    template<typename COMPONENT_CLASS>
    bool HasComponent();
    template<typename COMPONENT_CLASS>
    CReference<COMPONENT_CLASS> GetComponent();

private:
    uint16_t mId;
    CComponentsManager& mComponentsManager;
    CReference<CGameObject> mThis;

    CGameObject* mParent;
    CGameObject* mNextSibling;
    CGameObject* mFirstChild;
    uint16 mNumChildren;


    CFixedString32 mName;

    std::array<uint16, MAZ_ENUM_COUNT(EComponentType)> mComponents;
};


template<typename COMPONENT_CLASS>
inline CReference<COMPONENT_CLASS> CGameObject::AddComponent()
{
    CReference<COMPONENT_CLASS> component;
    if (mComponents[EnumToNumber(COMPONENT_CLASS::GetType())] == kInvalidComponentIndex)
    {
        component = mComponentsManager.AddComponent<COMPONENT_CLASS>(mThis);
        MAZ_ASSERT(component, "[CGameObject]::AddComponent - Failed to add component of desired type!");
        // Even though the component has been created, its ID is NOT added to the gameObject until the ComponentsManager is updated
    }

    return component;
}


template<typename COMPONENT_CLASS>
inline bool CGameObject::RemoveComponent()
{
    bool lOk = true;
    const uint16 componentIndex = mComponents[EnumToNumber(COMPONENT_CLASS::GetType())];
    lOk = (componentIndex != kInvalidComponentIndex);
    lOk = lOk && mComponentsManager.RemoveComponent<COMPONENT_CLASS>(componentIndex);
    // Even though the component has been flagged for removal (if it was present), its ID is NOT cleared from the gameObject until the ComponentsManager is updated

    return lOk;
}


template<typename COMPONENT_CLASS>
inline bool CGameObject::HasComponent()
{
    return (mComponents[EnumToNumber(COMPONENT_CLASS::GetType())] != kInvalidComponentIndex);
}


template<typename COMPONENT_CLASS>
inline CReference<COMPONENT_CLASS> CGameObject::GetComponent()
{
    CReference<COMPONENT_CLASS> component;
    const uint16 componentIndex = mComponents[EnumToNumber(COMPONENT_CLASS::GetType())];
    if (componentIndex != kInvalidComponentIndex)
    {
        component = mComponentsManager.GetComponent<COMPONENT_CLASS>(componentIndex);
    }

    return component;
}

} // maz

#endif // !_H_C_GAME_OBJECT_

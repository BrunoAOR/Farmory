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
public:
    CGameObject(CComponentsManager& aComponentsManager, CGameObject* aParent, const CFixedString32& aName);
    ~CGameObject();

    template<typename COMPONENT_CLASS>
    CReference<COMPONENT_CLASS> AddComponent();
    template<typename COMPONENT_CLASS>
    bool RemoveComponent();
    template<typename COMPONENT_CLASS>
    bool HasComponent();
    template<typename COMPONENT_CLASS>
    CReference<COMPONENT_CLASS> GetComponent();

private:
    CGameObject* mParent;
    CGameObject* mNextSibling;
    CGameObject* mFirstChild;
    uint16 mNumChildren;

    CComponentsManager& mComponentsManager;

    CFixedString32 mName;

    std::array<uint16, MAZ_ENUM_COUNT(EComponentType)> mComponents;
};


template<typename COMPONENT_CLASS>
inline CReference<COMPONENT_CLASS> CGameObject::AddComponent()
{
    CReference<COMPONENT_CLASS> component;
    if (mComponents[EnumToNumber(COMPONENT_CLASS::GetType())] == kInvalidComponentIndex)
    {
        const uint16 componentIndex = mComponentsManager.AddComponent<COMPONENT_CLASS>(*this);
        MAZ_ASSERT(componentIndex != kInvalidComponentIndex, "[CGameObject]::AddComponent - Failed to add component of desired type!");
        mComponents[EnumToNumber(COMPONENT_CLASS::GetType())] = componentIndex;
        component = mComponentsManager.GetComponent<COMPONENT_CLASS>(componentIndex);
    }

    return component;
}


template<typename COMPONENT_CLASS>
inline bool CGameObject::RemoveComponent()
{
    bool lOk = true;
    const uint16 componentIndex = mComponents[EnumToNumber(COMPONENT_CLASS::GetType())];
    lOk = (componentIndex != kInvalidComponentIndex);
    if (lOk)
    {
        lOk = mComponentsManager.RemoveComponent<COMPONENT_CLASS>(componentIndex);
    }

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

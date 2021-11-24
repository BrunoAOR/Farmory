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

class CEntity final
{
private:
    friend class CEntitiesManager;
    friend class CComponentsManager;
    
    void updateComponentId(EComponentType aComponentType, uint16 aComponentId);

public:
    CEntity(uint16 aId, CComponentsManager& arComponentsManager, const CFixedString32& arName);
    ~CEntity();

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
    CComponentsManager& mrComponentsManager;
    CReference<CEntity> mThis;

    SComponentsSignature mPreviousSignature;
    SComponentsSignature mSignature;
    bool mIsSignatureDirty;

    CFixedString32 mName;

    std::array<uint16, EnumCount<EComponentType>()> mComponents;
};


template<typename COMPONENT_CLASS>
inline CReference<COMPONENT_CLASS> CEntity::AddComponent()
{
    CReference<COMPONENT_CLASS> lComponent;
    if (mComponents[EnumToNumber(COMPONENT_CLASS::GetType())] == kInvalidComponentId)
    {
        const uint16 lNewComponentId = mrComponentsManager.AddComponent<COMPONENT_CLASS>(mThis);
        MAZ_ASSERT(lNewComponentId != kInvalidComponentId, "Failed to add component of desired type!");
        // Even though the lComponent has been created and we store its ID, the Entity signature is NOT modified until the ComponentsManager is updated.
        // This means that any systems that would now include this Entity, will only do so in the next frame.
        mComponents[EnumToNumber(COMPONENT_CLASS::GetType())] = lNewComponentId;
        lComponent = mrComponentsManager.GetComponent<COMPONENT_CLASS>(lNewComponentId);
    }

    return lComponent;
}


template<typename COMPONENT_CLASS>
inline bool CEntity::RemoveComponent()
{
    bool lOk = true;
    const uint16 lComponentId = mComponents[EnumToNumber(COMPONENT_CLASS::GetType())];
    lOk = (lComponentId != kInvalidComponentId);
    lOk = lOk && mrComponentsManager.RemoveComponent<COMPONENT_CLASS>(lComponentId);
    // Even though the lComponent has been flagged for removal (if it was present), its ID is NOT cleared from the Entity until the ComponentsManager is updated

    return lOk;
}


template<typename COMPONENT_CLASS>
inline bool CEntity::HasComponent() const
{
    return (mComponents[EnumToNumber(COMPONENT_CLASS::GetType())] != kInvalidComponentId);
}


template<typename COMPONENT_CLASS>
inline CReference<COMPONENT_CLASS> CEntity::GetComponent()
{
    CReference<COMPONENT_CLASS> lComponent;
    const uint16 lComponentId = mComponents[EnumToNumber(COMPONENT_CLASS::GetType())];
    if (lComponentId != kInvalidComponentId)
    {
        lComponent = mrComponentsManager.GetComponent<COMPONENT_CLASS>(lComponentId);
    }

    return lComponent;
}

} // maz

#endif // !_H_C_GAME_OBJECT_

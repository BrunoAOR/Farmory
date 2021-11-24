#define MAZ_LOG_VERBOSE
#include "CEntity.h"


namespace maz
{

CEntity::CEntity(uint16 aId, CComponentsManager& arComponentsManager, const CFixedString32& arName)
    : mId(aId)
    , mrComponentsManager(arComponentsManager)
    , mIsSignatureDirty(false)
    , mName(arName)
{
    MAZ_LOGGER_VERBOSE("Called");
    for (uint16 i = 0, iCount = static_cast<uint16>(mComponents.max_size()); i < iCount; ++i)
    {
        mComponents[i] = kInvalidComponentId;
    }
}


CEntity::~CEntity()
{
    MAZ_LOGGER_VERBOSE("Called");
#ifdef MAZ_DEBUG
    // Verify no components are left in Entity
    for (uint16 i = 0, iCount = static_cast<uint16>(mComponents.max_size()); i < iCount; ++i)
    {
        MAZ_ASSERT(mComponents[i] == kInvalidComponentId, "Component of type %hu has not been removed from Entity with id %hu", i, GetId());
    }
#endif // MAZ_DEBUG
}


void CEntity::updateComponentId(EComponentType aComponentType, uint16 aComponentId)
{
    mComponents[EnumToNumber(aComponentType)] = aComponentId;
    if (aComponentId != kInvalidComponentId)
    {
        mSignature.AddComponent(aComponentType);
    }
    else
    {
        mSignature.RemoveComponent(aComponentType);
    }
    mIsSignatureDirty = true;
}


uint16 CEntity::GetId() const
{
    return mId;
}


const CFixedString32& CEntity::GetName() const
{
    return mName;
}


bool CEntity::RemoveComponent(EComponentType aComponentType)
{
    bool lOk = true;
    const uint16 lComponentId = mComponents[EnumToNumber(aComponentType)];
    lOk = (lComponentId != kInvalidComponentId);
    lOk = lOk && mrComponentsManager.RemoveComponent(aComponentType, lComponentId);
    // Even though the component has been flagged for removal (if it was present), its ID is NOT cleared from the entity until the ComponentsManager is updated

    return lOk;
}


bool CEntity::IsSignatureDirty() const
{
    return mIsSignatureDirty;
}


const SComponentsSignature& CEntity::GetPreviousSignature() const
{
    return mPreviousSignature;
}


const SComponentsSignature& CEntity::GetSignature() const
{
    return mSignature;
}

} // maz

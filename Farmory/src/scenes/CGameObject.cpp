#define MAZ_LOG_VERBOSE
#include "CGameObject.h"


namespace maz
{

CGameObject::CGameObject(uint16 aId, CComponentsManager& aComponentsManager, CGameObject* aParent, const CFixedString32& aName)
    : mId(aId)
    , mComponentsManager(aComponentsManager)
    , mIsSignatureDirty(false)
    , mParent(aParent)
    , mNextSibling(nullptr)
    , mFirstChild(nullptr)
    , mNumChildren(0)
    , mName(aName)
{
    MAZ_LOGGER_VERBOSE("Called");
    for (uint16 i = 0, iCount = static_cast<uint16>(mComponents.max_size()); i < iCount; ++i)
    {
        mComponents[i] = kInvalidComponentId;
    }
}


CGameObject::~CGameObject()
{
    MAZ_LOGGER_VERBOSE("Called");
#ifdef DEBUG
    // Verify no components are left in GameObject
    for (uint16 i = 0, iCount = static_cast<uint16>(mComponents.max_size()); i < iCount; ++i)
    {
        MAZ_ASSERT(mComponents[i] == kInvalidComponentId, "Component of type %hu has not been removed from GameObject with id %hu", i, GetId());
    }
#endif // DEBUG
}


void CGameObject::updateComponentId(EComponentType aComponentType, uint16 aId)
{
    mComponents[EnumToNumber(aComponentType)] = aId;
    if (aId != kInvalidComponentId)
    {
        mSignature.AddComponent(aComponentType);
    }
    else
    {
        mSignature.RemoveComponent(aComponentType);
    }
    mIsSignatureDirty = true;
}


uint16 CGameObject::GetId() const
{
    return mId;
}


const CFixedString32 CGameObject::GetName() const
{
    return mName;
}


bool CGameObject::RemoveComponent(EComponentType aComponentType)
{
    bool lOk = true;
    const uint16 componentId = mComponents[EnumToNumber(aComponentType)];
    lOk = (componentId != kInvalidComponentId);
    lOk = lOk && mComponentsManager.RemoveComponent(aComponentType, componentId);
    // Even though the component has been flagged for removal (if it was present), its ID is NOT cleared from the gameObject until the ComponentsManager is updated

    return lOk;
}


bool CGameObject::IsSignatureDirty() const
{
    return mIsSignatureDirty;
}


const SComponentsSignature& CGameObject::GetPreviousSignature() const
{
    return mPreviousSignature;
}


const SComponentsSignature& CGameObject::GetSignature() const
{
    return mSignature;
}


} // maz

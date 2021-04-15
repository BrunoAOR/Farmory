#include "CGameObject.h"


namespace maz
{

CGameObject::CGameObject(uint16 aId, CComponentsManager& aComponentsManager, CGameObject* aParent, const CFixedString32& aName)
    : mId(aId)
    , mComponentsManager(aComponentsManager)
    , mParent(aParent)
    , mNextSibling(nullptr)
    , mFirstChild(nullptr)
    , mNumChildren(0)
    , mName(aName)
{
    for (size_t i = 0, iCount = mComponents.max_size(); i < iCount; ++i)
    {
        mComponents[i] = kInvalidComponentIndex;
    }
}


CGameObject::~CGameObject()
{}



} // maz

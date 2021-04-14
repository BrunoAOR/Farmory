#include "CGameObject.h"


namespace maz
{

CGameObject::CGameObject(CComponentsManager& aComponentsManager, CGameObject* aParent, const CFixedString32& aName)
    : mParent(aParent)
    , mNextSibling(nullptr)
    , mFirstChild(nullptr)
    , mNumChildren(0)
    , mComponentsManager(aComponentsManager)
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

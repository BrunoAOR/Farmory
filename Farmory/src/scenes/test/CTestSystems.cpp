#include "CTestSystems.h"


namespace maz
{



CSystemTransformA::CSystemTransformA()
{
    AddComponentToSignature(EComponentType::Transform);
    AddComponentToSignature(EComponentType::TestCompA);
}

CSystemTransformA::~CSystemTransformA()
{

}

void CSystemTransformA::Update()
{
    MAZ_LOGGER("CSystemTransformA::Update - START. Has %u GameObjects", mGameObjects.size());
    for (CReference<CGameObject>& gameObject : mGameObjects)
    {
        MAZ_LOGGER("CSystemTransformA::Update - Sees GameOjbect named '%s'", gameObject->GetName().c_str());
    }
    MAZ_LOGGER("CSystemTransformA::Update - END");
}

CSystemTransformB::CSystemTransformB()
{
    AddComponentToSignature(EComponentType::Transform);
    AddComponentToSignature(EComponentType::TestCompB);
}

CSystemTransformB::~CSystemTransformB()
{

}

void CSystemTransformB::Update()
{
    MAZ_LOGGER("CSystemTransformB::Update - START. Has %u GameObjects", mGameObjects.size());
    for (CReference<CGameObject>& gameObject : mGameObjects)
    {
        MAZ_LOGGER("CSystemTransformB::Update - Sees GameOjbect named '%s'", gameObject->GetName().c_str());
    }
    MAZ_LOGGER("CSystemTransformB::Update - END");
}

} // maz

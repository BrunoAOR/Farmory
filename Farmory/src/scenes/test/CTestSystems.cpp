#include "CTestSystems.h"
#include <scenes/components/CTransform2DComponent.h>
#include <services/CServicesManager.h>
#include <services/CInputService.h>
#include <services/CTimeService.h>
#include <GLFW/glfw3.h>
#include <external/glm/gtx/quaternion.hpp>


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
    MAZ_LOGGER("START. Has %u GameObjects", mGameObjects.size());
    for (CReference<CGameObject>& lrGameObject : mGameObjects)
    {
        if (lrGameObject)
        {
            MAZ_LOGGER("Sees GameOjbect named '%s'", lrGameObject->GetName().c_str());
        }
    }
    MAZ_LOGGER("END");
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
    MAZ_LOGGER("START. Has %u GameObjects", mGameObjects.size());
    for (CReference<CGameObject>& lrGameObject : mGameObjects)
    {
        if (lrGameObject)
        {
            MAZ_LOGGER("Sees GameOjbect named '%s'", lrGameObject->GetName().c_str());
        }
    }
    MAZ_LOGGER("END");
}


CMotionSystemTest::CMotionSystemTest()
{
    AddComponentToSignature(EComponentType::Transform);
    AddComponentToSignature(EComponentType::Sprite);
    AddComponentToSignature(EComponentType::TestCompA);
}


CMotionSystemTest::~CMotionSystemTest()
{
}


void CMotionSystemTest::Update()
{
    const CInputService* lpInputService = Services::GetManager()->GetService< CInputService >();
    const CTimeService* lpTimeService = Services::GetManager()->GetService< CTimeService >();
    bool lChangesMade = false;
    const float lDt = lpTimeService->GetDeltaTime();
    const float lSpeed = 0.1f;
    glm::vec2 lMotion(0.0f, 0.0f);
    if (lpInputService->IsKeyDown(GLFW_KEY_I))
    {
        lMotion.y += 1;
        lChangesMade = true;
    }
    if (lpInputService->IsKeyDown(GLFW_KEY_K))
    {
        lMotion.y -= 1;
        lChangesMade = true;
    }
    if (lpInputService->IsKeyDown(GLFW_KEY_J))
    {
        lMotion.x -= 1;
        lChangesMade = true;
    }
    if (lpInputService->IsKeyDown(GLFW_KEY_L))
    {
        lMotion.x += 1;
        lChangesMade = true;
    }
    lMotion *= lSpeed * lDt;

    const float lRotSpeed = 15.0f;
    float lZRotation = 0.0f;
    if (lpInputService->IsKeyDown(GLFW_KEY_U))
    {
        lZRotation += 1;
        lChangesMade = true;
    }
    if (lpInputService->IsKeyDown(GLFW_KEY_O))
    {
        lZRotation -= 1;
        lChangesMade = true;
    }
    lZRotation *= lRotSpeed * lDt;

    const float lScaleSpeed = 0.5f;
    TVec2 lScale(0.0f, 0.0f);
    if (lpInputService->IsKeyDown(GLFW_KEY_KP_4))
    {
        lScale.x -= 1;
        lChangesMade = true;
    }
    if (lpInputService->IsKeyDown(GLFW_KEY_KP_6))
    {
        lScale.x += 1;
        lChangesMade = true;
    }
    if (lpInputService->IsKeyDown(GLFW_KEY_KP_8))
    {
        lScale.y += 1;
        lChangesMade = true;
    }
    if (lpInputService->IsKeyDown(GLFW_KEY_KP_2))
    {
        lScale.y -= 1;
        lChangesMade = true;
    }
    lScale *= lScaleSpeed * lDt;

    if (lChangesMade)
    {
        for (CReference<CGameObject>& lrGameObject : mGameObjects)
        {
            if (lrGameObject)
            {
                CReference<CTransform2DComponent> lTransform = lrGameObject->GetComponent<CTransform2DComponent>();
                lTransform->SetTranslation(lTransform->GetTranslation() + lMotion);
                lTransform->SetRotation(lTransform->GetRotation() + lZRotation);
                lTransform->SetScale(lTransform->GetScale() + lScale);
            }
        }
    }
}

} // maz

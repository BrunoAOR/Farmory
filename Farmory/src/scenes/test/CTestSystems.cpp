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
    MAZ_LOGGER("CSystemTransformA::Update - START. Has %u GameObjects", mGameObjects.size());
    for (CReference<CGameObject>& gameObject : mGameObjects)
    {
        if (gameObject)
        {
            MAZ_LOGGER("CSystemTransformA::Update - Sees GameOjbect named '%s'", gameObject->GetName().c_str());
        }
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
        if (gameObject)
        {
            MAZ_LOGGER("CSystemTransformB::Update - Sees GameOjbect named '%s'", gameObject->GetName().c_str());
        }
    }
    MAZ_LOGGER("CSystemTransformB::Update - END");
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
    CInputService* lInputService = Services::GetManager()->GetService< CInputService >();
    CTimeService* lTimeService = Services::GetManager()->GetService< CTimeService >();
    bool lChangesMade = false;
    const float lDt = lTimeService->GetDeltaTime();
    const float lSpeed = 0.1f;
    glm::vec2 lMotion(0.0f, 0.0f);
    if (lInputService->IsKeyDown(GLFW_KEY_I))
    {
        lMotion.y += 1;
        lChangesMade = true;
    }
    if (lInputService->IsKeyDown(GLFW_KEY_K))
    {
        lMotion.y -= 1;
        lChangesMade = true;
    }
    if (lInputService->IsKeyDown(GLFW_KEY_J))
    {
        lMotion.x -= 1;
        lChangesMade = true;
    }
    if (lInputService->IsKeyDown(GLFW_KEY_L))
    {
        lMotion.x += 1;
        lChangesMade = true;
    }
    lMotion *= lSpeed * lDt;

    const float lRotSpeed = 15.0f;
    float zRotation = 0.0f;
    if (lInputService->IsKeyDown(GLFW_KEY_U))
    {
        zRotation += 1;
        lChangesMade = true;
    }
    if (lInputService->IsKeyDown(GLFW_KEY_O))
    {
        zRotation -= 1;
        lChangesMade = true;
    }
    zRotation *= lRotSpeed * lDt;

    const float lScaleSpeed = 0.5f;
    TVec2 lScale(0.0f, 0.0f);
    if (lInputService->IsKeyDown(GLFW_KEY_KP_4))
    {
        lScale.x -= 1;
        lChangesMade = true;
    }
    if (lInputService->IsKeyDown(GLFW_KEY_KP_6))
    {
        lScale.x += 1;
        lChangesMade = true;
    }
    if (lInputService->IsKeyDown(GLFW_KEY_KP_8))
    {
        lScale.y += 1;
        lChangesMade = true;
    }
    if (lInputService->IsKeyDown(GLFW_KEY_KP_2))
    {
        lScale.y -= 1;
        lChangesMade = true;
    }
    lScale *= lScaleSpeed * lDt;

    if (lChangesMade)
    {
        for (CReference<CGameObject>& gameObject : mGameObjects)
        {
            if (gameObject)
            {
                CReference<CTransform2DComponent> transform = gameObject->GetComponent<CTransform2DComponent>();
                transform->SetTranslation(transform->GetTranslation() + lMotion);
                transform->SetRotation(transform->GetRotation() + zRotation);
                transform->SetScale(transform->GetScale() + lScale);
            }
        }
    }
}

} // maz

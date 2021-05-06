#include "CRenderSystem.h"
#include <scenes/components/CTransformComponent.h>
#include <scenes/components/CSpriteComponent.h>
#include <render/CCamera.h>
#include <services/CServicesManager.h>
#include <test/CTestService.h>

namespace maz
{



CRenderSystem::CRenderSystem()
{
    AddComponentToSignature(EComponentType::Transform);
    AddComponentToSignature(EComponentType::Sprite);
}


CRenderSystem::~CRenderSystem()
{}


void CRenderSystem::Update()
{
    //TODO: Render system shouldn't actually be doing the render calls, it should only be gathering the relevant render information into a struct that can be passed to the RenderService.
    // Later, the RenderService can sort all the elements to render and do the actual render calls.

    const CCamera& camera = Services::GetManager()->GetService<CTestService>()->GetCamera();
    for (CReference<CGameObject>& gameObject : mGameObjects)
    {
        if (gameObject)
        {
            CReference<CTransformComponent> transformComponent = gameObject->GetComponent<CTransformComponent>();
            CReference<CSpriteComponent> spriteComponent = gameObject->GetComponent<CSpriteComponent>();

            spriteComponent->mShader.Use();
            spriteComponent->mShader.SetUniformMat4("aViewMatrix", camera.GetViewMatrixPtr());
            spriteComponent->mShader.SetUniformMat4("aProjectionMatrix", camera.GetProjMatrixPtr());
            spriteComponent->mShader.SetUniformMat4("aModelMatrix", glm::value_ptr(transformComponent->GetModelMatrix()));
            spriteComponent->mTexture.BindTexture(0);

            spriteComponent->mMesh.Draw();

            spriteComponent->mTexture.UnbindTexture();
            spriteComponent->mShader.StopUse();

        }
    }
}

} // maz

#include "CRenderSystem.h"
#include <scenes/components/CTransform2DComponent.h>
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

    const CCamera& lrCamera = Services::GetManager()->GetService<CTestService>()->GetCamera();
    for (CReference<CGameObject>& lrGameObject : mGameObjects)
    {
        if (lrGameObject)
        {
            CReference<CTransform2DComponent> lTransformComponent = lrGameObject->GetComponent<CTransform2DComponent>();
            CReference<CSpriteComponent> lSpriteComponent = lrGameObject->GetComponent<CSpriteComponent>();

            lSpriteComponent->mShader.Use();
            lSpriteComponent->mShader.SetUniformMat4("aViewMatrix", lrCamera.GetViewMatrixPtr());
            lSpriteComponent->mShader.SetUniformMat4("aProjectionMatrix", lrCamera.GetProjMatrixPtr());
            lSpriteComponent->mShader.SetUniformMat4("aModelMatrix", glm::value_ptr(lTransformComponent->GetModelMatrix()));
            lSpriteComponent->mTexture.BindTexture(0);

            lSpriteComponent->mMesh.Draw();

            lSpriteComponent->mTexture.UnbindTexture();
            lSpriteComponent->mShader.StopUse();
        }
    }
}

} // maz

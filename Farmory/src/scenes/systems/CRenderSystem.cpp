#include "CRenderSystem.h"
#include <render/CCamera.h>
#include <services/CServicesManager.h>
#include <test/CTestService.h>


namespace maz
{

CRenderSystem::CRenderSystem()
{}


CRenderSystem::~CRenderSystem()
{}


void CRenderSystem::Update()
{
    //TODO: Render system shouldn't actually be doing the render calls, it should only be gathering the relevant render information into a struct that can be passed to the RenderService.
    // Later, the RenderService can sort all the elements to render and do the actual render calls.

    const CCamera& lrCamera = Services::GetManager()->GetService<CTestService>()->GetCamera();
    for (CReference<CEntity>& lrEntity : mEntities)
    {
        if (lrEntity)
        {
            CReference<CTransform2DComponent> lTransformComponent = lrEntity->GetComponent<CTransform2DComponent>();
            CReference<CSpriteComponent> lSpriteComponent = lrEntity->GetComponent<CSpriteComponent>();

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

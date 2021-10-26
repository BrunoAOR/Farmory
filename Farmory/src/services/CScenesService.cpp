#include <maz/globals.h>
#include "CScenesService.h"

#include <scenes/CGameObject.h>
#include <scenes/test/CTestSystems.h>
#include <scenes/components/CTransform2DComponent.h>
#include <scenes/components/CSpriteComponent.h>
#include <scenes/systems/CRenderSystem.h>
#include <scenes/test/CTestComponents.h>


namespace maz
{

CScenesService::CScenesService()
    : mComponentsManager()
    , mGameObjectsManager(mComponentsManager)
    , mSystemsManager(mGameObjectsManager)
{ ; }


CScenesService::~CScenesService() { ; }


void test()
{
    CComponentsManager* componentsManager = MAZ_NEW(CComponentsManager);
    CGameObjectsManager* gameObjectsManager = MAZ_NEW(CGameObjectsManager, *componentsManager);
    componentsManager->RegisterComponent<CTransform2DComponent>();
    CReference<CGameObject> gameObject = gameObjectsManager->CreateGameObject("Timmy");
    gameObjectsManager->RefreshGameObjects();
    CReference<CTransform2DComponent> transform1 = gameObject->AddComponent<CTransform2DComponent>();
    bool hasIt = gameObject->HasComponent<CTransform2DComponent>();
    CReference<CTransform2DComponent> transform2 = gameObject->GetComponent<CTransform2DComponent>();
    bool isSame = (transform1 == transform2);
    bool isValid1 = (bool)transform1;
    bool isValid2 = (bool)transform2;
    componentsManager->RefreshComponents();
    hasIt = gameObject->HasComponent<CTransform2DComponent>();
    transform2 = gameObject->GetComponent<CTransform2DComponent>();
    isSame = (transform1 == transform2);
    transform2 = CReference<CTransform2DComponent>();
    isValid1 = (bool)transform1;
    isValid2 = (bool)transform2;
    gameObject->RemoveComponent<CTransform2DComponent>();
    isValid1 = (bool)transform1;
    isValid2 = (bool)transform2;
    componentsManager->RefreshComponents();
    isValid1 = (bool)transform1;
    isValid2 = (bool)transform2;

    gameObjectsManager->RequestDestroyGameObject(gameObject->GetId());
    gameObjectsManager->RefreshGameObjects();

    MAZ_UNUSED_VAR(hasIt);
    MAZ_UNUSED_VAR(isSame);
    MAZ_UNUSED_VAR(isValid1);
    MAZ_UNUSED_VAR(isValid2);

    MAZ_ASSERT(!transform1, "Transform should be invalid at this point");
    MAZ_ASSERT(!transform2, "Transform should be invalid at this point");

    gameObjectsManager->Shutdown();
    componentsManager->Shutdown();
    MAZ_DELETE(gameObjectsManager);
    MAZ_DELETE(componentsManager);
}


bool CScenesService::Init()
{
    bool lOk = true;
    test();

    return lOk;
}


void CScenesService::End()
{
    mSystemsManager.Shutdown();
    mComponentsManager.Shutdown();
    mGameObjectsManager.Shutdown();
}


bool CScenesService::IsOk() const
{
    return true;
}


void CScenesService::PreUpdate()
{}


void CScenesService::Update()
{
    static bool hasLoadedScene = false;
    if (!hasLoadedScene)
    {
        LoadTestScenes();
        hasLoadedScene = true;
    }

    // Update Scene loading
    mGameObjectsManager.RefreshGameObjects();
    mComponentsManager.RefreshComponents();
    mSystemsManager.RefreshSystems();
}


void CScenesService::PostUpdate()
{}

bool CScenesService::LoadTestScenes()
{
    bool lOk = true;

    bool useBaseScene = false;
    if (useBaseScene)
    {
        //Register components and systems
        lOk = lOk && mComponentsManager.RegisterComponent<CTransform2DComponent>();
        lOk = lOk && mComponentsManager.RegisterComponent<CTestComponentA>();
        lOk = lOk && mComponentsManager.RegisterComponent<CTestComponentB>();
        lOk = lOk && mSystemsManager.RegisterSystem<CSystemTransformA>();
        lOk = lOk && mSystemsManager.RegisterSystem<CSystemTransformB>();
        MAZ_ASSERT(lOk, "WTF?!");

        // Load test scene
        CReference<CGameObject> gameObjectTA = mGameObjectsManager.CreateGameObject("GO-TA");
        gameObjectTA->AddComponent<CTransform2DComponent>();
        gameObjectTA->AddComponent<CTestComponentA>();

        CReference<CGameObject> gameObjectTB = mGameObjectsManager.CreateGameObject("GO-TB");
        gameObjectTB->AddComponent<CTransform2DComponent>();
        gameObjectTB->AddComponent<CTestComponentB>();

        CReference<CGameObject> gameObjectTAB = mGameObjectsManager.CreateGameObject("GO-TAB");
        gameObjectTAB->AddComponent<CTransform2DComponent>();
        gameObjectTAB->AddComponent<CTestComponentA>();
        gameObjectTAB->AddComponent<CTestComponentB>();
    }

    bool useComplexScene = true;
    if (useComplexScene)
    {
        //Register components and systems
        lOk = lOk && mComponentsManager.RegisterComponent<CTransform2DComponent>();
        lOk = lOk && mComponentsManager.RegisterComponent<CSpriteComponent>();
        lOk = lOk && mComponentsManager.RegisterComponent<CTestComponentA>();
        lOk = lOk && mSystemsManager.RegisterSystem<CRenderSystem>();
        lOk = lOk && mSystemsManager.RegisterSystem<CMotionSystemTest>();

        // Load test scene
        {
            CReference<CGameObject> gameObject = mGameObjectsManager.CreateGameObject("Sprite +1");
            gameObject->AddComponent<CTransform2DComponent>();
            gameObject->AddComponent<CSpriteComponent>();
            CReference<CTransform2DComponent> transform = gameObject->GetComponent<CTransform2DComponent>();
            transform->SetTranslation(TVec2(1.0f, 1.0f));
        }
        {
            CReference<CGameObject> gameObject = mGameObjectsManager.CreateGameObject("Sprite -1");
            gameObject->AddComponent<CTransform2DComponent>();
            gameObject->AddComponent<CSpriteComponent>();
            CReference<CTransform2DComponent> transform = gameObject->GetComponent<CTransform2DComponent>();
            transform->SetTranslation(TVec2(-1.0f, -1.0f));
        }
        {
            CReference<CGameObject> gameObject = mGameObjectsManager.CreateGameObject("Sprite 0");
            gameObject->AddComponent<CTransform2DComponent>();
            gameObject->AddComponent<CSpriteComponent>();
            gameObject->AddComponent<CTestComponentA>();
            CReference<CTransform2DComponent> transform = gameObject->GetComponent<CTransform2DComponent>();
            transform->SetTranslation(TVec2(0.0f, 0.0f));
            transform->SetScale(TVec2(0.5f, 2.0f));
        }
    }

    return lOk;
}

} // maz

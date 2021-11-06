#include <maz/globals.h>
#include "CScenesService.h"

#include <scenes/CGameObject.h>
#include <scenes/test/CTestSystems.h>
#include <scenes/components/CTransform2DComponent.h>
#include <scenes/components/CSpriteComponent.h>
#include <scenes/systems/CRenderSystem.h>
#include <scenes/systems/CTransformHierarchySystem.h>
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
    CComponentsManager* lpComponentsManager = MAZ_NEW(CComponentsManager);
    CGameObjectsManager* lpGameObjectsManager = MAZ_NEW(CGameObjectsManager, *lpComponentsManager);
    lpComponentsManager->RegisterComponent<CTransform2DComponent>();
    CReference<CGameObject> lGameObject = lpGameObjectsManager->CreateGameObject("Timmy");
    lpGameObjectsManager->RefreshGameObjects();
    CReference<CTransform2DComponent> lTransform1 = lGameObject->AddComponent<CTransform2DComponent>();
    bool lHasIt = lGameObject->HasComponent<CTransform2DComponent>();
    CReference<CTransform2DComponent> lTransform2 = lGameObject->GetComponent<CTransform2DComponent>();
    bool lIsSame = (lTransform1 == lTransform2);
    bool lIsValid1 = (bool)lTransform1;
    bool lIsValid2 = (bool)lTransform2;
    lpComponentsManager->RefreshComponents();
    lHasIt = lGameObject->HasComponent<CTransform2DComponent>();
    lTransform2 = lGameObject->GetComponent<CTransform2DComponent>();
    lIsSame = (lTransform1 == lTransform2);
    lTransform2 = CReference<CTransform2DComponent>();
    lIsValid1 = (bool)lTransform1;
    lIsValid2 = (bool)lTransform2;
    lGameObject->RemoveComponent<CTransform2DComponent>();
    lIsValid1 = (bool)lTransform1;
    lIsValid2 = (bool)lTransform2;
    lpComponentsManager->RefreshComponents();
    lIsValid1 = (bool)lTransform1;
    lIsValid2 = (bool)lTransform2;

    lpGameObjectsManager->RequestDestroyGameObject(lGameObject->GetId());
    lpGameObjectsManager->RefreshGameObjects();

    MAZ_UNUSED_VAR(lHasIt);
    MAZ_UNUSED_VAR(lIsSame);
    MAZ_UNUSED_VAR(lIsValid1);
    MAZ_UNUSED_VAR(lIsValid2);

    MAZ_ASSERT(!lTransform1, "Transform should be invalid at this point");
    MAZ_ASSERT(!lTransform2, "Transform should be invalid at this point");

    lpGameObjectsManager->Shutdown();
    lpComponentsManager->Shutdown();
    MAZ_DELETE(lpGameObjectsManager);
    MAZ_DELETE(lpComponentsManager);
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


void CScenesService::Update()
{
    static bool sHasLoadedScene = false;
    if (!sHasLoadedScene)
    {
        LoadTestScenes();
        sHasLoadedScene = true;
    }

    // Update Scene loading
    mGameObjectsManager.RefreshGameObjects();
    mComponentsManager.RefreshComponents();
    mSystemsManager.RefreshSystems();
}


bool CScenesService::LoadTestScenes()
{
    bool lOk = true;

    bool lUseBaseScene = false;
    if (lUseBaseScene)
    {
        //Register components and systems
        lOk = lOk && mComponentsManager.RegisterComponent<CTransform2DComponent>();
        lOk = lOk && mComponentsManager.RegisterComponent<CTestComponentA>();
        lOk = lOk && mComponentsManager.RegisterComponent<CTestComponentB>();
        lOk = lOk && mSystemsManager.RegisterSystem<CSystemTransformA>();
        lOk = lOk && mSystemsManager.RegisterSystem<CSystemTransformB>();
        MAZ_ASSERT(lOk, "WTF?!");

        // Load test scene
        CReference<CGameObject> lGameObjectTA = mGameObjectsManager.CreateGameObject("GO-TA");
        lGameObjectTA->AddComponent<CTransform2DComponent>();
        lGameObjectTA->AddComponent<CTestComponentA>();

        CReference<CGameObject> lGameObjectTB = mGameObjectsManager.CreateGameObject("GO-TB");
        lGameObjectTB->AddComponent<CTransform2DComponent>();
        lGameObjectTB->AddComponent<CTestComponentB>();

        CReference<CGameObject> lGameObjectTAB = mGameObjectsManager.CreateGameObject("GO-TAB");
        lGameObjectTAB->AddComponent<CTransform2DComponent>();
        lGameObjectTAB->AddComponent<CTestComponentA>();
        lGameObjectTAB->AddComponent<CTestComponentB>();
    }

    bool lUuseComplexScene = true;
    if (lUuseComplexScene)
    {
        //Register components and systems
        lOk = lOk && mComponentsManager.RegisterComponent<CTransform2DComponent>();
        lOk = lOk && mComponentsManager.RegisterComponent<CSpriteComponent>();
        lOk = lOk && mComponentsManager.RegisterComponent<CTestComponentA>();
        lOk = lOk && mSystemsManager.RegisterSystem<CRenderSystem>();
        lOk = lOk && mSystemsManager.RegisterSystem<CMotionSystemTest>();
        lOk = lOk && mSystemsManager.RegisterSystem<CTransformHierarchySystem>();

        // Load test scene
        {
            CReference<CGameObject> lGameObject = mGameObjectsManager.CreateGameObject("Sprite +1");
            lGameObject->AddComponent<CTransform2DComponent>();
            lGameObject->AddComponent<CSpriteComponent>();
            CReference<CTransform2DComponent> lTransform = lGameObject->GetComponent<CTransform2DComponent>();
            lTransform->SetTranslation(TVec2(1.0f, 1.0f));
        }
        CReference<CTransform2DComponent> lParentTransform;
        {
            CReference<CGameObject> lGameObject = mGameObjectsManager.CreateGameObject("Sprite 0");
            lGameObject->AddComponent<CTransform2DComponent>();
            lGameObject->AddComponent<CSpriteComponent>();
            lGameObject->AddComponent<CTestComponentA>();
            CReference<CTransform2DComponent> lTransform = lGameObject->GetComponent<CTransform2DComponent>();
            lTransform->SetTranslation(TVec2(0.0f, 0.0f));
            lTransform->SetScale(TVec2(0.5f, 2.0f));
            lParentTransform = lTransform;
        }
        {
            CReference<CGameObject> lGameObject = mGameObjectsManager.CreateGameObject("Sprite -1");
            lGameObject->AddComponent<CTransform2DComponent>();
            lGameObject->AddComponent<CSpriteComponent>();
            CReference<CTransform2DComponent> lTransform = lGameObject->GetComponent<CTransform2DComponent>();
            lTransform->SetTranslation(TVec2(-1.0f, -1.0f));
            lTransform->SetParentTransform(lParentTransform);
        }
    }

    return lOk;
}

} // maz

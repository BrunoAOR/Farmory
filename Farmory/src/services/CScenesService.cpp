#include <maz/globals.h>
#include "CScenesService.h"

#include <scenes/CEntity.h>
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
    , mEntitiesManager(mComponentsManager)
    , mSystemsManager(mEntitiesManager)
{ ; }


CScenesService::~CScenesService() { ; }


void test()
{
    CComponentsManager* lpComponentsManager = MAZ_NEW(CComponentsManager);
    CEntitiesManager* lpEntitiesManager = MAZ_NEW(CEntitiesManager, *lpComponentsManager);
    lpComponentsManager->RegisterComponent<CTransform2DComponent>();
    CReference<CEntity> lEntity = lpEntitiesManager->CreateEntity("Timmy");
    lpEntitiesManager->RefreshEntities();
    CReference<CTransform2DComponent> lTransform1 = lEntity->AddComponent<CTransform2DComponent>();
    bool lHasIt = lEntity->HasComponent<CTransform2DComponent>();
    CReference<CTransform2DComponent> lTransform2 = lEntity->GetComponent<CTransform2DComponent>();
    bool lIsSame = (lTransform1 == lTransform2);
    bool lIsValid1 = (bool)lTransform1;
    bool lIsValid2 = (bool)lTransform2;
    lpComponentsManager->RefreshComponents();
    lHasIt = lEntity->HasComponent<CTransform2DComponent>();
    lTransform2 = lEntity->GetComponent<CTransform2DComponent>();
    lIsSame = (lTransform1 == lTransform2);
    lTransform2 = CReference<CTransform2DComponent>();
    lIsValid1 = (bool)lTransform1;
    lIsValid2 = (bool)lTransform2;
    lEntity->RemoveComponent<CTransform2DComponent>();
    lIsValid1 = (bool)lTransform1;
    lIsValid2 = (bool)lTransform2;
    lpComponentsManager->RefreshComponents();
    lIsValid1 = (bool)lTransform1;
    lIsValid2 = (bool)lTransform2;

    lpEntitiesManager->RequestDestroyEntity(lEntity->GetId());
    lpEntitiesManager->RefreshEntities();

    MAZ_UNUSED_VAR(lHasIt);
    MAZ_UNUSED_VAR(lIsSame);
    MAZ_UNUSED_VAR(lIsValid1);
    MAZ_UNUSED_VAR(lIsValid2);

    MAZ_ASSERT(!lTransform1, "Transform should be invalid at this point");
    MAZ_ASSERT(!lTransform2, "Transform should be invalid at this point");

    lpEntitiesManager->Shutdown();
    lpComponentsManager->Shutdown();
    MAZ_DELETE(lpEntitiesManager);
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
    mEntitiesManager.Shutdown();
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
    mEntitiesManager.RefreshEntities();
    mComponentsManager.RefreshComponents();
    mSystemsManager.RefreshSystems();
}


bool CScenesService::LoadTestScenes()
{
    bool lOk = true;

    const bool lUseBaseScene = false;
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
        CReference<CEntity> lEntityTA = mEntitiesManager.CreateEntity("GO-TA");
        lEntityTA->AddComponent<CTransform2DComponent>();
        lEntityTA->AddComponent<CTestComponentA>();

        CReference<CEntity> lEntityTB = mEntitiesManager.CreateEntity("GO-TB");
        lEntityTB->AddComponent<CTransform2DComponent>();
        lEntityTB->AddComponent<CTestComponentB>();

        CReference<CEntity> lEntityTAB = mEntitiesManager.CreateEntity("GO-TAB");
        lEntityTAB->AddComponent<CTransform2DComponent>();
        lEntityTAB->AddComponent<CTestComponentA>();
        lEntityTAB->AddComponent<CTestComponentB>();
    }

    const bool lUseComplexScene = !lUseBaseScene;
    if (lUseComplexScene)
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
            CReference<CEntity> lEntity = mEntitiesManager.CreateEntity("Sprite +1");
            lEntity->AddComponent<CTransform2DComponent>();
            lEntity->AddComponent<CSpriteComponent>();
            CReference<CTransform2DComponent> lTransform = lEntity->GetComponent<CTransform2DComponent>();
            lTransform->SetTranslation(TVec2(1.0f, 1.0f));
        }
        CReference<CTransform2DComponent> lParentTransform;
        {
            CReference<CEntity> lEntity = mEntitiesManager.CreateEntity("Sprite 0");
            lEntity->AddComponent<CTransform2DComponent>();
            lEntity->AddComponent<CSpriteComponent>();
            lEntity->AddComponent<CTestComponentA>();
            CReference<CTransform2DComponent> lTransform = lEntity->GetComponent<CTransform2DComponent>();
            lTransform->SetTranslation(TVec2(0.0f, 0.0f));
            lTransform->SetScale(TVec2(0.5f, 2.0f));
            lParentTransform = lTransform;
        }
        {
            CReference<CEntity> lEntity = mEntitiesManager.CreateEntity("Sprite -1");
            lEntity->AddComponent<CTransform2DComponent>();
            lEntity->AddComponent<CSpriteComponent>();
            CReference<CTransform2DComponent> lTransform = lEntity->GetComponent<CTransform2DComponent>();
            lTransform->SetTranslation(TVec2(-1.0f, -1.0f));
            lTransform->SetParentTransform(lParentTransform);
        }
    }

    return lOk;
}

} // maz

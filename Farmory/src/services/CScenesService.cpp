#include <maz/globals.h>
#include "CScenesService.h"

#include <scenes/CGameObject.h>
#include <scenes/CTransformComponent.h>
#include <scenes/test/CTestSystems.h>
#include <scenes/CTransformComponent.h>
#include <scenes/test/CTestComponents.h>

namespace maz
{

CScenesService::CScenesService()
    : mComponentsManager()
    , mGameObjectsManager(mComponentsManager)
{ ; }


CScenesService::~CScenesService() { ; }

void test()
{
    CComponentsManager* componentsManager = MAZ_NEW(CComponentsManager);
    CGameObjectsManager* gameObjectsManager = MAZ_NEW(CGameObjectsManager, *componentsManager);
    componentsManager->RegisterComponent<CTransformComponent>();
    CReference<CGameObject> gameObject = gameObjectsManager->CreateGameObject(nullptr, "Timmy");
    gameObjectsManager->RefreshGameObjects();
    CReference<CTransformComponent> transform1 = gameObject->AddComponent<CTransformComponent>();
    bool hasIt = gameObject->HasComponent<CTransformComponent>();
    CReference<CTransformComponent> transform2 = gameObject->GetComponent<CTransformComponent>();
    bool isSame = (transform1 == transform2);
    bool isValid1 = (bool)transform1;
    bool isValid2 = (bool)transform2;
    componentsManager->RefreshComponents();
    hasIt = gameObject->HasComponent<CTransformComponent>();
    transform2 = gameObject->GetComponent<CTransformComponent>();
    isSame = (transform1 == transform2);
    transform2 = CReference<CTransformComponent>();
    isValid1 = (bool)transform1;
    isValid2 = (bool)transform2;
    gameObject->RemoveComponent<CTransformComponent>();
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

    lOk = lOk && mComponentsManager.RegisterComponent<CTransformComponent>();
    lOk = lOk && mComponentsManager.RegisterComponent<CTestComponentA>();
    lOk = lOk && mComponentsManager.RegisterComponent<CTestComponentB>();
    //lOk = lOk && mSystemsManager.RegisterSystem<CSystemTransformA>();
    //lOk = lOk && mSystemsManager.RegisterSystem<CSystemTransformB>();
    MAZ_ASSERT(lOk, "WTF?!");

    return lOk;
}


void CScenesService::End()
{
    mGameObjectsManager.Shutdown();
    mComponentsManager.Shutdown();
    //mSystemsManager.Shutdown();
}


bool CScenesService::IsOk() const
{
    return true;
}


void CScenesService::PreUpdate()
{}


void CScenesService::Update()
{
    // Update Scene loading
    mGameObjectsManager.RefreshGameObjects();
    mComponentsManager.RefreshComponents();
    //mSystemsManager.RefreshSystems();
}


void CScenesService::PostUpdate()
{}

} // maz

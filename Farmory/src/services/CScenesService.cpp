#include <maz/globals.h>
#include "CScenesService.h"

#include <scenes/ComponentsManager.h>
#include <scenes/CGameObjectsManager.h>
#include <scenes/CGameObject.h>
#include <scenes/CTransformComponent.h>

namespace maz
{

CScenesService::CScenesService() { ; }
CScenesService::~CScenesService() { ; }

bool CScenesService::Init()
{
    bool lOk = true;

    // TODO
    /*
        - Init and close methods for CComponentsManager and CGameObjectsManager
        - On Close methods, clear the buffers and Reset ReferenceHolders
        - When destroying a CGameObject, remove its Components from CComponentsManager

        - Make Create/Destroy of gameobjects be effective when a new Update method is called in CGameObjectsManager
    */
    
    CComponentsManager* componentsManager = MAZ_NEW(CComponentsManager);
    CGameObjectsManager* gameObjectsManager = MAZ_NEW(CGameObjectsManager, *componentsManager);
    componentsManager->RegisterComponent<CTransformComponent>();
    CReference<CGameObject> gameObject = gameObjectsManager->CreateGameObject(nullptr, "Timmy");
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

    MAZ_UNUSED_VAR(hasIt);
    MAZ_UNUSED_VAR(isSame);
    MAZ_UNUSED_VAR(isValid1);
    MAZ_UNUSED_VAR(isValid2);

    MAZ_ASSERT(!transform1, "Transform should be invalid at this point");
    MAZ_ASSERT(!transform2, "Transform should be invalid at this point");



    return lOk;
}


void CScenesService::End()
{

}


bool CScenesService::IsOk() const
{
    return true;
}


void CScenesService::PreUpdate()
{}


void CScenesService::Update()
{}


void CScenesService::PostUpdate()
{}

} // maz

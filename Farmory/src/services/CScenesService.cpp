#include <maz/globals.h>
#include "CScenesService.h"

#include <scenes/ComponentsManager.h>
#include <scenes/CGameObject.h>
#include <scenes/CTransformComponent.h>

namespace maz
{

CScenesService::CScenesService() { ; }
CScenesService::~CScenesService() { ; }

bool CScenesService::Init()
{
    bool lOk = true;

    CComponentsManager componentsManager;
    componentsManager.RegisterComponent<CTransformComponent>();
    CGameObject* gameObject = MAZ_NEW(CGameObject, componentsManager, nullptr, "Timmy");
    CReference<CTransformComponent> transform1 = gameObject->AddComponent<CTransformComponent>();
    const bool hasIt = gameObject->HasComponent<CTransformComponent>();
    CReference<CTransformComponent> transform2 = gameObject->GetComponent<CTransformComponent>();
    CTransformComponent* transformPtr = transform1.get();
    const bool isSame = (transform1 == transform2);
    bool isValid1 = (bool)transform1;
    bool isValid2 = (bool)transform2;
    gameObject->RemoveComponent<CTransformComponent>();
    isValid1 = (bool)transform1;
    isValid2 = (bool)transform2;


    MAZ_UNUSED_VAR(hasIt);
    MAZ_UNUSED_VAR(transformPtr);
    MAZ_UNUSED_VAR(isSame);
    MAZ_UNUSED_VAR(isValid1);
    MAZ_UNUSED_VAR(isValid2);

    MAZ_ASSERT(transform1, "Invalid transform");



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

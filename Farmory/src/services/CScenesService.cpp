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
    CReference<CTransformComponent> transform = gameObject->AddComponent<CTransformComponent>();
    const bool hasIt = gameObject->HasComponent<CTransformComponent>();
    CReference<CTransformComponent> transform2 = gameObject->GetComponent<CTransformComponent>();
    CTransformComponent* transformPtr = transform.get();
    const bool isSame = (transform == transform2);


    MAZ_UNUSED_VAR(hasIt);
    MAZ_UNUSED_VAR(transformPtr);
    MAZ_UNUSED_VAR(isSame);

    MAZ_ASSERT(transform, "Invalid transform");



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

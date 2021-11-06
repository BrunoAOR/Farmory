#include <maz/globals.h>
#include "CServicesManager.h"

//Services
#include <services/CTimeService.h>
#include <services/CInputService.h>
#include <services/CScenesService.h>
#include <services/CRenderService.h>
#include <test/CTestService.h>
#include <services/CImGuiService.h>


namespace maz
{

namespace Services
{

namespace
{
CServicesManager* gpServiceManager = nullptr;
} // anonymous


bool StartUp()
{
    bool lOk = nullptr == gpServiceManager;
    
    MAZ_ASSERT(nullptr == gpServiceManager, "Called for a second time after successful initialization. No action will be performed!");
    if (lOk)
    {
        gpServiceManager = MAZ_NEW(CServicesManager);
        lOk = gpServiceManager->Init();
    }
    
    return lOk;
}


void Shutdown()
{
    MAZ_ASSERT(nullptr != gpServiceManager, "Called without having called StartUp before hand. No action will be performed!");
    if (nullptr != gpServiceManager)
    {
        gpServiceManager->End();
        MAZ_DELETE(gpServiceManager);
        gpServiceManager = nullptr;
    }
}


CServicesManager* GetManager()
{
    return gpServiceManager;
}

} // Services


CServicesManager::CServicesManager() { ; }
CServicesManager::~CServicesManager() { ; }


bool CServicesManager::Init()
{
    bool lOk = true;

    if (!mServices.empty())
    {
        MAZ_ERROR("Attempting to initialize the CServicesManager when it has already been initialized. the Manager will be Ended before continuing with initialization!");
        End();
    }

    mServices.push_back(MAZ_NEW(CTimeService));
    mServices.push_back(MAZ_NEW(CInputService));
    mServices.push_back(MAZ_NEW(CScenesService));
    mServices.push_back(MAZ_NEW(CRenderService));
    mServices.push_back(MAZ_NEW(CTestService));
#ifdef EDITOR
    mServices.push_back(MAZ_NEW(CImGuiService));
#endif

    for (size_t i = 0, iCount = mServices.size(); (i < iCount) && lOk; ++i)
    {
        lOk = mServices[i]->Init();
    }

    return lOk;
}


void CServicesManager::End()
{
    for (int i = static_cast< int >(mServices.size()) - 1; i >= 0; --i)
    {
        mServices[i]->End();
    }
    
    for (int i = static_cast<int>(mServices.size()) - 1; i >= 0; --i)
    {
        MAZ_DELETE(mServices[i]);
    }

    mServices.clear();
}


bool CServicesManager::IsOk()
{
    bool lOk = true;

    for (size_t i = 0, iCount = mServices.size(); (i < iCount) && lOk; ++i)
    {
        lOk = mServices[i]->IsOk();
    }

    return lOk;
}


void CServicesManager::PreUpdate()
{
    for (size_t i = 0, iCount = mServices.size(); (i < iCount); ++i)
    {
        mServices[i]->PreUpdate();
    }
}


void CServicesManager::Update()
{
    for (size_t i = 0, iCount = mServices.size(); (i < iCount); ++i)
    {
        mServices[i]->Update();
    }
}


void CServicesManager::PostUpdate()
{
    for (size_t i = 0, iCount = mServices.size(); (i < iCount); ++i)
    {
        mServices[i]->PostUpdate();
    }
}

} // maz

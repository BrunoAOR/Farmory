#include <maz/globals.h>
#include "CServicesManager.h"

//Services
#include <services/CTimeService.h>
#include <services/CInputService.h>
#include <test/CTestService.h>
#include <services/CRenderService.h>


namespace maz
{

namespace Services
{

namespace
{
CServicesManager* gServiceManager = nullptr;
} // anonymous


bool StartUp()
{
    bool lOk = nullptr == gServiceManager;
    
    MAZ_ASSERT(nullptr == gServiceManager, "{Services} StartUp - Called for a second time after successful initialization. No action will be performed!");
    if (lOk)
    {
        gServiceManager = MAZ_NEW(CServicesManager);
        lOk = gServiceManager->Init();
    }
    
    return lOk;
}


void Shutdown()
{
    MAZ_ASSERT(nullptr != gServiceManager, "{Services} Shutdown - Called without having called StartUp before hand. No action will be performed!");
    if (nullptr != gServiceManager)
    {
        gServiceManager->End();
        MAZ_DELETE(gServiceManager);
        gServiceManager = nullptr;
    }
}

CServicesManager* GetManager()
{
    return gServiceManager;
}

} // Services

CServicesManager::CServicesManager() { ; }
CServicesManager::~CServicesManager() { ; }

bool CServicesManager::Init()
{
    bool lOk = true;

    if (!mServices.empty())
    {
        MAZ_ERROR("[CServicesManager]::Init - Attempting to initialize the CServicesManager when it has already been initialized. the Manager will be Ended before continuing with initialization!");
        End();
    }

    mServices.push_back(MAZ_NEW(CTimeService));
    mServices.push_back(MAZ_NEW(CInputService));
    mServices.push_back(MAZ_NEW(CRenderService));
    mServices.push_back(MAZ_NEW(CTestService));

    for (size_t i = 0, iMax = mServices.size(); (i < iMax) && lOk; ++i)
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

    for (size_t i = 0, iMax = mServices.size(); (i < iMax) && lOk; ++i)
    {
        lOk = mServices[i]->IsOk();
    }

    return lOk;
}


void CServicesManager::Update()
{
    for (size_t i = 0, iMax = mServices.size(); (i < iMax); ++i)
    {
        mServices[i]->Update();
    }
}

} // maz

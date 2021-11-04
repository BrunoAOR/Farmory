#include <maz/globals.h>
#include "mazApp.h"
#include <maz/CGlfwHandler.h>
#include <services/CServicesManager.h>

#include <test/test.h>


namespace maz
{

int MazMain()
{
    TestMain();

    MAZ_LOGGER("Starting up");

    bool lOk = true;
    CServicesManager lServicesManager;

    lOk = lOk && CGlfwHandler::StartUp();
    lOk = lOk && Services::StartUp();

    // Loop
    MAZ_LOGGER("Loop will start");
    while (lOk && !CGlfwHandler::IsWindowCloseRequested() && !maz::global::gShouldClose)
    {
        CGlfwHandler::PollEvents();
        Services::GetManager()->PreUpdate();
        Services::GetManager()->Update();
        Services::GetManager()->PostUpdate();
    }

    Services::Shutdown();
    CGlfwHandler::ShutDown();

    MAZ_LOGGER("Closing up");
    return 0;
}

} // maz

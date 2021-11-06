#include <maz/globals.h>
#include "mazApp.h"
#include <maz/CGlfwHandler.h>
#include <services/CServicesManager.h>

#include <test/test.h>


namespace maz
{

int MazMain()
{
#ifdef MAZ_DEBUG
    TestMain();
#endif // MAZ_DEBUG

    MAZ_LOGGER("Starting up");

    bool lOk = true;
    lOk = lOk && CGlfwHandler::StartUp();
    lOk = lOk && Services::StartUp();

    // Loop
    if (lOk)
    {
        MAZ_LOGGER("Loop will start");
        while (!CGlfwHandler::IsWindowCloseRequested() && !maz::global::gShouldClose)
        {
            CGlfwHandler::PollEvents();
            Services::GetManager()->PreUpdate();
            Services::GetManager()->Update();
            Services::GetManager()->PostUpdate();
        }
    }

    Services::Shutdown();
    CGlfwHandler::ShutDown();

    MAZ_LOGGER("Closing up");
    return lOk ? 0 : -1;
}

} // maz

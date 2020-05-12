#include <maz/globals.h>
#include "mazApp.h"
#include <maz/CGlfwHandler.h>
#include <services/CServicesManager.h>
#include <GLFW/glfw3.h>

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
    while (lOk && !glfwWindowShouldClose(CGlfwHandler::GetWindow()) && !maz::global::gShouldClose)
    {
        glfwPollEvents();
        Services::GetManager()->Update();
    }

    Services::Shutdown();
    CGlfwHandler::ShutDown();

    MAZ_LOGGER("Closing up");
    return 0;
}

} // maz

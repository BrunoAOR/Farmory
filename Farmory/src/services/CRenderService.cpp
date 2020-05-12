#include <maz/globals.h>
#include "CRenderService.h"
#include <maz/CGlfwHandler.h>
#include <GLAD/glad.h>
#include <GLFW/glfw3.h>
#include <maz/CDelegate.h>

// TMP
#include <services/CServicesManager.h>
#include <test/CTestService.h>


namespace maz
{

namespace
{

const int   kWindowWidth  = 1024; // 1920;
const int   kWindowHeight = 768;  // 1080;
const char* kWindowName   = "Test";

} // anonymous


CRenderService::CRenderService() : mWindow(nullptr)
{
}


CRenderService::~CRenderService()
{
}


bool CRenderService::Init()
{
    bool lOk = true;

    lOk = lOk && CGlfwHandler::CreateGlWindow(kWindowWidth, kWindowHeight, kWindowName);

    // Defaults that should probably go elsewhere
    if (lOk)
    {
        glViewport(0, 0, kWindowWidth, kWindowHeight);
    }

    // Callback registering
    lOk = lOk && CGlfwHandler::RegisterWindowResizedCallback(CGlfwHandler::WindowResizedCallback::Create< CRenderService, &CRenderService::OnWindowResized >(this));

    if (lOk)
    {
        mWindow = CGlfwHandler::GetWindow();
        lOk = (nullptr != mWindow);
    }

    return lOk;
}


void CRenderService::End()
{
    CGlfwHandler::UnregisterWindowResizedCallback(CGlfwHandler::WindowResizedCallback::Create< CRenderService, &CRenderService::OnWindowResized >(this));
}


bool CRenderService::IsOk() const
{
    return (nullptr != mWindow) && (mWindow == CGlfwHandler::GetWindow());
}


void CRenderService::Update()
{
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Render calls
    Services::GetManager()->GetService< CTestService >()->TestRender();

    glfwSwapBuffers(mWindow);
}


void CRenderService::OnWindowResized(int aWidth, int aHeight)
{
    glViewport(0, 0, aWidth, aHeight);
}

} // maz

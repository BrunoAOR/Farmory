#include <maz/globals.h>
#include <maz/CGlfwHandler.h>
#include <GLAD/glad.h>
#include <GLFW/glfw3.h>

namespace maz
{
CGlfwHandler*   CGlfwHandler::sInstance = nullptr;
bool            CGlfwHandler::sOk       = false;
GLFWwindow*     CGlfwHandler::sWindow   = nullptr;
std::vector< CGlfwHandler::FramebufferResizedCallback > CGlfwHandler::sWindowResizedCallbacks;
std::vector< CGlfwHandler::KeyboardStateCallback      > CGlfwHandler::sKeyboardStateCallbacks;
std::vector< CGlfwHandler::MousePositionCallback      > CGlfwHandler::sMousePositionCallbacks;
std::vector< CGlfwHandler::MouseScrollCallback        > CGlfwHandler::sMouseScrollCallbacks;


CGlfwHandler::CGlfwHandler()
{
}


CGlfwHandler::~CGlfwHandler()
{
}


bool CGlfwHandler::StartUp()
{
    bool lOk = false; 
    if (!sOk)
    {
        sOk = glfwInit();
        MAZ_LOGGER_IF(!sOk, "[CGlfwHandled]::StartUp - glfwInit failed!");
        lOk = sOk;
    }
    return lOk;
}


void CGlfwHandler::ShutDown()
{
    if (sOk)
    {
        glfwTerminate();
        sOk = false;
    }
}


bool CGlfwHandler::IsOk()
{
    return sOk;
}


bool CGlfwHandler::CreateGlWindow(int aWindowWidth, int aWindowHeight, const char* aWindowName)
{
    if (sOk && (sWindow == nullptr))
    {
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        // See https://www.glfw.org/docs/latest/window.html#window_hints for more hints

        sWindow = glfwCreateWindow(aWindowWidth, aWindowHeight, aWindowName, nullptr, nullptr);
        sOk = sOk && sWindow;
        if (sOk)
        {
            glfwMakeContextCurrent(sWindow);
        }
        else
        {
            MAZ_LOGGER("[CGlfwHandled]::CreateWindow - Failed to create a Window!");
            glfwTerminate();
        }

        if (sOk)
        {
            sOk = gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress));
            MAZ_LOGGER_IF(!sOk, "[CGlfwHandled]::StartUp - Failed to initialize GLAD!");
        }

        if (sOk)
        {
            glEnable(GL_DEPTH_TEST);
            LogInfo();
            SetCallbacks();
        }
    }
    return sOk;
}


GLFWwindow* CGlfwHandler::GetWindow()
{
    return sWindow;
}

bool CGlfwHandler::GetFramebufferSize(int& aOutWidth, int& aOutHeight)
{
    if (sOk)
    {
        glfwGetFramebufferSize(sWindow, &aOutWidth, &aOutHeight);
    }

    return sOk;
}


bool CGlfwHandler::SetCursorMode(ECursorMode aCursorMode)
{
    bool lOk = sOk;

    if (lOk)
    {
        switch (aCursorMode)
        {
        case maz::ECursorMode::NORMAL:
            glfwSetInputMode(sWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            break;
        case maz::ECursorMode::HIDDEN:
            glfwSetInputMode(sWindow, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
            break;
        case maz::ECursorMode::DISABLED:
            glfwSetInputMode(sWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            break;
        default:
            lOk = false;
            MAZ_ERROR("[CGlfwHandler]::SetCursorMode - Invalid ECursorMode value received. No  change will be performed!");
            break;
        }
    }

    return lOk;
}



bool CGlfwHandler::RegisterFramebufferResizedCallback(FramebufferResizedCallback aCallback)
{
    bool lFound = false;
    
    for (size_t i = 0, iCount = sWindowResizedCallbacks.size(); (i < iCount) && !lFound; ++i)
    {
        lFound = (sWindowResizedCallbacks[i] == aCallback);
    }
    if (!lFound)
    {
        sWindowResizedCallbacks.push_back(aCallback);
    }

    return !lFound;
}


bool CGlfwHandler::UnregisterFramebufferResizedCallback(FramebufferResizedCallback aCallback)
{
    bool lFound = false;

    for (size_t i = 0, iCount = sWindowResizedCallbacks.size(); (i < iCount) && !lFound; ++i)
    {
        lFound = (sWindowResizedCallbacks[i] == aCallback);
        if (lFound)
        {
            sWindowResizedCallbacks.erase(sWindowResizedCallbacks.begin() + i);
        }
    }

    return lFound;
}


bool CGlfwHandler::RegisterKeyboardStateCallback(KeyboardStateCallback aCallback)
{
    bool lFound = false;
    
    for (size_t i = 0, iCount = sKeyboardStateCallbacks.size(); (i < iCount) && !lFound; ++i)
    {
        lFound = (sKeyboardStateCallbacks[i] == aCallback);
    }
    if (!lFound)
    {
        sKeyboardStateCallbacks.push_back(aCallback);
    }

    return !lFound;
}


bool CGlfwHandler::UnregisterKeyboardStateCallback(KeyboardStateCallback aCallback)
{
    bool lFound = false;

    for (size_t i = 0, iCount = sKeyboardStateCallbacks.size(); (i < iCount) && !lFound; ++i)
    {
        lFound = (sKeyboardStateCallbacks[i] == aCallback);
        if (lFound)
        {
            sKeyboardStateCallbacks.erase(sKeyboardStateCallbacks.begin() + i);
        }
    }

    return lFound;
}


bool CGlfwHandler::RegisterMousePositionCallback(MousePositionCallback aCallback)
{
    bool lFound = false;

    for (size_t i = 0, iCount = sMousePositionCallbacks.size(); (i < iCount) && !lFound; ++i)
    {
        lFound = (sMousePositionCallbacks[i] == aCallback);
    }
    if (!lFound)
    {
        sMousePositionCallbacks.push_back(aCallback);
    }

    return !lFound;
}


bool CGlfwHandler::UnregisterMousePositionCallback(MousePositionCallback aCallback)
{
    bool lFound = false;

    for (size_t i = 0, iCount = sMousePositionCallbacks.size(); (i < iCount) && !lFound; ++i)
    {
        lFound = (sMousePositionCallbacks[i] == aCallback);
        if (lFound)
        {
            sMousePositionCallbacks.erase(sMousePositionCallbacks.begin() + i);
        }
    }

    return lFound;
}


bool CGlfwHandler::RegisterMouseScrollallback(MouseScrollCallback aCallback)
{
    bool lFound = false;

    for (size_t i = 0, iCount = sMouseScrollCallbacks.size(); (i < iCount) && !lFound; ++i)
    {
        lFound = (sMouseScrollCallbacks[i] == aCallback);
    }
    if (!lFound)
    {
        sMouseScrollCallbacks.push_back(aCallback);
    }

    return !lFound;
}


bool CGlfwHandler::UnregisterMouseScrollCallback(MouseScrollCallback aCallback)
{
    bool lFound = false;

    for (size_t i = 0, iCount = sMouseScrollCallbacks.size(); (i < iCount) && !lFound; ++i)
    {
        lFound = (sMouseScrollCallbacks[i] == aCallback);
        if (lFound)
        {
            sMouseScrollCallbacks.erase(sMouseScrollCallbacks.begin() + i);
        }
    }

    return lFound;
}


void CGlfwHandler::LogInfo()
{
    MAZ_LOGGER("[CGlfwHandler]::LogInfo - Vendor: %s", glGetString(GL_VENDOR));
    MAZ_LOGGER("[CGlfwHandler]::LogInfo - Renderer: %s", glGetString(GL_RENDERER));
    MAZ_LOGGER("[CGlfwHandler]::LogInfo - OpenGL version supported %s", glGetString(GL_VERSION));
    MAZ_LOGGER("[CGlfwHandler]::LogInfo - GLSL: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
    int lMaxVertexAttribs;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &lMaxVertexAttribs);
    MAZ_LOGGER("[CGlfwHandler]::LogInfo - Max Vertex Attributes Count: %d\n", lMaxVertexAttribs);
}


void CGlfwHandler::SetCallbacks()
{
    glfwSetFramebufferSizeCallback(sWindow, &CGlfwHandler::OnFramebufferResized);
    glfwSetKeyCallback(sWindow, &CGlfwHandler::OnKeyboardState);
    glfwSetCursorPosCallback(sWindow, &CGlfwHandler::OnMousePosition);
    glfwSetScrollCallback(sWindow, &CGlfwHandler::OnMouseScroll);
}


void CGlfwHandler::OnFramebufferResized(GLFWwindow* aWindow, int aWidth, int aHeight)
{
    MAZ_UNUSED_VAR(aWindow);
    MAZ_ASSERT(sWindow == aWindow, "[CGlfwHandler]::OnFramebufferResized - Callback received for a window other that the managed one!");

    for (size_t i = 0, iCount = sWindowResizedCallbacks.size(); (i < iCount); ++i)
    {
        sWindowResizedCallbacks[i](aWidth, aHeight);
    }
}


void CGlfwHandler::OnKeyboardState(GLFWwindow* aWindow, int aKey, int aScancode, int aAction, int aModifierBits)
{
    MAZ_UNUSED_VAR(aWindow);
    MAZ_ASSERT(sWindow == aWindow, "[CGlfwHandler]::OnKeyboardState - Callback received for a window other that the managed one!");

    for (size_t i = 0, iCount = sKeyboardStateCallbacks.size(); (i < iCount); ++i)
    {
        sKeyboardStateCallbacks[i](aKey, aScancode, aAction, aModifierBits);
    }
}


void CGlfwHandler::OnMousePosition(GLFWwindow* aWindow, double aXPos, double aYPos)
{
    MAZ_UNUSED_VAR(aWindow);
    MAZ_ASSERT(sWindow == aWindow, "[CGlfwHandler]::OnMousePosition - Callback received for a window other that the managed one!");

    for (size_t i = 0, iCount = sMousePositionCallbacks.size(); (i < iCount); ++i)
    {
        sMousePositionCallbacks[i](aXPos, aYPos);
    }
}


void CGlfwHandler::OnMouseScroll(GLFWwindow* aWindow, double aXOffset, double aYOffset)
{
    MAZ_UNUSED_VAR(aWindow);
    MAZ_ASSERT(sWindow == aWindow, "[CGlfwHandler]::OnMouseScroll - Callback received for a window other that the managed one!");

    for (size_t i = 0, iCount = sMouseScrollCallbacks.size(); (i < iCount); ++i)
    {
        sMouseScrollCallbacks[i](aXOffset, aYOffset);
    }
}


} //maz
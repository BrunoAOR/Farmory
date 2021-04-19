#ifndef _H_C_GLWF_HANDLER_
#define _H_C_GLWF_HANDLER_

#include <vector>
#include <maz/CDelegate.h>

struct GLFWwindow;
namespace maz
{
enum class ECursorMode : uint32;

class CGlfwHandler
{
public:

    using FramebufferResizedCallback = CDelegate< void, int, int >;
    using KeyboardStateCallback      = CDelegate< void, int, int, int, int >;
    using MousePositionCallback      = CDelegate< void, double, double >;
    using MouseScrollCallback        = CDelegate< void, double, double >;

    // Lifecycle
    static bool StartUp();
    static void ShutDown();

    static bool IsOk();

    static bool CreateGlWindow(int aWindowWidth, int aWindowHeight, const char* aWindowName);
    static GLFWwindow* GetWindow();

    // Getters
    static bool GetFramebufferSize(int& aOutWidth, int& aOutHeight);

    // Setters
    static bool SetCursorMode(ECursorMode aCursorMode);

    // Callbacks
    static bool RegisterFramebufferResizedCallback(FramebufferResizedCallback aCallback);
    static bool UnregisterFramebufferResizedCallback(FramebufferResizedCallback aCallback);

    static bool RegisterKeyboardStateCallback(KeyboardStateCallback aCallback);
    static bool UnregisterKeyboardStateCallback(KeyboardStateCallback aCallback);

    static bool RegisterMousePositionCallback(MousePositionCallback aCallback);
    static bool UnregisterMousePositionCallback(MousePositionCallback aCallback);

    static bool RegisterMouseScrollallback(MouseScrollCallback aCallback);
    static bool UnregisterMouseScrollCallback(MouseScrollCallback aCallback);

private: 
    CGlfwHandler();
    ~CGlfwHandler();

    static void LogInfo();
    static void SetCallbacks();
    static void OnFramebufferResized(GLFWwindow* aWindow, int aWidth, int aHeight);
    static void OnKeyboardState(GLFWwindow* aWindow, int aKey, int aScancode, int aAction, int aModifierBits);
    static void OnMousePosition(GLFWwindow* aWindow, double aXPos, double aYPos);
    static void OnMouseScroll  (GLFWwindow* aWindow, double aXOffset, double aYOffset);


private:
    static bool sOk;
    static GLFWwindow* sWindow;
    
    static std::vector< FramebufferResizedCallback > sWindowResizedCallbacks;
    static std::vector< KeyboardStateCallback      > sKeyboardStateCallbacks;
    static std::vector< MousePositionCallback      > sMousePositionCallbacks;
    static std::vector< MouseScrollCallback        > sMouseScrollCallbacks;
};


enum class ECursorMode : uint32
{
    NORMAL,
    HIDDEN,
    DISABLED
};

} // maz

#endif // !_H_C_GLWF_HANDLER_

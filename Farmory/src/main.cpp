#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <maz/mazApp.h>
#include <maz/globals.h>
#include <test//test.h>
#include <maz/CGlfwHandler.h>
#include <services/CServicesManager.h>
#include <GLFW/glfw3.h>


#if defined(IS_X86)
int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR pCmdLine, _In_ int nCmdShow)
#elif defined(IS_X64)
int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR pCmdLine, _In_ int nCmdShow)
#endif
{
    MAZ_UNUSED_VAR(hInstance);
    MAZ_UNUSED_VAR(hPrevInstance);
    MAZ_UNUSED_VAR(pCmdLine);
    MAZ_UNUSED_VAR(nCmdShow);
    return maz::MazMain();
}


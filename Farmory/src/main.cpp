#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <maz/globals.h>
#include <maz/mazApp.h>


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

/*
* Variable naming convention
*   Variable names must use camel case (1st character in lower case) and be prefixed with the following sets of characters according to the following rules:
*       1st set:
*         k:  Constant (either global, compilation unit or class static) (so, non-block scope local)
*         g:  Global variable
*         s:  Static variable (either compilation unit static, class static or function static)
*         m:  Class member variable
*         a:  Function argument
*         l:  Block scope variable
*       2nd set:
*         p:  Pointer
*         r:  L-value reference
*         rr: R-Value reference
*         No character: L-value variable
*       3rd set:
*         Out: When a function parameter will be filled in by the called function. The initial value is ignored
*         InOut: When a function parameter will be filled in by the called function. The initial value is relevant for the called function
*   Exceptions:
*       Iterators and counters within a for-loop or while-loop
* 
* Function naming convention
*   Function names must use camel case. the first character of the name must  be:
*       Upper case: When the function is a free function or a public class member function.
*       Lower case: When the function is a protected or private class member function.
*/

#include <maz/globals.h>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdio.h>


namespace maz
{

namespace global
{

bool gShouldClose = false;

} // global

void Logger(const char* apFileName, int aLine, const char* apFunctionName, const char* apFormat, ...)
{
    static char tmp_string[4096];
    static char tmp_string2[4096];
    static va_list ap;

    /* Construct the string from variable arguments */
    va_start(ap, apFormat);
    vsprintf_s(tmp_string, 4096, apFormat, ap);
    va_end(ap);
    sprintf_s(tmp_string2, 4096, "%s (ln. %d) [%s]: %s\n", apFileName, aLine, apFunctionName, tmp_string);
    OutputDebugStringA(tmp_string2);
}

} // maz

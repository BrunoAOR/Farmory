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

void Logger(const char* file, int line, const char* function, const char* format, ...)
{
    static char tmp_string[4096];
    static char tmp_string2[4096];
    static va_list ap;

    /* Construct the string from variable arguments */
    va_start(ap, format);
    vsprintf_s(tmp_string, 4096, format, ap);
    va_end(ap);
    sprintf_s(tmp_string2, 4096, "%s (ln. %d) [%s]: %s\n", file, line, function, tmp_string);
    OutputDebugStringA(tmp_string2);
}

} // maz

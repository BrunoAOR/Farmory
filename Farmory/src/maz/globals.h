#ifndef _H_GLOBALS_
#define _H_GLOBALS_

#include <maz/config.h>
#include <maz/types.h>
#include <maz/math.h>
#include <maz/constants.h>
#include <maz/enumHelpers.h>
#include <assert.h>


#define MAZ_UNUSED_VAR(var) (var);
#ifdef MAZ_DEBUG
#define MAZ_DBG_UNUSED_VAR(var) (var);
#else
#define MAZ_DBG_UNUSED_VAR(var) ((void)0);
#endif // MAZ_DEBUG


#define MAZ_REL_LOGGER(format, ...) maz::Logger(__FILE__, __LINE__, MAZ_FUNC_SIG, format, __VA_ARGS__);
#ifdef MAZ_DEBUG
#define MAZ_LOGGER(format, ...) maz::Logger(__FILE__, __LINE__, MAZ_FUNC_SIG, format, __VA_ARGS__);
#define MAZ_LOGGER_IF(expression, format, ...) if(expression) { maz::Logger(__FILE__, __LINE__, MAZ_FUNC_SIG, format, __VA_ARGS__); }
#else
#define MAZ_LOGGER(format, ...) ((void)0);
#define MAZ_LOGGER_IF(expression, format, ...) ((void)0);
#endif // MAZ_DEBUG


#ifdef MAZ_LOG_VERBOSE
#define MAZ_LOGGER_VERBOSE(format, ...) maz::Logger(__FILE__, __LINE__, MAZ_FUNC_SIG, format, __VA_ARGS__);
#define MAZ_LOGGER_VERBOSE_IF(expression, format, ...) if(expression) { maz::Logger(__FILE__, __LINE__, MAZ_FUNC_SIG, format, __VA_ARGS__); }
#else
#define MAZ_LOGGER_VERBOSE(format, ...) ((void)0);
#define MAZ_LOGGER_VERBOSE_IF(expression, format, ...) ((void)0);
#endif


#ifdef MAZ_DEBUG
#define MAZ_NEW(klass, ...) new klass( ## __VA_ARGS__)
#define MAZ_PLACEMENT_NEW(ptr, klass, ...) new (ptr) klass( ## __VA_ARGS__)
#define MAZ_DELETE(ptr) delete ptr
#else
#define MAZ_NEW(klass, ...) new klass( ## __VA_ARGS__)
#define MAZ_PLACEMENT_NEW(ptr, klass, ...) new (ptr) klass( ## __VA_ARGS__)
#define MAZ_DELETE(ptr) delete ptr
#endif // MAZ_DEBUG


#ifdef MAZ_DEBUG
#define MAZ_ASSERT(expression, format, ...) if(!(expression)) maz::Logger(__FILE__, __LINE__, MAZ_FUNC_SIG, "%s" format, "ASSERT: ", __VA_ARGS__); assert(expression);
#define MAZ_ERROR(format, ...) maz::Logger(__FILE__, __LINE__, MAZ_FUNC_SIG, "%s" format, "ERROR: ", __VA_ARGS__); assert(false);
#else
#define MAZ_ASSERT(expression, format, ...) ((void)0);
#define MAZ_ERROR(format, ...) ((void)0);
#endif // MAZ_DEBUG


#define EDITOR

namespace maz
{

namespace global
{
extern bool gShouldClose;
} // global

void Logger(const char* pFileName, int aLine, const char* apFunctionName, const char* apFormat, ...);

} // maz

#endif // !_H_GLOBALS_

#ifndef _H_CONFIG_
#define _H_CONFIG_


#ifndef _MSC_VER
#error "Base macros are only defined for MSVC!"
#endif // !_MSC_VER

#ifdef MAZ_IS_X64
#undef MAZ_IS_X64
#endif // MAZ_IS_X64
#ifdef MAZ_IS_X86
#undef MAZ_IS_X86
#endif // MAZ_IS_X86

#if defined (_WIN64)
#define MAZ_IS_X64
#elif defined (_WIN32)
#define MAZ_IS_X86
#endif

#ifdef MAZ_RTTI
#undef MAZ_RTTI
#endif // MAZ_RTTI
#ifdef _CPPRTTI 
#define MAZ_RTTI
#endif // _CPPRTTI 

#ifdef MAZ_DEBUG
#undef MAZ_DEBUG
#endif // MAZ_DEBUG
#ifdef MAZ_RELEASE
#undef MAZ_RELEASE
#endif // MAZ_RELEASE 

#ifdef _DEBUG
#define MAZ_DEBUG
#elif defined (NDEBUG)
#define MAZ_RELEASE
#endif // _DEBUG

#ifdef MAZ_FUNC_NAME
#undef MAZ_FUNC_NAME
#endif // MAZ_FUNC_NAME
#define MAZ_FUNC_NAME __FUNCTION__

#ifdef MAZ_FUNC_SIG
#undef MAZ_FUNC_SIG
#endif // MAZ_FUNC_SIG
#define MAZ_FUNC_SIG __FUNCSIG__

#endif // !_H_CONFIG_

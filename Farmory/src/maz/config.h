#ifndef _H_CONFIG_
#define _H_CONFIG_

#ifdef IS_X64
#undef IS_X64
#endif // IS_X64
#ifdef IS_X86
#undef IS_X86
#endif // IS_X86

#if defined (_WIN64)
#define IS_X64
#undef  IS_X86
#elif defined (_WIN32)
#define IS_X86
#undef  IS_X64
#endif

#ifdef MAZ_DEBUG
#undef MAZ_DEBUG
#endif // MAZ_DEBUG
#ifdef MAZ_RELEASE
#undef MAZ_RELEASE
#endif //MAZ_RELEASE 

#ifdef _DEBUG
#define MAZ_DEBUG
#elif defined (NDEBUG)
#define MAZ_RELEASE
#endif // _DEBUG

#endif // !_H_CONFIG_

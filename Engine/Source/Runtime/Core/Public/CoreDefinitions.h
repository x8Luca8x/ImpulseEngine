#pragma once

#define PRAGMA_DISABLE_DEPRECATION_WARNINGS \
	__pragma(warning(push)) __pragma(warning(disable : 4996))

#define PRAGMA_ENABLE_DEPRECATION_WARNINGS __pragma(warning(pop))

#if !defined(IE_NODISCARD) && defined(__has_cpp_attribute)
#if __has_cpp_attribute(nodiscard)
#define IE_NODISCARD [[nodiscard]]
#endif
#endif
#ifndef IE_NODISCARD
#define IE_NODISCARD
#endif

/* Use before a function declaration to indicate that the function never returns */
#if !defined(IE_NORETURN) && defined(__has_cpp_attribute)
#if __has_cpp_attribute(noreturn)
#define IE_NORETURN [[noreturn]]
#endif
#endif
#ifndef IE_NORETURN
#define IE_NORETURN
#endif

/* Macro wrapper for the consteval keyword which isn't yet present on all compilers - constexpr
   can be used as a workaround but is less strict and so may let some non-consteval code pass */
#if defined(__cpp_consteval)
#define IE_CONSTEVAL consteval
#else
#define IE_CONSTEVAL constexpr
#endif

#ifndef FORCEINLINE
#define FORCEINLINE __forceinline
#endif

   // Platform detection

#if defined(_WIN32) || defined(_WIN64)

// Windows platform

#define PLATFORM_WINDOWS 1

#define DLLIMPORT __declspec(dllimport)
#define DLLEXPORT __declspec(dllexport)

#define IS_TCHAR_WIDE 1

#define USE_WIN_HEAP 1
#define WIN_ALLOC_ZEROED 1

#if defined(_WIN64)

// Windows 64-bit platform

#define PLATFORM_X64 1

#else

// Windows 32-bit platform

#define PLATFORM_X64 0

#endif

#else

#error "Unsupported platform"

#endif

// Compiler detection

#if defined(_MSC_VER)

// Microsoft Visual C++ compiler

#define COMPILER_MSVC 1

#else

#error "Unsupported compiler"

#endif

#define STDCALL __stdcall
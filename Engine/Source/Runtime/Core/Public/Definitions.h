#pragma once

#include "Platform/Platform.h"

// This block will be replaced by the build tool

#if _DEBUG

#define IE_BUILD_DEVELOPMENT 1
#define IE_BUILD_SHIPPING 0

#else

#define IE_BUILD_DEVELOPMENT 0
#define IE_BUILD_SHIPPING 1

#endif

#undef TEXT

enum
{
	INDEX_NONE = -1
};

typedef decltype(nullptr) NULLPTR_T;;

typedef FPlatformTypes::uint8 uint8;
typedef FPlatformTypes::uint16 uint16;
typedef FPlatformTypes::uint32 uint32;
typedef FPlatformTypes::uint64 uint64;

typedef FPlatformTypes::int8 int8;
typedef FPlatformTypes::int16 int16;
typedef FPlatformTypes::int32 int32;
typedef FPlatformTypes::int64 int64;

typedef FPlatformTypes::BYTE BYTE;
typedef FPlatformTypes::WORD WORD;
typedef FPlatformTypes::DWORD DWORD;
typedef FPlatformTypes::QWORD QWORD;

typedef FPlatformTypes::PTRINT PTRINT;
typedef FPlatformTypes::UPTRINT UPTRINT;

typedef FPlatformTypes::ANSICHAR ANSICHAR;
typedef FPlatformTypes::WIDECHAR WIDECHAR;
typedef FPlatformTypes::CHAR8 CHAR8;
typedef FPlatformTypes::CHAR16 CHAR16;
typedef FPlatformTypes::CHAR32 CHAR32;

typedef CHAR8 UTF8CHAR;
typedef CHAR16 UTF16CHAR;
typedef CHAR32 UTF32CHAR;

typedef decltype(nullptr) TYPE_OF_NULLPTR;

// Platform macros

#define ATEXT(x) x
#define WTEXT(x) L##x
#define U8TEXT(x) u8##x
#define U16TEXT(x) u##x
#define U32TEXT(x) U##x

#ifndef PLATFORM_WINDOWS
#define PLATFORM_WINDOWS 0
#endif

#ifndef PLATFORM_X64
#define PLATFORM_X64 0
#endif

#ifndef COMPILER_MSVC
#define COMPILER_MSVC 0
#endif

#define PLATFORM_X86 (!PLATFORM_X64)

#ifndef IS_TCHAR_WIDE
#define IS_TCHAR_WIDE 0
#endif

#ifndef IS_TCHAR_ANSI
#define IS_TCHAR_ANSI 0
#endif

#ifndef IS_TCHAR_UTF8
#define IS_TCHAR_UTF8 0
#endif

#ifndef IS_TCHAR_UTF16
#define IS_TCHAR_UTF16 0
#endif

#ifndef IS_TCHAR_UTF32
#define IS_TCHAR_UTF32 0
#endif

#ifndef DLLIMPORT
#define DLLIMPORT
#endif

#ifndef DLLEXPORT
#define DLLEXPORT
#endif

#ifndef IE_PACKAGED_BUILD
#define IE_PACKAGED_BUILD 0
#endif

#ifndef ENCRYPT_ARCHIVE_STRING

#if IE_DISABLE_STRING_ENCRYPTION

#define ENCRYPT_ARCHIVE_STRING 0

#else

#define ENCRYPT_ARCHIVE_STRING 1

#endif // IE_DISABLE_STRING_ENCRYPTION
#endif // ENCRYPT_ARCHIVE_STRING

// Platform types

#if IS_TCHAR_WIDE

typedef WIDECHAR TCHAR;
#define TEXT(x) WTEXT(x)

#elif IS_TCHAR_ANSI

typedef ANSICHAR TCHAR;
#define TEXT(x) ATEXT(x)

#elif IS_TCHAR_UTF8

typedef UTF8CHAR TCHAR;
#define TEXT(x) U8TEXT(x)

#elif IS_TCHAR_UTF16

typedef UTF16CHAR TCHAR;
#define TEXT(x) U16TEXT(x)

#elif IS_TCHAR_UTF32

typedef UTF32CHAR TCHAR;
#define TEXT(x) U32TEXT(x)

#else

#error "No TCHAR type defined"

#endif
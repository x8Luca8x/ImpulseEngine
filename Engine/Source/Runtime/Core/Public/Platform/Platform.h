#pragma once

#include "CoreModule.h"

template<typename T32BITS, typename T64BITS, int PointerSize>
struct SelectIntPointerType;

template<typename T32BITS, typename T64BITS>
struct SelectIntPointerType<T32BITS, T64BITS, 4>
{
	typedef T32BITS Type;
};

template<typename T32BITS, typename T64BITS>
struct SelectIntPointerType<T32BITS, T64BITS, 8>
{
	typedef T64BITS Type;
};

class CORE_API FGenericPlatform
{
public:
};

class CORE_API FGenericPlatformTypes
{
public:

	typedef unsigned char uint8;
	typedef unsigned short uint16;
	typedef unsigned int uint32;
	typedef unsigned long long uint64;

	typedef signed char int8;
	typedef signed short int16;
	typedef signed int int32;
	typedef signed long long int64;

	typedef uint8 BYTE;
	typedef uint16 WORD;
	typedef uint32 DWORD;
	typedef uint64 QWORD;

	typedef char ANSICHAR;
	typedef wchar_t WIDECHAR;
	typedef char8_t CHAR8;
	typedef char16_t CHAR16;
	typedef char32_t CHAR32;

	typedef SelectIntPointerType<int32, int64, sizeof(void*)>::Type PTRINT;
	typedef SelectIntPointerType<uint32, uint64, sizeof(void*)>::Type UPTRINT;
};

#if PLATFORM_WINDOWS
#include "Windows/WindowsPlatform.h"
#endif
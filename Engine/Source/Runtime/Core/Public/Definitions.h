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

/* Numeric constants
 *****************************************************************************/

#define MIN_uint8		((uint8)	0x00)
#define	MIN_uint16		((uint16)	0x0000)
#define	MIN_uint32		((uint32)	0x00000000)
#define MIN_uint64		((uint64)	0x0000000000000000)
#define MIN_int8		((int8)		-128)
#define MIN_int16		((int16)	-32768)
#define MIN_int32		((int32)	0x80000000)
#define MIN_int64		((int64)	0x8000000000000000)

#define MAX_uint8		((uint8)	0xff)
#define MAX_uint16		((uint16)	0xffff)
#define MAX_uint32		((uint32)	0xffffffff)
#define MAX_uint64		((uint64)	0xffffffffffffffff)
#define MAX_int8		((int8)		0x7f)
#define MAX_int16		((int16)	0x7fff)
#define MAX_int32		((int32)	0x7fffffff)
#define MAX_int64		((int64)	0x7fffffffffffffff)

#define MIN_flt			(1.175494351e-38F)			/* min positive value */
#define MAX_flt			(3.402823466e+38F)
#define MIN_dbl			(2.2250738585072014e-308)	/* min positive value */
#define MAX_dbl			(1.7976931348623158e+308)	


 /* Numeric type traits
  *****************************************************************************/

  /**
   * Helper class to map a numeric type to its limits
   */
template <typename NumericType>
struct TNumericLimits;


/**
 * Numeric limits for const types
 */
template <typename NumericType>
struct TNumericLimits<const NumericType>
	: public TNumericLimits<NumericType>
{ };


/**
 * Numeric limits for volatile types
 */
template <typename NumericType>
struct TNumericLimits<volatile NumericType>
	: public TNumericLimits<NumericType>
{ };


/**
 * Numeric limits for const volatile types
 */
template <typename NumericType>
struct TNumericLimits<const volatile NumericType>
	: public TNumericLimits<NumericType>
{ };


template<>
struct TNumericLimits<uint8>
{
	typedef uint8 NumericType;

	static constexpr NumericType Min()
	{
		return MIN_uint8;
	}

	static constexpr NumericType Max()
	{
		return MAX_uint8;
	}

	static constexpr NumericType Lowest()
	{
		return Min();
	}
};


template<>
struct TNumericLimits<uint16>
{
	typedef uint16 NumericType;

	static constexpr NumericType Min()
	{
		return MIN_uint16;
	}

	static constexpr NumericType Max()
	{
		return MAX_uint16;
	}

	static constexpr NumericType Lowest()
	{
		return Min();
	}
};


template<>
struct TNumericLimits<uint32>
{
	typedef uint32 NumericType;

	static constexpr NumericType Min()
	{
		return MIN_uint32;
	}

	static constexpr NumericType Max()
	{
		return MAX_uint32;
	}

	static constexpr NumericType Lowest()
	{
		return Min();
	}
};


template<>
struct TNumericLimits<uint64>
{
	typedef uint64 NumericType;

	static constexpr NumericType Min()
	{
		return MIN_uint64;
	}

	static constexpr NumericType Max()
	{
		return MAX_uint64;
	}

	static constexpr NumericType Lowest()
	{
		return Min();
	}
};


template<>
struct TNumericLimits<int8>
{
	typedef int8 NumericType;

	static constexpr NumericType Min()
	{
		return MIN_int8;
	}

	static constexpr NumericType Max()
	{
		return MAX_int8;
	}

	static constexpr NumericType Lowest()
	{
		return Min();
	}
};


template<>
struct TNumericLimits<int16>
{
	typedef int16 NumericType;

	static constexpr NumericType Min()
	{
		return MIN_int16;
	}

	static constexpr NumericType Max()
	{
		return MAX_int16;
	}

	static constexpr NumericType Lowest()
	{
		return Min();
	}
};


template<>
struct TNumericLimits<int32>
{
	typedef int32 NumericType;

	static constexpr NumericType Min()
	{
		return MIN_int32;
	}

	static constexpr NumericType Max()
	{
		return MAX_int32;
	}

	static constexpr NumericType Lowest()
	{
		return Min();
	}
};


template<>
struct TNumericLimits<int64>
{
	typedef int64 NumericType;

	static constexpr NumericType Min()
	{
		return MIN_int64;
	}

	static constexpr NumericType Max()
	{
		return MAX_int64;
	}

	static constexpr NumericType Lowest()
	{
		return Min();
	}
};


template<>
struct TNumericLimits<float>
{
	typedef float NumericType;

	static constexpr NumericType Min()
	{
		return MIN_flt;
	}

	static constexpr NumericType Max()
	{
		return MAX_flt;
	}

	static constexpr NumericType Lowest()
	{
		return -Max();
	}
};


template<>
struct TNumericLimits<double>
{
	typedef double NumericType;

	static constexpr NumericType Min()
	{
		return MIN_dbl;
	}

	static constexpr NumericType Max()
	{
		return MAX_dbl;
	}

	static constexpr NumericType Lowest()
	{
		return -Max();
	}
};


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

#ifndef IE_BUILD_DEVELOPMENT
#define IE_BUILD_DEVELOPMENT 1
#endif

#ifndef IE_BUILD_SHIPPING
#define IE_BUILD_SHIPPING 0
#endif

#ifndef PLATFORM_LITTLE_ENDIAN
#define PLATFORM_LITTLE_ENDIAN 0
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
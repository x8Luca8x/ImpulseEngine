#pragma once

#include "CoreModule.h"
#include "Misc/Char.h"
#include "Math/Math.h"

#include "Memory/Memory.h"

#include "Templates/ImpulseTemplates.h"
#include "Templates/AreTypesEqual.h"

#include "Debug/ImpulseDebug.h"

#include <memory>
#include <cstdarg>
#include <string>

enum class ESearchCase : uint8
{
	CaseSensitive,
	IgnoreCase
};

template<typename NumType>
struct TFloatingPointIntegralEquivalent
{
	static_assert(TIsFloat<NumType>::Value, "NumType must be a floating point type");

	using Type = NumType;
};

template<>
struct TFloatingPointIntegralEquivalent<float>
{
	using Type = int32;
};

template<>
struct TFloatingPointIntegralEquivalent<double>
{
	using Type = int64;
};

template<typename T>
class TPlatformString
{

	static_assert(ARE_TYPES_EQUAL(T, ANSICHAR) || ARE_TYPES_EQUAL(T, WIDECHAR) || ARE_TYPES_EQUAL(T, UTF8CHAR) || ARE_TYPES_EQUAL(T, UTF16CHAR) || ARE_TYPES_EQUAL(T, UTF32CHAR), "TPlatformString can only be used with ANSICHAR, WIDECHAR, UTF8CHAR, UTF16CHAR or UTF32CHAR");

public:

	// Determines whether the specified string is null or an Empty string.
	static bool IsNullOrEmpty(const T* String, int32 Count);

	// Determines whether the specified string is null, empty, or consists only of white-space characters.
	static bool IsNullOrWhiteSpace(const T* String, int32 Count);

	// Determine the length of the string.
	static int32 Strlen(const T* String);

	// Copies the specified string into a buffer.
	static void Strcpy(T* Dest, int32 DestCount, const T* Source);

	// Copies the specified string into a buffer.
	static void Strncpy(T* Dest, int32 DestCount, const T* Source, int32 MaxCount);

	// Concatenates two specified instances of String.
	static void Strcat(T* Dest, int32 DestCount, const T* Source);

	// Concatenates two specified instances of String.
	static void Strncat(T* Dest, int32 DestCount, const T* Source, int32 MaxCount);

	// Compares two specified String objects and returns an integer that indicates their relative position in the sort order.
	static int32 Strcmp(const T* String1, const T* String2);

	// Compares two specified String objects and returns an integer that indicates their relative position in the sort order.
	static int32 Strncmp(const T* String1, const T* String2, int32 MaxCount);

	// Compares two specified String objects and returns an integer that indicates their relative position in the sort order.
	static int32 Stricmp(const T* String1, const T* String2);

	// Compares two specified String objects and returns an integer that indicates their relative position in the sort order.
	static int32 Strnicmp(const T* String1, const T* String2, int32 MaxCount);

	static int32 Strcmp(const T* String1, const T* String2, int32 MinLength);
	static int32 Stricmp(const T* String1, const T* String2, int32 MinLength);

	// Formats a string.
	static int32 Sprintf(T* const Dest, int32 DestCount, const T* Format, ...);

	static int32 Vsnprintf(T* const Dest, int32 DestCount, const T* Format, va_list Args);

	// Helper function to compare two strings.
	static bool Compare(const T* String1, const T* String2, ESearchCase SearchCase = ESearchCase::CaseSensitive);
	// Helper function to compare two strings.
	static bool Compare(const T* String1, const T* String2, int32 MinLength, ESearchCase SearchCase = ESearchCase::CaseSensitive);

	static uint32 Strihash(const T* String, int32 Length);

	/**
	* Converts a string to a boolean.
	* @param String - The string to convert
	* @param OutBool - Will contain the converted boolean
	* @return Whether the conversion was successful
	*/
	static bool ToBool(const T* String, int32 Length, bool& OutBool);

	/**
	* Converts a string to a number.
	* @param String - The string to convert
	* @param OutNumber - Will contain the converted number
	* @return Whether the conversion was successful
	*/
	static bool ToInt8(const T* String, int32 Length, int8& OutNumber);

	/**
	* Converts a string to a number.
	* @param String - The string to convert
	* @param OutNumber - Will contain the converted number
	* @return Whether the conversion was successful
	*/
	static bool ToInt16(const T* String, int32 Length, int16& OutNumber);

	/**
	* Converts a string to a number.
	* @param String - The string to convert
	* @param OutNumber - Will contain the converted number
	* @return Whether the conversion was successful
	*/
	static bool ToInt32(const T* String, int32 Length, int32& OutNumber);

	/**
	* Converts a string to a number.
	* @param String - The string to convert
	* @param OutNumber - Will contain the converted number
	* @return Whether the conversion was successful
	*/
	static bool ToInt64(const T* String, int32 Length, int64& OutNumber);

	/**
	* Converts a string to a number.
	* @param String - The string to convert
	* @param OutNumber - Will contain the converted number
	* @return Whether the conversion was successful
	*/
	static bool ToUInt8(const T* String, int32 Length, uint8& OutNumber);

	/**
	* Converts a string to a number.
	* @param String - The string to convert
	* @param OutNumber - Will contain the converted number
	* @return Whether the conversion was successful
	*/
	static bool ToUInt16(const T* String, int32 Length, uint16& OutNumber);

	/**
	* Converts a string to a number.
	* @param String - The string to convert
	* @param OutNumber - Will contain the converted number
	* @return Whether the conversion was successful
	*/
	static bool ToUInt32(const T* String, int32 Length, uint32& OutNumber);

	/**
	* Converts a string to a number.
	* @param String - The string to convert
	* @param OutNumber - Will contain the converted number
	* @return Whether the conversion was successful
	*/
	static bool ToUInt64(const T* String, int32 Length, uint64& OutNumber);

	/**
	* Converts a string to a floating number.
	* @param String - The string to convert
	* @param OutNumber - Will contain the converted number
	* @return Whether the conversion was successful
	*/
	static bool ToFloat(const T* String, int32 Length, float& OutNumber);

	/**
	* Converts a string to a number.
	* @param String - The string to convert
	* @param OutNumber - Will contain the converted number
	* @return Whether the conversion was successful
	*/
	static bool ToDouble(const T* String, int32 Length, double& OutNumber);

	/**
	* Reverses a string.
	* @param String - The string to reverse
	* @param Count - Size of the string
	*/
	static void Reverse(T* String, int32 Count)
	{
		for (int32 i = 0; i < Count / 2; ++i)
		{
			T Temp = String[i];

			String[i] = String[Count - i - 1];
			String[Count - i - 1] = Temp;
		}
	}

public:

	template<typename NumType>
	static bool ToIntegral(const T* String, int32 Length, NumType& OutNumber);
	template<typename NumType>
	static bool ToFloating(const T* String, int32 Length, NumType& OutNumber);

	template<typename NumType>
	static bool FromIntegral(NumType Number, T* Dest, int32 DestCount);
	template<typename NumType>
	static bool FromFloating(NumType Number, T* Dest, int32 DestCount, int32 MinFractionalDigits = 10);
};

template<typename T>
inline bool TPlatformString<T>::IsNullOrEmpty(const T* String, int32 Count)
{
	return String == nullptr || Count == 0 || String[0] == '\0';
}

template<typename T>
inline bool TPlatformString<T>::IsNullOrWhiteSpace(const T* String, int32 Count)
{
	if (IsNullOrEmpty(String, Count))
		return true;

	for (int32 i = 0; i < Count; ++i)
	{
		if (!TChar<T>::IsWhiteSpace(String[i]))
			return false;
	}

	return true;
}

template<typename T>
inline int32 TPlatformString<T>::Strlen(const T* String)
{
	for (int32 i = 0;; ++i)
	{
		if (String[i] == '\0')
			return i;
	}

	// Unreachable

	return 0;
}

template<typename T>
inline void TPlatformString<T>::Strcpy(T* Dest, int32 DestCount, const T* Source)
{
	strcpy_s(Dest, DestCount, Source);
}

template<typename T>
inline void TPlatformString<T>::Strncpy(T* Dest, int32 DestCount, const T* Source, int32 MaxCount)
{
	strncpy_s(Dest, DestCount, Source, MaxCount);
}

template<typename T>
inline void TPlatformString<T>::Strcat(T* Dest, int32 DestCount, const T* Source)
{
	strcat_s(Dest, DestCount, Source);
}

template<typename T>
inline void TPlatformString<T>::Strncat(T* Dest, int32 DestCount, const T* Source, int32 MaxCount)
{
	strncat_s(Dest, DestCount, Source, MaxCount);
}

template<typename T>
inline int32 TPlatformString<T>::Strcmp(const T* String1, const T* String2)
{
	return strcmp(String1, String2);
}

template<typename T>
inline int32 TPlatformString<T>::Strncmp(const T* String1, const T* String2, int32 MaxCount)
{
	return strncmp(String1, String2, MaxCount);
}

template<typename T>
inline int32 TPlatformString<T>::Stricmp(const T* String1, const T* String2)
{
	PRAGMA_DISABLE_DEPRECATION_WARNINGS
	return stricmp(String1, String2);
	PRAGMA_ENABLE_DEPRECATION_WARNINGS
}

template<typename T>
inline int32 TPlatformString<T>::Strnicmp(const T* String1, const T* String2, int32 MaxCount)
{
	return strnicmp(String1, String2, MaxCount);
}

template<typename T>
inline int32 TPlatformString<T>::Strcmp(const T* String1, const T* String2, int32 MinLength)
{
	return FMemory::Memcmp(String1, String2, MinLength * sizeof(TCHAR));
}

template<typename T>
inline int32 TPlatformString<T>::Stricmp(const T* String1, const T* String2, int32 MinLength)
{
	return FMemory::Memicmp(String1, String2, MinLength * sizeof(TCHAR));
}

template<typename T>
inline int32 TPlatformString<T>::Sprintf(T* const Dest, int32 DestCount, const T* Format, ...)
{
	va_list Args;
	va_start(Args, Format);
	int32 Result = Vsnprintf(Dest, DestCount, Format, Args);
	va_end(Args);

	return Result;
}

template<typename T>
inline int32 TPlatformString<T>::Vsnprintf(T* const Dest, int32 DestCount, const T* Format, va_list Args)
{
	if constexpr (sizeof(T) == 1)
		return vsprintf_s((ANSICHAR* const)Dest, (size_t)DestCount, (const ANSICHAR*)Format, Args);
	else if constexpr (sizeof(T) == 2)
		return vswprintf_s((WIDECHAR* const)Dest, (size_t)DestCount, (const WIDECHAR*)Format, Args);
	else
		static_assert(sizeof(T) == 0, "Unsupported character type");
}

template<typename T>
inline bool TPlatformString<T>::Compare(const T* String1, const T* String2, ESearchCase SearchCase)
{
	if (SearchCase == ESearchCase::CaseSensitive)
		return Strcmp(String1, String2) == 0;
	else
		return Stricmp(String1, String2) == 0;
}

template<typename T>
inline bool TPlatformString<T>::Compare(const T* String1, const T* String2, int32 MinLength, ESearchCase SearchCase)
{
	if (SearchCase == ESearchCase::CaseSensitive)
		return Strcmp(String1, String2, MinLength) == 0;
	else
		return Stricmp(String1, String2, MinLength) == 0;
}

#pragma warning (disable : 4333)

template<typename T>
inline uint32 TPlatformString<T>::Strihash(const T* String, int32 Length)
{
	static uint32 CRCTable[256] =
	{
		0x00000000, 0x04C11DB7, 0x09823B6E, 0x0D4326D9, 0x130476DC, 0x17C56B6B, 0x1A864DB2, 0x1E475005, 0x2608EDB8, 0x22C9F00F, 0x2F8AD6D6, 0x2B4BCB61, 0x350C9B64, 0x31CD86D3, 0x3C8EA00A, 0x384FBDBD,
		0x4C11DB70, 0x48D0C6C7, 0x4593E01E, 0x4152FDA9, 0x5F15ADAC, 0x5BD4B01B, 0x569796C2, 0x52568B75, 0x6A1936C8, 0x6ED82B7F, 0x639B0DA6, 0x675A1011, 0x791D4014, 0x7DDC5DA3, 0x709F7B7A, 0x745E66CD,
		0x9823B6E0, 0x9CE2AB57, 0x91A18D8E, 0x95609039, 0x8B27C03C, 0x8FE6DD8B, 0x82A5FB52, 0x8664E6E5, 0xBE2B5B58, 0xBAEA46EF, 0xB7A96036, 0xB3687D81, 0xAD2F2D84, 0xA9EE3033, 0xA4AD16EA, 0xA06C0B5D,
		0xD4326D90, 0xD0F37027, 0xDDB056FE, 0xD9714B49, 0xC7361B4C, 0xC3F706FB, 0xCEB42022, 0xCA753D95, 0xF23A8028, 0xF6FB9D9F, 0xFBB8BB46, 0xFF79A6F1, 0xE13EF6F4, 0xE5FFEB43, 0xE8BCCD9A, 0xEC7DD02D,
		0x34867077, 0x30476DC0, 0x3D044B19, 0x39C556AE, 0x278206AB, 0x23431B1C, 0x2E003DC5, 0x2AC12072, 0x128E9DCF, 0x164F8078, 0x1B0CA6A1, 0x1FCDBB16, 0x018AEB13, 0x054BF6A4, 0x0808D07D, 0x0CC9CDCA,
		0x7897AB07, 0x7C56B6B0, 0x71159069, 0x75D48DDE, 0x6B93DDDB, 0x6F52C06C, 0x6211E6B5, 0x66D0FB02, 0x5E9F46BF, 0x5A5E5B08, 0x571D7DD1, 0x53DC6066, 0x4D9B3063, 0x495A2DD4, 0x44190B0D, 0x40D816BA,
		0xACA5C697, 0xA864DB20, 0xA527FDF9, 0xA1E6E04E, 0xBFA1B04B, 0xBB60ADFC, 0xB6238B25, 0xB2E29692, 0x8AAD2B2F, 0x8E6C3698, 0x832F1041, 0x87EE0DF6, 0x99A95DF3, 0x9D684044, 0x902B669D, 0x94EA7B2A,
		0xE0B41DE7, 0xE4750050, 0xE9362689, 0xEDF73B3E, 0xF3B06B3B, 0xF771768C, 0xFA325055, 0xFEF34DE2, 0xC6BCF05F, 0xC27DEDE8, 0xCF3ECB31, 0xCBFFD686, 0xD5B88683, 0xD1799B34, 0xDC3ABDED, 0xD8FBA05A,
		0x690CE0EE, 0x6DCDFD59, 0x608EDB80, 0x644FC637, 0x7A089632, 0x7EC98B85, 0x738AAD5C, 0x774BB0EB, 0x4F040D56, 0x4BC510E1, 0x46863638, 0x42472B8F, 0x5C007B8A, 0x58C1663D, 0x558240E4, 0x51435D53,
		0x251D3B9E, 0x21DC2629, 0x2C9F00F0, 0x285E1D47, 0x36194D42, 0x32D850F5, 0x3F9B762C, 0x3B5A6B9B, 0x0315D626, 0x07D4CB91, 0x0A97ED48, 0x0E56F0FF, 0x1011A0FA, 0x14D0BD4D, 0x19939B94, 0x1D528623,
		0xF12F560E, 0xF5EE4BB9, 0xF8AD6D60, 0xFC6C70D7, 0xE22B20D2, 0xE6EA3D65, 0xEBA91BBC, 0xEF68060B, 0xD727BBB6, 0xD3E6A601, 0xDEA580D8, 0xDA649D6F, 0xC423CD6A, 0xC0E2D0DD, 0xCDA1F604, 0xC960EBB3,
		0xBD3E8D7E, 0xB9FF90C9, 0xB4BCB610, 0xB07DABA7, 0xAE3AFBA2, 0xAAFBE615, 0xA7B8C0CC, 0xA379DD7B, 0x9B3660C6, 0x9FF77D71, 0x92B45BA8, 0x9675461F, 0x8832161A, 0x8CF30BAD, 0x81B02D74, 0x857130C3,
		0x5D8A9099, 0x594B8D2E, 0x5408ABF7, 0x50C9B640, 0x4E8EE645, 0x4A4FFBF2, 0x470CDD2B, 0x43CDC09C, 0x7B827D21, 0x7F436096, 0x7200464F, 0x76C15BF8, 0x68860BFD, 0x6C47164A, 0x61043093, 0x65C52D24,
		0x119B4BE9, 0x155A565E, 0x18197087, 0x1CD86D30, 0x029F3D35, 0x065E2082, 0x0B1D065B, 0x0FDC1BEC, 0x3793A651, 0x3352BBE6, 0x3E119D3F, 0x3AD08088, 0x2497D08D, 0x2056CD3A, 0x2D15EBE3, 0x29D4F654,
		0xC5A92679, 0xC1683BCE, 0xCC2B1D17, 0xC8EA00A0, 0xD6AD50A5, 0xD26C4D12, 0xDF2F6BCB, 0xDBEE767C, 0xE3A1CBC1, 0xE760D676, 0xEA23F0AF, 0xEEE2ED18, 0xF0A5BD1D, 0xF464A0AA, 0xF9278673, 0xFDE69BC4,
		0x89B8FD09, 0x8D79E0BE, 0x803AC667, 0x84FBDBD0, 0x9ABC8BD5, 0x9E7D9662, 0x933EB0BB, 0x97FFAD0C, 0xAFB010B1, 0xAB710D06, 0xA6322BDF, 0xA2F33668, 0xBCB4666D, 0xB8757BDA, 0xB5365D03, 0xB1F740B4
	};

	uint32 Hash = 0;
	for (int32 Idx = 0; Idx < Length; ++Idx)
	{
		T Ch = TChar<T>::ToUpper(*String++);
		uint16  B = static_cast<uint16>(Ch);
		Hash = ((Hash >> 8) & 0x00FFFFFF) ^ CRCTable[(Hash ^ B) & 0x000000FF];
		B = Ch >> 8;
		Hash = ((Hash >> 8) & 0x00FFFFFF) ^ CRCTable[(Hash ^ B) & 0x000000FF];
	}
	return Hash;
}

template<typename T>
inline bool TPlatformString<T>::ToBool(const T* String, int32 Length, bool& OutBool)
{
	if (Length == 1)
	{
		OutBool = String[0] == '1';
		return true;
	}

	if (Length == 4)
	{
		if (String[0] == 't' || String[0] == 'T')
		{
			if (String[1] == 'r' || String[1] == 'R')
			{
				if (String[2] == 'u' || String[2] == 'U')
				{
					if (String[3] == 'e' || String[3] == 'E')
					{
						OutBool = true;
						return true;
					}
				}
			}
		}
	}
	else if (Length == 5)
	{
		if (String[0] == 'f' || String[0] == 'F')
		{
			if (String[1] == 'a' || String[1] == 'A')
			{
				if (String[2] == 'l' || String[2] == 'L')
				{
					if (String[3] == 's' || String[3] == 'S')
					{
						if (String[4] == 'e' || String[4] == 'E')
						{
							OutBool = false;
							return true;
						}
					}
				}
			}
		}
	}

	return false;
}

template<typename T>
inline bool TPlatformString<T>::ToInt8(const T* String, int32 Length, int8& OutNumber)
{
	return ToIntegral<int8>(String, Length, OutNumber);
}

template<typename T>
inline bool TPlatformString<T>::ToInt16(const T* String, int32 Length, int16& OutNumber)
{
	return ToIntegral<int16>(String, Length, OutNumber);
}

template<typename T>
inline bool TPlatformString<T>::ToInt32(const T* String, int32 Length, int32& OutNumber)
{
	return ToIntegral<int32>(String, Length, OutNumber);
}

template<typename T>
inline bool TPlatformString<T>::ToInt64(const T* String, int32 Length, int64& OutNumber)
{
	return ToIntegral<int64>(String, Length, OutNumber);
}

template<typename T>
inline bool TPlatformString<T>::ToUInt8(const T* String, int32 Length, uint8& OutNumber)
{
	return ToIntegral<uint8>(String, Length, OutNumber);
}

template<typename T>
inline bool TPlatformString<T>::ToUInt16(const T* String, int32 Length, uint16& OutNumber)
{
	return ToIntegral<uint16>(String, Length, OutNumber);
}

template<typename T>
inline bool TPlatformString<T>::ToUInt32(const T* String, int32 Length, uint32& OutNumber)
{
	return ToIntegral<uint32>(String, Length, OutNumber);
}

template<typename T>
inline bool TPlatformString<T>::ToUInt64(const T* String, int32 Length, uint64& OutNumber)
{
	return ToIntegral<uint64>(String, Length, OutNumber);
}

template<typename T>
inline bool TPlatformString<T>::ToFloat(const T* String, int32 Length, float& OutNumber)
{
	return ToFloating<float>(String, Length, OutNumber);
}

template<typename T>
inline bool TPlatformString<T>::ToDouble(const T* String, int32 Length, double& OutNumber)
{
	return ToFloating<double>(String, Length, OutNumber);
}

typedef TPlatformString<ANSICHAR> FPlatformANSIString;
typedef TPlatformString<WIDECHAR> FPlatformWIDEString;
typedef TPlatformString<UTF8CHAR> FPlatformUTF8String;
typedef TPlatformString<UTF16CHAR> FPlatformUTF16String;
typedef TPlatformString<UTF32CHAR> FPlatformUTF32String;

typedef TPlatformString<TCHAR> FPlatformString;

inline uint32 GetTypeHash(const ANSICHAR* String, int32 Length)
{
	return FPlatformANSIString::Strihash(String, Length);
}

inline uint32 GetTypeHash(const WIDECHAR* String, int32 Length)
{
	return FPlatformWIDEString::Strihash(String, Length);
}

inline uint32 GetTypeHash(const UTF8CHAR* String, int32 Length)
{
	return FPlatformUTF8String::Strihash(String, Length);
}

inline uint32 GetTypeHash(const UTF16CHAR* String, int32 Length)
{
	return FPlatformUTF16String::Strihash(String, Length);
}

inline uint32 GetTypeHash(const UTF32CHAR* String, int32 Length)
{
	return FPlatformUTF32String::Strihash(String, Length);
}

template<typename T>
template<typename NumType>
inline bool TPlatformString<T>::ToIntegral(const T* String, int32 Length, NumType& OutNumber)
{
	static_assert(TIsIntegral<NumType>::Value, "NumType must be an integral type");

	if (Length == 0)
		return false;

	const bool bIsNegative = String[0] == '-';
	if constexpr (TIsUnsigned<NumType>::Value)
	{
		checkf(!bIsNegative, TEXT("Cannot convert a negative string to an unsigned number"));
	}

	T* strPtr = bIsNegative ? (T*)String + 1 : (T*)String;
	const T* endPtr = (const T*)String + Length;

	NumType result{};
	while (strPtr != endPtr)
	{
		if (!TChar<T>::IsDigit(*strPtr))
			return false;

		result = result * 10 + (*strPtr - '0');
		++strPtr;
	}

	if constexpr (TIsSigned<NumType>::Value)
		OutNumber = bIsNegative ? -result : result;
	else
		OutNumber = result;

	return true;
}

template<typename T>
template<typename NumType>
inline bool TPlatformString<T>::ToFloating(const T* String, int32 Length, NumType& OutNumber)
{
	static_assert(TIsFloat<NumType>::Value, "NumType must be a floating point type");

	if (Length == 0)
		return false;

	const bool bIsNegative = String[0] == '-';

	T* strPtr = bIsNegative ? (T*)String + 1 : (T*)String;
	const T* strEndPtr = String + Length;

	// Find dot

	int32 numberOfDigitsBeforeDot = 0;
	typename TFloatingPointIntegralEquivalent<NumType>::Type beforeDotResult{};

	while (strPtr != strEndPtr && *strPtr != '.')
	{
		if (!TChar<T>::IsDigit(*strPtr))
			return false;

		beforeDotResult = beforeDotResult * 10 + (*strPtr - '0');

		++numberOfDigitsBeforeDot;
		++strPtr;
	}

	// Check if we have a dot

	if (strPtr >= strEndPtr)
	{
		OutNumber = static_cast<NumType>(beforeDotResult);
		return true;
	}

	// Find digits after dot

	++strPtr;

	int32 numberOfDigitsAfterDot = 0;
	typename TFloatingPointIntegralEquivalent<NumType>::Type afterDotResult{};

	while (strPtr != strEndPtr)
	{
		if (!TChar<T>::IsDigit(*strPtr))
			return false;

		afterDotResult = afterDotResult * 10 + (*strPtr - '0');

		++numberOfDigitsAfterDot;
		++strPtr;
	}

	// Calculate result

	NumType result = static_cast<NumType>(beforeDotResult);
	NumType afterDot = static_cast<NumType>(afterDotResult);

	for (int32 i = 0; i < numberOfDigitsAfterDot; ++i)
		afterDot /= 10;

	result += afterDot;
	OutNumber = bIsNegative ? -result : result;

	return true;
}

template<typename T>
template<typename NumType>
inline bool TPlatformString<T>::FromIntegral(NumType Number, T* Dest, int32 DestCount)
{
	static_assert(TIsIntegral<NumType>::Value, "NumType must be an integral type");

	if (DestCount <= 0)
		return false;

	T* numberStart = Dest;

	if (Number < 0)
	{
		*Dest = '-';

		++Dest;
		++numberStart;
		--DestCount;
	}

	if constexpr (TIsSigned<T>::Value)
		Number = FMath::Abs(Number);

	while (DestCount - 1 > 0 && Number > 0)
	{
		*Dest = '0' + Number % 10;
		Number /= 10;

		++Dest;
		--DestCount;
	}

	if (Number > 0)
		return false;

	*Dest = '\0';
	Reverse(numberStart, static_cast<int32>(Dest - numberStart));

	return true;
}

template<typename T>
template<typename NumType>
inline bool TPlatformString<T>::FromFloating(NumType Number, T* Dest, int32 DestCount, int32 MinFractionalDigits)
{
	static_assert(TIsFloat<NumType>::Value, "NumType must be a floating point type");

	NumType floorVal = FMath::Floor(Number);
	NumType floatVal = Number - floorVal;

	{
		NumType floatFloor;
		do
		{

			floatVal *= NumType(10.0);
			floatFloor = FMath::Floor(floatVal);

		} while (floatVal > floatFloor);
	}

	T floorValStr[64];
	T floatValStr[64];

	if (!FromIntegral<typename TFloatingPointIntegralEquivalent<NumType>::Type>(static_cast<typename TFloatingPointIntegralEquivalent<NumType>::Type>(floorVal), floorValStr, 64))
		return false;
	if (!FromIntegral<typename TFloatingPointIntegralEquivalent<NumType>::Type>(static_cast<typename TFloatingPointIntegralEquivalent<NumType>::Type>(floatVal), floatValStr, 64))
		return false;

	int32 strFloorLen = Strlen(floorValStr);
	int32 strFloatLen = Strlen(floatValStr);

	int32 length = strFloorLen + (MinFractionalDigits > 0 && MinFractionalDigits < strFloatLen ? MinFractionalDigits : strFloatLen) + 1; // + 1 beacuse of the dot
	if (DestCount + 1 < length)
		return false;

	FMemory::Memcpy(Dest, floorValStr, static_cast<size_t>(strFloorLen) * sizeof(T));
	Dest[strFloorLen] = '.';
	FMemory::Memcpy(&Dest[strFloorLen + 1], floatValStr, static_cast<size_t>(MinFractionalDigits > 0 && MinFractionalDigits < strFloatLen ? MinFractionalDigits : strFloatLen) * sizeof(T));

	Dest[length] = '\0';
	return true;
}
#pragma once

#include "Memory/Memory.h"
#include "Templates/ImpulseTemplates.h"

#include "Platform/PlatformString.h"
#include "Containers/ImpulseString.h"

#include "ScopeLock.h"

#include "Containers/Array.h"

#define TCHAR_TO_ANSI(String) TStringConverter<TCHAR, ANSICHAR>(String)
#define ANSI_TO_TCHAR(String) TStringConverter<ANSICHAR, TCHAR>(String)

#define FSTRING_TO_ANSI(String) TStringConverter<TCHAR, ANSICHAR>(*String, static_cast<uint32>(String.Length()))

template<typename Left, typename Right>
struct FCanRawCastString
{
	static_assert(TIsCharType<Left>::Value, "Left type must be a character type");
	static_assert(TIsCharType<Right>::Value, "Right type must be a character type");

	enum { Value = sizeof(Left) == sizeof(Right) };
};

/**
* Helper class for converting between different string types
* Converted string are only valid as long as the source string is valid
*/
template<typename Source, typename Target>
class TStringConverter;

/**
* Helper class for converting between different string types
* Converted string are only valid as long as the source string is valid
*/
template<typename T>
class TStringConverter<T, T>
{
public:

	TStringConverter(const T* InString)
		: String(InString) {}

	T* Get() const
	{
		return const_cast<T*>(String);
	}

private:

	const T* String;
};

/**
* Helper class for converting between different string types
* Converted string are only valid as long as the source string is valid
*/
template<typename Source, typename Target>
class TStringConverter
{

	using FSourcePlatformString = TPlatformString<Source>;
	using FTargetPlatformString = TPlatformString<Target>;

public:

	TStringConverter() = delete;
	TStringConverter(const TStringConverter&) = delete;

	TStringConverter(const Source* InString)
		: TStringConverter(InString, FSourcePlatformString::Strlen(InString)) {}

	TStringConverter(const Source* InString, uint32 InLength)
		: SourceString(InString), Length(InLength) {
		Init();
	}

	~TStringConverter()
	{
		if (ConvertedString)
			FMemory::Free(ConvertedString);
	}

	TStringConverter& operator=(const TStringConverter&) = delete;

	Target* Get() const
	{
		return ConvertedString;
	}

	uint32 GetLength() const
	{
		return Length;
	}

	void Init()
	{
		if constexpr (FCanRawCastString<Source, Target>::Value)
			ConvertedString = reinterpret_cast<Target*>(SourceString);
		else
		{
			ConvertedString = (Target*)FMemory::Malloc((Length + 1) * sizeof(Target));

			Target* str = ConvertedString;
			const Target* end = ConvertedString + Length;

			while (str != end)
				*(str++) = static_cast<Target>(*(SourceString++));

			ConvertedString[Length] = (Target)'\0';
		}
	}

	void Detach()
	{
		ConvertedString = nullptr;
	}

private:

	Target* ConvertedString = nullptr;

	uint32 Length = 0;
	const Source* SourceString;
};

// -------------------------------------------------------------------------------------------------
// New string conversion functions
// -------------------------------------------------------------------------------------------------

/**
* Saves all string casted strings
* Needs to be cleared at some point
*/
class CORE_API FStringCastRegistry
{
public:

	/**
	* Adds a string conversion to the registry
	* @param InConverter - The string converter to add
	*/
	template<typename Source, typename Target>
	static void Add(TStringConverter<Source, Target>& InConverter)
	{
		if (FCanRawCastString<Source, Target>::Value)
			return; // No need to save the conversion

		Add(InConverter.Get());
		InConverter.Detach(); // Detach so the string is not freed when the converter is destroyed
	}

	/**
	* Adds a string to the registry
	* @param InString - The string to add
	*/
	static void Add(void* InString);

	/**
	* Clears the registry
	*/
	static void Clear();

private:

	static TArray<void*> CastedStrings;
	static FCriticalSection CriticalSection;
};

template<typename Target, typename Source>
Target* StringCast(const Source* InString)
{
	auto converter = TStringConverter<Source, Target>(InString);
	Target* result = converter.Get();

	FStringCastRegistry::Add(converter);
	return result;
}

template<typename Target, typename Source>
Target* StringCast(const Source* InString, uint32 InLength)
{
	auto converter = TStringConverter<Source, Target>(InString, InLength);
	Target* result = converter.Get();

	FStringCastRegistry::Add(converter);
	return result;
}

template<typename Target>
Target* StringCast(const FString& InString)
{
	auto converter = TStringConverter<TCHAR, Target>(*InString, InString.Length());
	Target* result = converter.Get();

	FStringCastRegistry::Add(converter);
	return result;
}
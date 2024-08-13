#pragma once

#include "Array.h"
#include "Platform/PlatformString.h"
#include "Serialization/Archive.h"

#include "Misc/Char.h"

#define STRING_PRINTF_BUFFER_SIZE 512

enum class ESearchDir : uint8
{
	FromStart,
	FromEnd
};

class CORE_API FString
{
	using FPlatformString = TPlatformString<TCHAR>;

public:

	FString() = default;
	FString(const FString&) = default;
	FString(FString&& Other) noexcept;
	FString(const TCHAR* InData);

	FString& operator=(const FString&) = default;
	FString& operator=(FString&& Other) noexcept;
	FString& operator=(const TCHAR* InData);

	~FString() = default;

	// @return The number of characters that are in the string
	int32 Length() const;

	// @return The number of characters that can be stored in the string
	int32 Size() const;

	// @param InIndex - Index where the substring should end
	// @return The characters on the left side of the specified index (Index not included)
	FString Left(int32 InIndex) const;

	// @param InIndex - Index where the substring should start
	// @return The characters on the right side of the specified index (Index not included)
	FString Right(int32 InIndex) const;

	// @param InStartIndex - Index where the substring should start
	// @param InLength - The number of characters to include in the substring
	// @return The characters in the specified range
	FString Mid(int32 InStartIndex, int32 InLength) const;

	// Finds the first occurrence of the specified substring
	// @param InSubStr - The substring to search for
	// @param InSearchCase - Whether or not the search should be case sensitive
	// @param InSearchDir - The direction to search in
	// @param InStartIndex - The index to start relative searching from
	// @return The index of the first occurrence of the specified character or -1 if not found
	int32 Find(const FString& InSubStr, ESearchCase InSearchCase = ESearchCase::CaseSensitive, ESearchDir InSearchDir = ESearchDir::FromStart, int32 InStartIndex = 0) const;

	// Finds all occurrences of the specified substring
	// @param InSubStr - The substring to search for
	// @param InSearchCase - Whether or not the search should be case sensitive
	// @param InSearchDir - The direction to search in
	// @param InStartIndex - The index to start relative searching from
	// @return The indices of all occurrences of the specified substring
	TArray<int32> FindAll(const FString& InSubStr, ESearchCase InSearchCase = ESearchCase::CaseSensitive, ESearchDir InSearchDir = ESearchDir::FromStart, int32 InStartIndex = 0) const;

	// Checks whether or not the string contains the specified substring
	// @param InSubStr - The substring to search for
	// @param InSearchCase - Whether or not the search should be case sensitive
	// @return True if the string contains the specified substring, false otherwise
	bool Contains(const FString& InSubStr, ESearchCase InSearchCase = ESearchCase::CaseSensitive) const;

	/**
	* Checks whether or not the string contains the specified character
	* @param InChar - The character to search for
	* @param InSearchCase - Whether or not the search should be case sensitive
	* @return True if the string contains the specified character, false otherwise
	*/
	bool ContainsChar(TCHAR InChar, ESearchCase InSearchCase = ESearchCase::CaseSensitive) const;

	// Checks whether or not the string starts with the specified substring
	// @param InSubStr - The substring to search for
	// @param InSearchCase - Whether or not the search should be case sensitive
	// @return True if the string starts with the specified substring, false otherwise
	bool StartsWith(const FString& InSubStr, ESearchCase InSearchCase = ESearchCase::CaseSensitive) const;

	// Checks whether or not the string ends with the specified substring
	// @param InSubStr - The substring to search for
	// @param InSearchCase - Whether or not the search should be case sensitive
	// @return True if the string ends with the specified substring, false otherwise
	bool EndsWith(const FString& InSubStr, ESearchCase InSearchCase = ESearchCase::CaseSensitive) const;

	// Checks whether two strings are equal
	// @param InOther - The string to compare to
	// @param InSearchCase - Whether or not the comparison should be case sensitive
	// @return True if the strings are equal, false otherwise
	bool Equals(const FString& InOther, ESearchCase InSearchCase = ESearchCase::CaseSensitive) const;

	// Checks whether or not the string is null or empty
	// @return True if the string is null or empty, false otherwise
	bool IsEmpty() const;

	/**
	* Empties the string
	*/
	void Empty();

	// Checks whether or not the string is null, empty or consists only of white-space characters
	// @return True if the string is null, empty or consists only of white-space characters, false otherwise
	bool IsWhiteSpace() const;

	// Trims the beginning of the string
	// @param InTrimChars - The characters to trim
	void InlineTrimStart(const TCHAR* InTrimChars = nullptr);

	// Trims the end of the string
	// @param InTrimChars - The characters to trim
	void InlineTrimEnd(const TCHAR* InTrimChars = nullptr);

	// Trims the beginning and end of the string
	// @param InTrimChars - The characters to trim
	void InlineTrim(const TCHAR* InTrimChars = nullptr);

	// Trims the beginning of the string
	// @param InTrimChars - The characters to trim
	// @return A new string with the specified characters trimmed from the beginning
	FString TrimStart(const TCHAR* InTrimChars = nullptr) const;

	// Trims the end of the string
	// @param InTrimChars - The characters to trim
	// @return A new string with the specified characters trimmed from the end
	FString TrimEnd(const TCHAR* InTrimChars = nullptr) const;

	// Trims the beginning and end of the string
	// @param InTrimChars - The characters to trim
	// @return A new string with the specified characters trimmed from the beginning and end
	FString Trim(const TCHAR* InTrimChars = nullptr) const;

	// Appends the specified string to the end of the string
	// @param InOther - The string to append
	void InlineAppend(const FString& InOther);

	// Appends the specified string to the end of the string
	// @param InOther - The string to append
	// @return A new string with the specified string appended to the end
	FString Append(const FString& InOther) const;

	// Appends the specified character to the end of the string
	// @param InChar - The character to append
	void InlineAppendChar(TCHAR InChar);

	// Appends the specified character to the end of the string
	// @param InChar - The character to append
	// @return A new string with the specified character appended to the end
	FString AppendChar(TCHAR InChar) const;

	FORCEINLINE TCHAR* operator*() { return m_Data.GetData(); }
	FORCEINLINE const TCHAR* operator*() const { return m_Data.GetData(); }

	FORCEINLINE TCHAR& operator[](int32 InIndex) { return m_Data[InIndex]; }
	FORCEINLINE const TCHAR& operator[](int32 InIndex) const { return m_Data[InIndex]; }

	FORCEINLINE bool operator==(const FString& InOther) const { return Equals(InOther); }
	FORCEINLINE bool operator!=(const FString& InOther) const { return !Equals(InOther); }

	FORCEINLINE friend FString& operator+=(FString& Lhs, const FString& Rhs) { Lhs.InlineAppend(Rhs); return Lhs; }
	FORCEINLINE friend FString operator+(const FString& Lhs, const FString& Rhs) { return Lhs.Append(Rhs); }

	FORCEINLINE friend FString& operator+=(FString& Lhs, TCHAR Rhs) { Lhs.InlineAppendChar(Rhs); return Lhs; }
	FORCEINLINE friend FString operator+(const FString& Lhs, TCHAR Rhs) { return Lhs.AppendChar(Rhs); }

	FORCEINLINE friend bool operator==(const TCHAR* Lhs, const FString& Rhs) { return Rhs.Equals(Lhs); }
	FORCEINLINE friend bool operator==(const FString& Lhs, const TCHAR* Rhs) { return Lhs.Equals(Rhs); }

	FORCEINLINE friend bool operator!=(const TCHAR* Lhs, const FString& Rhs) { return !Rhs.Equals(Lhs); }
	FORCEINLINE friend bool operator!=(const FString& Lhs, const TCHAR* Rhs) { return !Lhs.Equals(Rhs); }

public:

	/**
	* Formats the string using the specified format string and arguments
	* @param InFormat - The format string
	* @param ... - The arguments
	*/
	static FString Printf(const TCHAR* InFormat, ...);

public:

	/**
	* Converts the string to an integer
	* @param OutValue - The integer value
	* @return True if the conversion was successful, false otherwise
	*/
	bool ToInt8Save(int8& OutValue) const;

	/**
	* Converts the string to an integer
	* @param OutValue - The integer value
	* @return True if the conversion was successful, false otherwise
	*/
	bool ToInt16Save(int16& OutValue) const;

	/**
	* Converts the string to an integer
	* @param OutValue - The integer value
	* @return True if the conversion was successful, false otherwise
	*/
	bool ToInt32Save(int32& OutValue) const;

	/**
	* Converts the string to an integer
	* @param OutValue - The integer value
	* @return True if the conversion was successful, false otherwise
	*/
	bool ToInt64Save(int64& OutValue) const;

	/**
	* Converts the string to an integer
	* @param OutValue - The integer value
	* @return True if the conversion was successful, false otherwise
	*/
	bool ToUInt8Save(uint8& OutValue) const;

	/**
	* Converts the string to an integer
	* @param OutValue - The integer value
	* @return True if the conversion was successful, false otherwise
	*/
	bool ToUInt16Save(uint16& OutValue) const;

	/**
	* Converts the string to an integer
	* @param OutValue - The integer value
	* @return True if the conversion was successful, false otherwise
	*/
	bool ToUInt32Save(uint32& OutValue) const;

	/**
	* Converts the string to an integer
	* @param OutValue - The integer value
	* @return True if the conversion was successful, false otherwise
	*/
	bool ToUInt64Save(uint64& OutValue) const;

	/**
	* Converts the string to a float
	* @param OutValue - The integer value
	* @return True if the conversion was successful, false otherwise
	*/
	bool ToFloatSave(float& OutValue) const;

	/**
	* Converts the string to an double
	* @param OutValue - The integer value
	* @return True if the conversion was successful, false otherwise
	*/
	bool ToDoubleSave(double& OutValue) const;

public:

	/**
	* Converts the string to an integer
	* @return The integer value
	*/
	int8 ToInt8() const;

	/**
	* Converts the string to an integer
	* @return The integer value
	*/
	int16 ToInt16() const;

	/**
	* Converts the string to an integer
	* @return The integer value
	*/
	int32 ToInt32() const;

	/**
	* Converts the string to an integer
	* @return The integer value
	*/
	int64 ToInt64() const;

	/**
	* Converts the string to an integer
	* @return The integer value
	*/
	uint8 ToUInt8() const;

	/**
	* Converts the string to an integer
	* @return The integer value
	*/
	uint16 ToUInt16() const;

	/**
	* Converts the string to an integer
	* @return The integer value
	*/
	uint32 ToUInt32() const;

	/**
	* Converts the string to an integer
	* @return The integer value
	*/
	uint64 ToUInt64() const;

	/**
	* Converts the string to a float
	* @return The integer value
	*/
	float ToFloat() const;

	/**
	* Converts the string to a float
	* @return The integer value
	*/
	double ToDouble() const;

public:

	/**
	* Makes a string from an integer
	* @param InValue - The integer value
	* @return The string
	*/
	static FString FromInt8(int8 InValue);

	/**
	* Makes a string from an integer
	* @param InValue - The integer value
	* @return The string
	*/
	static FString FromInt16(int16 InValue);

	/**
	* Makes a string from an integer
	* @param InValue - The integer value
	* @return The string
	*/
	static FString FromInt32(int32 InValue);

	/**
	* Makes a string from an integer
	* @param InValue - The integer value
	* @return The string
	*/
	static FString FromInt64(int64 InValue);

	/**
	* Makes a string from an integer
	* @param InValue - The integer value
	* @return The string
	*/
	static FString FromUInt8(uint8 InValue);

	/**
	* Makes a string from an integer
	* @param InValue - The integer value
	* @return The string
	*/
	static FString FromUInt16(uint16 InValue);

	/**
	* Makes a string from an integer
	* @param InValue - The integer value
	* @return The string
	*/
	static FString FromUInt32(uint32 InValue);

	/**
	* Makes a string from an integer
	* @param InValue - The integer value
	* @return The string
	*/
	static FString FromUInt64(uint64 InValue);

	/**
	* Makes a string from a float
	* @param InValue - The float value
	* @return The string
	*/
	static FString FromFloat(float InValue, int32 MinFractionalDigits = 10);

	/**
	* Makes a string from a float
	* @param InValue - The float value
	* @return The string
	*/
	static FString FromDouble(double InValue, int32 MinFractionalDigits = 10);

public:

	/**
	* Serializes a string.
	* String will be encrypted by default.
	* To disable encryption, you have to rebuild the engine with the IE_DISABLE_STRING_ENCRYPTION macro defined.
	*/
	friend CORE_API FArchive& operator<<(FArchive& Ar, FString& Value);

public:

	// Default trim characters used by TrimStart, TrimEnd and Trim if no trim characters are specified
	static FString DefaultTrimChars;

private:

	// Resizes the string to the specified number of characters
	// @param InNum The number of characters to resize the string to without null terminator
	void Allocate(int32 InNum);

	void Copy(const TCHAR* InData, int32 InNum);

	int32 FindForward(const TCHAR* InSubStr, int32 InSubStrLength, int32 InStartIndex, ESearchCase SearchCase) const;
	int32 FindBackward(const TCHAR* InSubStr, int32 InSubStrLength, int32 InStartIndex, ESearchCase SearchCase) const;

	int32 m_Length = 0;
	TArray<TCHAR> m_Data;
};

inline uint32 GetTypeHash(const FString& InString)
{
	return GetTypeHash(*InString, InString.Length());
}
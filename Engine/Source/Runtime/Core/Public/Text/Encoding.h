#pragma once

#include "CoreMinimal.h"

enum class EEncoding : uint8
{
	None = 0,
	UTF8 = 1,
	UTF16 = 2,
	UTF32 = 4
};

class CORE_API FEncoding
{
public:

	/**
	* Makes a string from a byte array.
	* @param Bytes - The byte array to make a string from (Must not be null terminated)
	* @return The string
	*/
	static FString FromBytes(const TArray<uint8>& Bytes);

	/**
	* Makes a byte array from a string.
	* @param String - The string to make a byte array from
	* @return The byte array
	*/
	static TArray<uint8> ToBytes(const FString& String);

public:

	/**
	* Makes a string from a byte array.
	* @param Bytes - The byte array to make a string from (Must not be null terminated)
	* @Encoding - The encoding to use
	* @return A pointer to the string (Must be freed via FMemory::Free)
	*/
	template<EEncoding Encoding>
	static void* FromBytes(const TArray<uint8>& Bytes);
};

template<EEncoding Encoding>
inline void* FEncoding::FromBytes(const TArray<uint8>& Bytes)
{
	constexpr size_t encodingSize = sizeof(uint8[static_cast<uint8>(Encoding)]);

	constexpr int32 length = sizeof(encodingSize) / sizeof(uint8);
}
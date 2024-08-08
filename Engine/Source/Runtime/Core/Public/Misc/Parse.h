#pragma once

#include "Definitions.h"

#include "Containers/Array.h"
#include "Containers/ImpulseString.h"

class CORE_API FParse final
{
public:

	/**
	* Parses a string into an array of tokens.
	* @param InStr - The string to parse.
	* @param OutTokens - Will hold the parsed tokens.
	* @param bInAllowEmptyTokens - If true, empty tokens will be added to OutTokens.
	* @param bInKeepQuotes - If true, tokens surrounded by quotes will be kept as a single token.
	* @param InDelim - The delimiter to use when parsing the string.
	* @param InQuote - The quote character to use when parsing the string.
	* @return The number of tokens added to OutTokens.
	*/
	static int32 Tokenize(const FString& InStr, TArray<FString>& OutTokens, bool bInAllowEmptyTokens = false, bool bInKeepQuotes = false, TCHAR InDelim = TEXT(','), TCHAR InQuote = TEXT('\"'));

	/**
	* Parses a key/value pair from a string.
	* @param InStr - The string to parse.
	* @param OutKey - Will hold the parsed key.
	* @param OutValue - Will hold the parsed value.
	* @return true if the key/value pair was parsed successfully, false otherwise.
	*/
	static bool KeyValue(const FString& InStr, FString& OutKey, FString& OutValue);

	/**
	* Parses a value from a string.
	* @param InStr - The string to parse.
	* @param Key - The key to look for in the string.
	* @param OutValue - Will hold the parsed value.
	* @param SearchCase - Whether or not to perform a case-sensitive search.
	* @return true if the value was parsed successfully, false otherwise.
	*/
	static bool Value(const FString& InStr, const FString& Key, FString& OutValue, ESearchCase SearchCase = ESearchCase::CaseSensitive);

	/**
	* Checks if a string contains a parameter. Every parameter must start with a '-'.
	* @param InStr - The string to check.
	* @param Key - The key to look for in the string.
	* @param SearchCase - Whether or not to perform a case-sensitive search.
	*/
	static bool Param(const FString& InStr, const FString& Key, ESearchCase SearchCase = ESearchCase::CaseSensitive);
};
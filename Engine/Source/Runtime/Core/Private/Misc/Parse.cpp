#include "Misc/Parse.h"

int32 FParse::Tokenize(const FString& InStr, TArray<FString>& OutTokens, bool bInAllowEmptyTokens, bool bInKeepQuotes, TCHAR InDelim, TCHAR InQuote)
{
	OutTokens.Empty();

	const int32 StrLen = InStr.Length();
	if (StrLen == 0)
	{
		return 0;
	}

	FString Token;
	bool bInQuotes = false;
	for (int32 CharIndex = 0; CharIndex < StrLen; ++CharIndex)
	{
		const TCHAR Char = InStr[CharIndex];
		if (Char == InQuote)
		{
			if (bInKeepQuotes)
			{
				Token.InlineAppendChar(Char);
			}
			bInQuotes = !bInQuotes;
		}
		else if (Char == InDelim && !bInQuotes)
		{
			if (Token.Length() > 0 || bInAllowEmptyTokens)
			{
				OutTokens.Add(MoveTemp(Token));
			}
		}
		else
		{
			Token.InlineAppendChar(Char);
		}
	}

	if (Token.Length() > 0 || bInAllowEmptyTokens)
	{
		OutTokens.Add(MoveTemp(Token));
	}

	return OutTokens.Num();
}

bool FParse_Value_Parse_Value(const FString& InStr, int32 InEqualsIndex, FString& OutValue)
{
	int32 startIndex = InEqualsIndex + 1;

	// Find the start of the value

	for (int32 i = startIndex; i < InStr.Length(); ++i)
	{
		if (!TChar<TCHAR>::IsWhiteSpace(InStr[i]))
		{
			startIndex = i;
			break;
		}
	}

	if (InStr[startIndex] == '\"')
	{
		for (int32 i = startIndex + 1; i < InStr.Length(); ++i)
		{
			if (InStr[i] == '\"')
			{
				OutValue = InStr.Mid(startIndex + 1, i - startIndex - 1);
				return true;
			}
		}

		// No closing quote

		return false;
	}
	else
	{
		for (int32 i = startIndex; i < InStr.Length(); ++i)
		{
			if (TChar<TCHAR>::IsWhiteSpace(InStr[i]))
			{
				OutValue = InStr.Mid(startIndex, i - startIndex);
				return true;
			}
		}

		OutValue = InStr.Mid(startIndex, InStr.Length() - startIndex);
		return true;
	}
}

bool FParse::KeyValue(const FString& InStr, FString& OutKey, FString& OutValue)
{
	int32 equalsIndex = INDEX_NONE;
	for (int32 i = 0; i < InStr.Length(); ++i)
	{
		if (InStr[i] == TEXT('='))
		{
			equalsIndex = i;
			break;
		}
	}

	if (equalsIndex == INDEX_NONE || equalsIndex == 0)
		return false;

	OutKey = InStr.Mid(0, equalsIndex);
	return FParse_Value_Parse_Value(InStr, equalsIndex, OutValue);
}

bool FParse::Value(const FString& InStr, const FString& Key, FString& OutValue, ESearchCase SearchCase)
{
	int32 keyIndex = INDEX_NONE;
	for (int32 i = 0; i <= InStr.Length() - Key.Length(); ++i)
	{
		if (!TChar<TCHAR>::IsWhiteSpace(InStr[i]) && FPlatformString::Compare(&InStr[i], *Key, Key.Length(), SearchCase))
		{
			keyIndex = i;
			break;
		}
	}

	if (keyIndex == INDEX_NONE)
		return false;

	int32 equalsIndex = INDEX_NONE;
	for (int32 i = keyIndex + Key.Length(); i < InStr.Length(); ++i)
	{
		if (InStr[i] == TEXT('='))
		{
			equalsIndex = i;
			break;
		}
	}

	if (equalsIndex == INDEX_NONE)
		return false;

	return FParse_Value_Parse_Value(InStr, equalsIndex, OutValue);
}

bool FParse::Param(const FString& InStr, const FString& Key, ESearchCase SearchCase)
{
	for(int32 i = 0; i <= InStr.Length() - Key.Length(); ++i)
	{
		const TCHAR character = InStr[i];
		if (TChar<TCHAR>::IsWhiteSpace(character) || character != '-')
			continue;

		if (FPlatformString::Compare(&InStr[i + 1], *Key, Key.Length(), SearchCase))
			return true;

		i += Key.Length();
	}

	return false;
}
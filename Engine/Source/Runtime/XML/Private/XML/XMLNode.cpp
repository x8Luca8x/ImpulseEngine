#include "XML/XMLNode.h"

// NOT TESTED YET!
bool CompareXMLQueryString_WithWildcards(const FString& String, const FString& CompareWith, bool bCaseInsensitive)
{
	const bool bIgnoreBefore = CompareWith[0] == TEXT('%');
	const bool bIgnoreAfter = CompareWith[CompareWith.Length() - 1] == TEXT('%');
	const bool bIgnoreAny = bIgnoreBefore || bIgnoreAfter;

	FString compareString = CompareWith;

	if (bIgnoreBefore)
		compareString = compareString.Right(1);
	if (bIgnoreAfter)
		compareString = compareString.Left(compareString.Length() - 1);

	// If the string is empty, it means it's a wildcard, so it will always match.
	if (compareString.IsEmpty())
		return true;

	const int32 compareLength = compareString.Length();
	const int32 stringLength = String.Length();

	int32 compareIndex = 0;
	int32 stringIndex = 0;

	while (stringIndex < stringLength && compareIndex < compareLength)
	{
		const TCHAR compareChar = compareString[compareIndex];
		const TCHAR stringChar = String[stringIndex];

		if (compareChar == TEXT('*'))
		{
			// Ignore 1 character.
			compareIndex++;
			stringIndex++;
		}
		else if (compareChar == TEXT('%'))
		{
			// Ignore any characters before or after.
			if (bIgnoreBefore)
			{
				// Ignore any characters before.
				while (stringIndex < stringLength && String[stringIndex] != compareString[compareIndex])
				{
					stringIndex++;
				}
			}
			else if (bIgnoreAfter)
			{
				// Ignore any characters after.
				while (stringIndex < stringLength && String[stringIndex] != compareString[compareIndex])
				{
					stringIndex++;
				}
			}
			else
			{
				// Ignore any characters before or after.
				while (stringIndex < stringLength && String[stringIndex] != compareString[compareIndex])
				{
					stringIndex++;
				}
			}

			compareIndex++;
		}
		else if (bCaseInsensitive ? TChar<TCHAR>::ToLower(compareChar) == TChar<TCHAR>::ToLower(stringChar) : compareChar == stringChar)
		{
			compareIndex++;
			stringIndex++;
		}
		else
		{
			// Characters don't match.
			return false;
		}
	}

	// If the compare index is at the end of the compare string, it means it's a match.

	return compareIndex == compareLength;
}

bool CompareXMLQueryStrings(const FString& String, const FString& CompareWith, int32 Flags)
{
	FString compareString = CompareWith.Trim();
	if(compareString.IsEmpty())
		return false; // Empty string is not a valid query.

	const bool bHasWildcards = compareString.Contains(TEXT("*")) || compareString.Contains(TEXT("%")); // * ignores 1 character, % ignores any characters before or after.
	const bool bCaseInsensitive = (Flags & XQUERY_CaseInsensitive) != 0;

	if (bHasWildcards)
		return CompareXMLQueryString_WithWildcards(String, compareString, bCaseInsensitive);
	else
		return String.Equals(compareString, bCaseInsensitive ? ESearchCase::IgnoreCase : ESearchCase::CaseSensitive);

}

FXMLNodePtr FXMLNode::GetChild(const FXMLNodeQuery& Query) const
{
	return FXMLNodePtr();
}

TArray<FXMLNodePtr> FXMLNode::GetChildren(const FXMLNodeQuery& Query) const
{
	
}
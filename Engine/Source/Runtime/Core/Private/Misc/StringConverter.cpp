#include "Misc/StringConverter.h"

TArray<void*> FStringCastRegistry::CastedStrings;
FCriticalSection FStringCastRegistry::CriticalSection;

void FStringCastRegistry::Add(void* InString)
{
	FScopeLock Lock(&CriticalSection);
	CastedStrings.Add(InString);
}

void FStringCastRegistry::Clear()
{
	FScopeLock Lock(&CriticalSection);

	for (void* String : CastedStrings)
		FMemory::Free(String);

	CastedStrings.Empty();
}
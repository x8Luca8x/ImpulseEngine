#include "Containers/Name.h"

#include <Misc/StringConverter.h>

using FNameString = TPlatformString<ANSICHAR>;

// ~ Begin name definitions.

FName NAME_None;

FName NAME_Stream = FName("Stream");
FName NAME_Datagram = FName("Datagram");

// ~ End name definitions.

FNameEntry::FNameEntry()
{
	FMemory::Memset(m_Name, 0, sizeof(m_Name));
}

FNameEntry::FNameEntry(const ANSICHAR* InName)
{
	const int32 strLen = FMath::Min(FNameString::Strlen(InName), MAX_NAME_LENGTH);
	FMemory::Memcpy(m_Name, InName, strLen * sizeof(ANSICHAR));

	m_Name[strLen] = 0;
}

bool FNameEntry::IsNone() const
{
	return m_Name[0] == 0;
}

FString FNameEntry::ToString() const
{
	if (IsNone())
		return TEXT("None");

	auto converter = ANSI_TO_TCHAR(m_Name);
	return FString(converter.Get());
}

bool FNameEntry::operator==(const ANSICHAR* Other) const
{
	const int32 strLen = FNameString::Strlen(Other);
	return FMemory::Memcmp(m_Name, Other, FMath::Min(strLen, NAME_SIZE) * sizeof(ANSICHAR)) == 0;
}

FNameEntry* FNameRegistry::Find(const ANSICHAR* InName, ENameSearchMode SearchMode)
{
	FNameEntry entry(InName);

	TSetIterator<FNameEntry> it = m_Registry.Find(entry);
	if (!it.IsValid() && SearchMode == ENameSearchMode::FindOrAdd)
	{
		m_Registry.Add(entry);
		it = m_Registry.Find(entry);
	}

	return &*it;
}

FNameRegistry& FNameRegistry::GetGlobalRegistry()
{
	static FNameRegistry globalRegistry;
	return globalRegistry;
}

FName::FName()
{
}

FName::FName(const ANSICHAR* InName, ENameSearchMode InSearchMode)
{
	m_NameEntry = FNameRegistry::GetGlobalRegistry().Find(InName, InSearchMode);
}

FName::FName(const FString& InName, ENameSearchMode InSearchMode)
{
	auto converter = FSTRING_TO_ANSI(InName);
	m_NameEntry = FNameRegistry::GetGlobalRegistry().Find(converter.Get(), InSearchMode);
}

#pragma once

#include "CoreModule.h"

#include "Map.h"
#include "ImpulseString.h"

// Max size of a name including null-terminator
#define NAME_SIZE 1024
#define MAX_NAME_LENGTH (NAME_SIZE - 1)

enum class ENameSearchMode : uint8
{
	FindOrAdd,
	FindOnly,
};

class CORE_API FNameEntry
{
public:

	FNameEntry();
	FNameEntry(const ANSICHAR* InName);

	/**
	* Checks if the name entry is none.
	* @return true if the name entry is none, false otherwise.
	*/
	bool IsNone() const;

	/**
	* Makes a string representation of the name entry.
	* @return The string representation of the name entry.
	*/
	FString ToString() const;

public:

	bool operator==(const ANSICHAR* Other) const;
	FORCEINLINE bool operator==(const FNameEntry& Other) const { return operator==(m_Name); }

public:

	friend uint32 GetTypeHash(const FNameEntry& NameEntry)
	{
		return GetTypeHash(NameEntry.m_Name);
	}

private:

	/** The name's string representation. */
	ANSICHAR m_Name[NAME_SIZE];
};

class CORE_API FNameRegistry
{
public:

	/**
	* Finds a name entry in the registry.
	* @param InName - The name to find.
	* @param SearchMode - The mode to use when searching.
	* @return The name entry if found, or nullptr if not found.
	*/
	FNameEntry* Find(const ANSICHAR* InName, ENameSearchMode SearchMode);

public:

	/**
	* Gets the global name registry.
	* @return The global name registry.
	*/
	static FNameRegistry& GetGlobalRegistry();

private:

	TSet<FNameEntry> m_Registry;
};

class CORE_API FName
{
public:

	FName();
	FName(const ANSICHAR* InName, ENameSearchMode InSearchMode = ENameSearchMode::FindOrAdd);
	explicit FName(const FString& InName, ENameSearchMode InSearchMode = ENameSearchMode::FindOrAdd);

	FName(const FName& Other) = default;
	FName(FName&& Other) = default;

	FName& operator=(const FName& Other) = default;
	FName& operator=(FName&& Other) = default;

	FORCEINLINE bool operator==(const FName& Other) const { return m_NameEntry == Other.m_NameEntry; }

public:

	/**
	* Checks if the name is none.
	* @return true if the name is none, false otherwise.
	*/
	FORCEINLINE bool IsNone() const { return !m_NameEntry || m_NameEntry->IsNone(); }

	/**
	* Makes a string representation of the name.
	* @return The string representation of the name.
	*/
	FORCEINLINE FString ToString() const { return m_NameEntry ? m_NameEntry->ToString() : TEXT("None"); }

private:

	FNameEntry* m_NameEntry = nullptr;

public:

	friend uint32 GetTypeHash(const FName& Name)
	{
		return !Name.IsNone() ? GetTypeHash(*Name.m_NameEntry) : 0;
	}
};

// Default names

extern CORE_API FName NAME_None;

// Network names

extern CORE_API FName NAME_Stream;
extern CORE_API FName NAME_Datagram;
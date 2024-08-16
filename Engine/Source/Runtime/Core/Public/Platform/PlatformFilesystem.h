#pragma once

#include "CoreModule.h"
#include "Definitions.h"

enum EFileAccess : int32
{
	// Read only
	FILE_Read = 0x00000001,

	// Write only
	FILE_Write = 0x00000002,

	// Read write
	FILE_ReadWrite = FILE_Read | FILE_Write,

	// Appends to the end of the file (Implies Write)
	FILE_Append = 0x00000004,

	// Truncates the file to 0 length (Implies Write)
	FILE_Truncate = 0x00000008,
};

enum class ESpecialDirectory : uint8
{
	Documents,
	Programs,
	ProgramsX86,
	ProgramData,
	Desktop,
	Downloads,
	Music,
	Pictures,
	Videos,
	User,
	AppData,
};

struct FGenericReadFileOptions
{
	uint64 Offset = 0;
};

class CORE_API FGenericPlatformFilesystem
{
public:
};

#if PLATFORM_WINDOWS
#include "Windows/WindowsFilesystem.h"
#endif
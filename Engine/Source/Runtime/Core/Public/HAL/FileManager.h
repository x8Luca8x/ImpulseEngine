#pragma once

#include "CoreModule.h"
#include "Filesystem.h"

#include "Serialization/Archive.h"

#define IE_PACKAGED_FILE_METADATA_VERSION 1
#define IE_PACKAGED_FILES_METADATA_VERSION 1

#define IE_PACKAGED_FILES_METADATA_FILENAME "pak.imdta" // pak.imdta = Impulse Packaged Metadata

struct FPackagedFileMetadata
{
	/** Version of the packaged files. */
	uint32 Version = 0;

	/** Relative path to the file. */
	FString Path;

	/** Offset of the file in the package. */
	uint64 Offset = 0;

	/** Size of the file. */
	uint64 Size = 0;

	void Serialize(FArchive& Ar)
	{
		Ar << Version;
		Ar << Path;
		Ar << Offset;
		Ar << Size;

		if (Ar.IsLoading())
			checkf(Version <= IE_PACKAGED_FILE_METADATA_VERSION, TEXT("Invalid version."));
	}

	friend FArchive& operator<<(FArchive& Ar, FPackagedFileMetadata& Value)
	{
		Value.Serialize(Ar);
		return Ar;
	}
};

struct FPacakgedFilesMetadata
{
	/** Version of the packaged files. */
	uint32 Version = 0;

	/** Number of files in the packaged files. (Only used to validate loaded files numbers) */
	uint32 NumFiles = 0;

	/** Relative path to all files */
	TArray<FPackagedFileMetadata> Files;

	void Serialize(FArchive& Ar)
	{
		Ar << Version;
		Ar << NumFiles;
		Ar << Files;

		if (Ar.IsLoading())
		{
			checkf(Files.Num() == NumFiles, TEXT("Number of paths does not match number of files."));
			checkf(Version <= IE_PACKAGED_FILES_METADATA_VERSION, TEXT("Invalid version."));
		}
	}

	friend FArchive& operator<<(FArchive& Ar, FPacakgedFilesMetadata& Value)
	{
		Value.Serialize(Ar);
		return Ar;
	}
};

/**
* @brief The file manager is a high-level interface to the file system, providing a range of file operations.
* It is intended to be a platform-independent way to access files, and should be used in preference to
* the platform-specific file I/O functions.
* 
* 
* 
* @brief There are two main types of file manager. 
* A file manager that is used for development and one that is used in pre-packaged programs. 
* The development file manager behaves identically to the platform file system.
* Whereas the pre-package file manager reads and writes the necessary data from a single file.
*/
class CORE_API IFileManager
{
public:

	// Writes bytes to a file
	// @param InPath - Relative path to the file to write to
	// @param Data - Array of bytes to write
	// @return true if the data was written successfully, false otherwise
	virtual bool WriteBytes(const FString& InPath, const TArray<uint8>& Data) = 0;

	// Read the contents of a file into a byte array
	// @param InPath - Relative path to the file to read from
	// @param OutData - Array to store the file contents in
	// @param Options - Options for reading the file
	// @return true if the file was read successfully, false otherwise
	virtual bool ReadBytes(const FString& InPath, TArray<uint8>& OutData, const FReadFileOptions& Options = FReadFileOptions()) = 0;

public:

	/**
	* Gets the engine file manager.
	* @return Engine file manager.
	*/
	static IFileManager& GetEngineFileManager();

	/**
	* Gets the game file manager.
	* @return Game file manager.
	*/
	static IFileManager& GetGameFileManager();
};
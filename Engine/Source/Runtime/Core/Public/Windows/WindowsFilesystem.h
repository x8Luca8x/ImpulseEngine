#pragma once

#include "Platform/PlatformFilesystem.h"

#if PLATFORM_WINDOWS

#include "Containers/ArrayView.h"
#include "Containers/ImpulseString.h"

typedef void* FFileHandle;

class CORE_API FWindowsFilesystem : public FGenericPlatformFilesystem
{
public:

	// Validates a file handle
	// @param Handle - Handle to validate
	// @return true if the handle is valid, false otherwise
	static bool IsFileValid(FFileHandle Handle);

	// Opens a new file handle
	// @param Filename - Name of the file to open
	// @param Access - Access mode to open the file with
	// @return a handle to the opened file, or nullptr if the file could not be opened
	static FFileHandle OpenFile(const FString& Filename, EFileAccess Access);

	// Closes a file handle
	// @param Handle - Handle to the file to close
	static void CloseFile(FFileHandle Handle);

	// Get the size of a file
	// @param Handle - Handle to the file to get the size of
	// @return the size of the file in bytes
	static uint64 GetFileSize(FFileHandle Handle);

	/**
	* Checks whether a path is a file (Can also be used to check if a file exists)
	* @param Path - Path to check
	* @return true if the path is a file, false otherwise
	*/
	static bool IsFile(const FString& Path);

	/**
	* Checks whether a path is a directory (Can also be used to check if a directory exists)
	* @param Path - Path to check
	* @return true if the path is a directory, false otherwise
	*/
	static bool IsDirectory(const FString& Path);

	/**
	* Creates a new file
	* @param Filename - Name of the file to create
	* @return true if the file was created successfully, false otherwise
	*/
	static bool CreateFile(const FString& Filename);

	/**
	* Creates an new directory
	* @param Directory - Name of the directory to create
	* @return true if the directory was created successfully, false otherwise
	*/
	static bool CreateDirectory(const FString& Directory);

	// Write data to a file
	// @param Handle - Handle to the file to write to
	// @param Data - Pointer to the data to write
	// @param Size - Number of bytes to write
	// @return true if the data was written successfully, false otherwise
	static bool WriteData(FFileHandle Handle, const void* Data, uint64 Size);

	// Read data from a file
	// @param Handle - Handle to the file to read from
	// @param Data - Pointer to the buffer to read into (must be at least Size bytes)
	// @param Size - Number of bytes to read
	// @return true if the data was read successfully, false otherwise
	static bool ReadData(FFileHandle Handle, void* Data, uint64 Size, const FGenericReadFileOptions& Options = FGenericReadFileOptions());
};

typedef FWindowsFilesystem FPlatformFilesystem;

#endif
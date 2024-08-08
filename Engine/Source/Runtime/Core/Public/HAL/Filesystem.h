#pragma once

#include "Containers/Array.h"
#include "Platform/PlatformFilesystem.h"

struct FReadFileOptions : public FGenericReadFileOptions
{
	// If MaxBytesToRead is 0, the entire file will be read
	uint64 MaxBytesToRead = 0;
};

class CORE_API FFilesystem : public FPlatformFilesystem
{
public:

	// Calculates the number of bytes to read with offset
	// @param FileSize - Size of the file to read from
	// @param Options - Options for reading the file
	// @return the number of bytes to read
	static uint64 CalculateBytesToRead(uint64 FileSize, const FReadFileOptions& Options = FReadFileOptions());

	// Writes bytes to a file
	// @param Handle - Handle to the file to write to
	// @param Data - Array of bytes to write
	// @return true if the data was written successfully, false otherwise
	static bool WriteBytes(FFileHandle Handle, const TArray<uint8>& Data);

	// Read the contents of a file into a byte array
	// @param Handle - Handle to the file to read from
	// @param OutData - Array to store the file contents in
	// @param Options - Options for reading the file
	// @return true if the file was read successfully, false otherwise
	static bool ReadBytes(FFileHandle Handle, TArray<uint8>& OutData, const FReadFileOptions& Options = FReadFileOptions());
};

/**
* Helper class for esily reading/writing data to/from a file.
*/
class CORE_API FFileHelper
{
public:

	/**
	* Creates a file at the specified path with all parent directories.
	* @param Filename - Name of the file to create.
	* @return true if the file was created successfully, false otherwise.
	*/
	static bool CreateFilePath(const FString& Filename);

	/**
	* Reads the contents of a file into a byte array.
	* @param Filename - Name of the file to read.
	* @param OutBytes - Array to store the file contents in.
	* @param Options - Options for reading the file.
	* @return true if the file was read successfully, false otherwise.
	*/
	static bool ReadBytesFromFile(const FString& Filename, TArray<uint8>& OutBytes, const FReadFileOptions& Options = FReadFileOptions());

	/**
	* Writes the contents of a byte array to a file.
	* @param Filename - Name of the file to write.
	* @param Data - Array of bytes to write.
	* @param bCreateFile - If true, the file will be created if it does not exist.
	* @return true if the data was written successfully, false otherwise.
	*/
	static bool WriteBytesToFile(const FString& Filename, const TArray<uint8>& Data, bool bCreateFile = true);
};
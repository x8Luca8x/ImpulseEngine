#include "HAL/Filesystem.h"

#include "Misc/Paths.h"

uint64 FFilesystem::CalculateBytesToRead(uint64 FileSize, const FReadFileOptions& Options)
{
	/*const uint64 bytesToRead = Options.MaxBytesToRead == 0 ? FileSize : FMath::Min(Options.MaxBytesToRead, FileSize);
	return bytesToRead > Options.Offset ? bytesToRead - Options.Offset : 0;*/

	uint64 maxBytes = FileSize - Options.Offset;
	if (Options.MaxBytesToRead > 0)
		maxBytes = FMath::Min(maxBytes, Options.MaxBytesToRead);

	return maxBytes;
}

bool FFilesystem::WriteBytes(FFileHandle Handle, const TArray<uint8>& Data)
{
	return WriteData(Handle, Data.GetData(), (uint64)Data.Num());
}

bool FFilesystem::ReadBytes(FFileHandle Handle, TArray<uint8>& OutData, const FReadFileOptions& Options)
{
	OutData.Empty();

	// Validate data

	if (!IsFileValid(Handle))
		return false;

	// Get file size and calculate number of bytes to read

	const uint64 fileSize = GetFileSize(Handle);
	if (fileSize == 0)
		return true;

	const uint64 bytesToRead = CalculateBytesToRead(fileSize, Options);

	// Read data

	OutData.SetNumUninitialized((int32)bytesToRead);
	return ReadData(Handle, OutData.GetData(), bytesToRead, Options);
}

bool FFileHelper::CreateFilePath(const FString& Filename)
{
	TArray<FString> directories;
	FPaths::Split(Filename, directories, true);

	for (int32 i = 0; i < directories.Num() - 1; ++i)
		FFilesystem::CreateDirectory(directories[i]);

	return FFilesystem::CreateFile(Filename);
}

bool FFileHelper::ReadBytesFromFile(const FString& Filename, TArray<uint8>& OutBytes, const FReadFileOptions& Options)
{
	FFileHandle handle = FFilesystem::OpenFile(Filename, FILE_Read);
	const bool bResult = FFilesystem::ReadBytes(handle, OutBytes, Options);

	FFilesystem::CloseFile(handle);
	return bResult;
}

bool FFileHelper::WriteBytesToFile(const FString& Filename, const TArray<uint8>& Data, bool bCreateFile)
{
	if(bCreateFile && !FFilesystem::IsFile(Filename))
		CreateFilePath(Filename);

	FFileHandle handle = FFilesystem::OpenFile(Filename, FILE_Write);
	const bool bResult = FFilesystem::WriteBytes(handle, Data);

	FFilesystem::CloseFile(handle);
	return bResult;
}
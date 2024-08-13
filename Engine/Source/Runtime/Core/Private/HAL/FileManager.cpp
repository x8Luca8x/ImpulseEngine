#include "HAL/FileManager.h"

#include "Misc/Paths.h"

#include "Containers/Map.h"

struct FPackagedFile
{
	/** Absolute path to the file. */
	FString Path;

	/** Offset to the start of the file. */
	uint64 Offset;

	/** Size of the file. */
	uint64 Size;
};

/**
* File manager that reads and writes files from the platform file system.
*/
class FFileManager final : public IFileManager
{
public:

	FFileManager(const FString& InBasePath);

public:

	//~ IFileManager interface

	bool WriteBytes(const FString& InPath, const TArray<uint8>& Data) override;
	bool ReadBytes(const FString& InPath, TArray<uint8>& OutData, const FReadFileOptions& Options = FReadFileOptions()) override;

	//~ End IFileManager interface

private:

	FString BasePath;
};

/**
* File manager that reads and writes files from a single file.
*/
class FPackagedFileManager final : public IFileManager
{
public:

	//~ IFileManager interface

	bool WriteBytes(const FString& InPath, const TArray<uint8>& Data) override;
	bool ReadBytes(const FString& InPath, TArray<uint8>& OutData, const FReadFileOptions& Options = FReadFileOptions()) override;

	//~ End IFileManager interface

private:

	/**
	* Caches the file data.
	*/
	void CacheFiles();

	/** 
	* Cached files. 
	* Key: Relative path to the file.
	* Value: File data.
	*/
	TMap<FString, FPackagedFile> Files;
};

#if IE_PACKAGED_BUILD

static FPackagedFileManager GPackagedFileManager;

#endif

IFileManager& IFileManager::GetEngineFileManager()
{
#if IE_PACKAGED_BUILD

	return GPackagedFileManager;

#else

	static FFileManager manager(FPaths::EngineDir());
	return manager;

#endif
}

IFileManager& IFileManager::GetGameFileManager()
{
#if IE_PACKAGED_BUILD

	return GPackagedFileManager;

#else

	static FFileManager manager(FPaths::GameDir());
	return manager;

#endif
}

FFileManager::FFileManager(const FString& InBasePath)
	: BasePath(InBasePath) {}

bool FFileManager::WriteBytes(const FString& InPath, const TArray<uint8>& Data)
{
	FFileHandle handle = FFilesystem::OpenFile(FPaths::MakePathAbsolute(InPath, BasePath), FILE_Write);
	const bool bResult = FFilesystem::WriteBytes(handle, Data);

	FFilesystem::CloseFile(handle);
	return bResult;
}

bool FFileManager::ReadBytes(const FString& InPath, TArray<uint8>& OutData, const FReadFileOptions& Options)
{
	FFileHandle handle = FFilesystem::OpenFile(FPaths::MakePathAbsolute(InPath, BasePath), FILE_Read);
	const bool bResult = FFilesystem::ReadBytes(handle, OutData, Options);

	FFilesystem::CloseFile(handle);
	return bResult;
}



bool FPackagedFileManager::WriteBytes(const FString& InPath, const TArray<uint8>& Data)
{
	return false;
}

bool FPackagedFileManager::ReadBytes(const FString& InPath, TArray<uint8>& OutData, const FReadFileOptions& Options)
{
	return false;
}

void FPackagedFileManager::CacheFiles()
{
	if (Files.Num() == 0)
	{

	}
}

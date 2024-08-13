#include "Misc/Paths.h"

#include <Windows/WindowsAPI.h>

const TCHAR FPaths::DefaultDirectorySeparator = TEXT('\\');
const FString FPaths::DirectorySeparatorChars = TEXT("\\/");

void FPaths::Split(const FString& InPath, TArray<FString>& OutPaths, bool bFullPath)
{
	OutPaths.Empty(1);

	const TCHAR* start = *InPath;
	const TCHAR* end = start + InPath.Length();

	TCHAR* str = const_cast<TCHAR*>(start);

	while (str != end)
	{
		if (DirectorySeparatorChars.ContainsChar(*(str++)))
		{
			const int32 length = static_cast<int32>(str - start) - 1;
			if (length > 0)
			{
				if (bFullPath)
					OutPaths.Add(InPath.Left(length));
				else
					OutPaths.Add(GetFilename(InPath.Left(length)));
			}
		}
	}


	if (bFullPath)
		OutPaths.Add(InPath);
	else
		OutPaths.Add(GetFilename(InPath));
}

FString FPaths::GetPathParent(const FString& InPath)
{
	for (int32 i = InPath.Length() - 1; i >= 0; --i)
	{
		if (DirectorySeparatorChars.ContainsChar(InPath[i]))
			return InPath.Left(i);
	}

	return FString();
}

FString FPaths::GetFilename(const FString& InPath)
{
	for (int32 i = InPath.Length() - 1; i >= 0; --i)
	{
		if (DirectorySeparatorChars.ContainsChar(InPath[i]))
			return InPath.Right(i);
	}

	return InPath;
}

FString FPaths::GetFilenameWithoutExtension(const FString& InPath)
{
	FString filename = GetFilename(InPath);
	const int32 dotIndex = filename.Find(TEXT("."), ESearchCase::CaseSensitive, ESearchDir::FromEnd);

	return dotIndex == INDEX_NONE ? filename : filename.Left(dotIndex);
}

FString FPaths::GetExtension(const FString& InPath)
{
	FString filename = GetFilename(InPath);
	const int32 dotIndex = filename.Find(TEXT("."), ESearchCase::CaseSensitive, ESearchDir::FromEnd);

	return dotIndex == INDEX_NONE ? FString() : filename.Right(dotIndex);
}

FString FPaths::MakePathRelativeTo(const FString& InPath, const FString& InBasePath)
{
	// Check InPath and InBasePath have the same root

	TArray<FString> pathParts;
	TArray<FString> basePathParts;

	Split(InPath, pathParts, false);
	Split(InBasePath, basePathParts, false);

	if (pathParts.Num() == 0 || basePathParts.Num() == 0 || pathParts[0] != basePathParts[0])
		return FString();

	int32 samePathIndex = 0;
	for (int32 i = 0; i < pathParts.Num() && i < basePathParts.Num(); ++i)
	{
		if (pathParts[i] != basePathParts[i])
			break;

		samePathIndex = i;
	}

	const int32 numStepsBack = basePathParts.Num() - samePathIndex - 1;

	FString relativePath;
	for (int32 i = 0; i < numStepsBack; ++i)
		relativePath.InlineAppend(FString(TEXT("..")) + DefaultDirectorySeparator);

	for (int32 i = samePathIndex + 1; i < pathParts.Num(); ++i)
	{
		relativePath.InlineAppend(pathParts[i]);
		if (i < pathParts.Num() - 1)
			relativePath.InlineAppendChar(DefaultDirectorySeparator);
	}

	return relativePath;
}

FString FPaths::MakePathAbsolute(const FString& InPath, const FString& InBasePath)
{
	FString absolutePath = InBasePath;
	
	TArray<FString> paths;
	Split(InPath, paths, false);

	for (int32 i = 0; i < paths.Num(); ++i)
	{
		if (paths[i] == TEXT(".."))
			absolutePath = GetPathParent(absolutePath);
		else
			absolutePath = Combine(absolutePath, paths[i]);
	}

	return absolutePath;
}

FString FPaths::ExecutableDir()
{
#if PLATFORM_WINDOWS

	TCHAR buffer[MAX_PATH];

	GetModuleFileName(nullptr, buffer, MAX_PATH);
	return FString(buffer);

#else

#error "ExecutableDir is not implemented for this platform"

#endif
}

FString FPaths::EngineDir()
{
#if IE_PACKAGED_BUILD
	return Combine(GameRootDir(), TEXT("Engine"));
#else
	return GetPathParent(GetPathParent(GetPathParent(GetPathParent(ExecutableDir()))));
#endif
}

FString FPaths::GameDir()
{
#if IE_PACKAGED_BUILD
	return Combine(GameRootDir(), TEXT("Game"));
#else
	return GameRootDir();
#endif
}

FString FPaths::GameRootDir()
{
#if IE_PACKAGED_BUILD
	return GetPathParent(GetPathParent(GetPathParent(GetPathParent(GetPathParent(ExecutableDir())))));
#else

	// TODO: Get from command line
	return FString();

#endif
}

FString FPaths::EnginePluginsDir()
{
	return Combine(EngineDir(), TEXT("Plugins"));
}

FString FPaths::GamePluginsDir()
{
	return Combine(GameDir(), TEXT("Plugins"));
}

FString FPaths::Combine(const TArray<FString>& InPaths)
{
	FString combinedPath;

	for (int32 i = 0; i < InPaths.Num(); ++i)
	{
		combinedPath.InlineAppend(InPaths[i]);
		if (i < InPaths.Num() - 1)
			combinedPath.InlineAppendChar(DefaultDirectorySeparator);
	}

	return combinedPath;
}
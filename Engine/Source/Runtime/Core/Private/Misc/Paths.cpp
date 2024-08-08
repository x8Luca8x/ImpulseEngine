#include "Misc/Paths.h"

const TCHAR FPaths::DefaultDirectorySeparator = TEXT('\\');
const FString FPaths::DirectorySeparatorChars = TEXT("\\/");

void FPaths::Split(const FString& InPath, TArray<FString>& OutPaths)
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
				OutPaths.Add(InPath.Left(length));
		}
	}

	OutPaths.Add(InPath);
}

FString FPaths::EngineDir()
{
	return FString();
}

FString FPaths::GameDir()
{
	return FString();
}
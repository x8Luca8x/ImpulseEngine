#pragma once

#include "CoreMinimal.h"

class CORE_API FPaths
{
public:

	/**
	* Splits a path into full paths of directory segments and the filename.
	* E.g. "C:\User\TestUser\Documents\MyFile.txt" would return "C:", "C:\User", "C:\User\TestUser", "C:\User\TestUser\Documents", "C:\User\TestUser\Documents\MyFile.txt"
	* @param InPath - Path to split.
	* @param OutPaths - Array to store the split paths in.
	*/
	static void Split(const FString& InPath, TArray<FString>& OutPaths);

	/**
	* Gets the base engine directory.
	* @return Base engine directory.
	*/
	static FString EngineDir();

	/**
	* Gets the base directory for the game.
	* @return Base directory for the game.
	*/
	static FString GameDir();

public:

	/** Default sperator character to sperator directies */
	static const TCHAR DefaultDirectorySeparator;

	/** Character that can be used to separate directories. */
	static const FString DirectorySeparatorChars;
};
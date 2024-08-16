#pragma once

#include "CoreMinimal.h"

class CORE_API FPaths
{
public:

	/**
	* Splits a path into full paths of directory segments and the filename.
	* E.g. "C:\User\TestUser\Documents\MyFile.txt" would return "C:", "C:\User", "C:\User\TestUser", "C:\User\TestUser\Documents", "C:\User\TestUser\Documents\MyFile.txt"
	* @param InPath - Path to split.
	* @param bFullPath - Whether to include the full path or just the filename.
	* @param OutPaths - Array to store the split paths in.
	*/
	static void Split(const FString& InPath, TArray<FString>& OutPaths, bool bFullPath = true);

	/**
	* Gets the parent directory of a path.
	* @param InPath - Path to get the parent directory of.
	* @return Parent directory of the path.
	*/
	static FString GetPathParent(const FString& InPath);

	/**
	* Gets the filename of a path.
	* @param InPath - Path to get the filename of.
	* @return Filename of the path.
	*/
	static FString GetFilename(const FString& InPath);

	/**
	* Gets the filename without the extension of a path.
	* @param InPath - Path to get the filename without the extension of.
	* @return Filename without the extension of the path.
	*/
	static FString GetFilenameWithoutExtension(const FString& InPath);

	/**
	* Gets the file extension of a path.
	* @param InPath - Path to get the file extension of.
	* @return File extension of the path.
	*/
	static FString GetExtension(const FString& InPath);

	/**
	* Makes a path relative to the base directory.
	* @param InPath - Path to make relative.
	* @param InBasePath - Base path to make the path relative to.
	* @return Path relative to the base directory.
	*/
	static FString MakePathRelativeTo(const FString& InPath, const FString& InBasePath);

	/**
	* Makes a relative path absolute by combining it with the base directory.
	* @param InPath - Path to make absolute.
	* @param InBasePath - Base path to make the path absolute to.
	* @return Absolute path.
	*/
	static FString MakePathAbsolute(const FString& InPath, const FString& InBasePath);

	/**
	* Gets the executable directory.
	* @return Executable directory.
	*/
	static FString ExecutableDir();

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

	/**
	* Gets the root directory for the game.
	* @return Root directory for the game.
	*/
	static FString GameRootDir();

	/**
	* Gets the plugin directory of the engine.
	* @return Plugin directory of the engine.
	*/
	static FString EnginePluginsDir();

	/**
	* Gets the plugin directory of the game.
	* @return Plugin directory of the game.
	*/
	static FString GamePluginsDir();

	/**
	* Gets the user directory for the game.
	* @return User directory for the game.
	*/
	static FString GameUserDir();

	/**
	* Gets the saved directory for the game.
	* @return Saved directory for the game.
	*/
	static FString GameSavedDir();

	/**
	* Gets the logs directory for the game.
	* @return Logs directory for the game.
	*/
	static FString GameLogDir();

	/**
	* Combines multiple paths into a single path.
	* @param InPaths - Paths to combine.
	* @return Combined path.
	*/
	static FString Combine(const TArray<FString>& InPaths);

	/**
	* Combines multiple paths into a single path.
	* @param InPaths - Paths to combine.
	* @return Combined path.
	*/
	template<typename... Args>
	static FString Combine(const FString& InPath, Args&&... InPaths)
	{
		TArray<FString> paths = { InPath, Forward<Args>(InPaths)... };
		return Combine(paths);
	}

public:

	/** Default sperator character to sperator directies */
	static const TCHAR DefaultDirectorySeparator;

	/** Character that can be used to separate directories. */
	static const FString DirectorySeparatorChars;
};
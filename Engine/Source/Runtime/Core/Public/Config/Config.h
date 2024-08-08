#pragma once

#include "ConfigCache.h"

/**
* Paths to the various config files.
*/
struct FConfigFile
{
	FConfigFile(const FString& InFilename, const FString& InPlatform, const FString& InPlatformIni, const FString& InBaseIni, const FString& InEngineIni, const FString& InProjectIni)
		: Filename(InFilename)
		, Platform(InPlatform)
		, PlatformIni(InPlatformIni)
		, BaseIni(InBaseIni)
		, EngineIni(InEngineIni)
		, ProjectIni(InProjectIni)
	{
	}

	FString Filename;
	FString Platform;
	FString PlatformIni;
	FString BaseIni;
	FString EngineIni;
	FString ProjectIni;
};
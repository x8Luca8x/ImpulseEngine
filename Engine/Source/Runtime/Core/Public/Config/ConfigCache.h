#pragma once

#include "Containers/Map.h"
#include "Containers/ImpulseString.h"

#include "ConfigValues.h"

struct FConfigSection
{
	TMap<FString, FConfigValue*> Values;
};

struct FConfigContext
{
	FString Filename;
	TMap<FString, FConfigSection> Sections;
};

/**
* Caches every config file in memory.
*/
class CORE_API FConfigCacheIni
{
public:

private:

	/** Map of config file name to config context */
	TMap<FString, FConfigContext> Configs;
};

FConfigCacheIni GConfig;
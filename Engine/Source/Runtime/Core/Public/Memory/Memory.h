#pragma once

#include "Windows/WindowsMemory.h"

class CORE_API FMemory : public FPlatformMemory
{
public:

	static FPlatformMemoryConstants* GetConstants();

private:

	static bool bConstantsCached;
	static FPlatformMemoryConstants CachedConstants;
};
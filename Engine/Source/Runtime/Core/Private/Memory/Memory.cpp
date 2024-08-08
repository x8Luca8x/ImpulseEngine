#include "Memory/Memory.h"

bool FMemory::bConstantsCached = false;
FPlatformMemoryConstants FMemory::CachedConstants;

FPlatformMemoryConstants* FMemory::GetConstants()
{
	if (bConstantsCached)
		return &CachedConstants;

	bConstantsCached = FillConstants(CachedConstants);
	return &CachedConstants;
}

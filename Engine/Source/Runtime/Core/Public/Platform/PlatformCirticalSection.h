#pragma once

#include "Definitions.h"

class CORE_API FGenericPlatformCriticalSection
{
};

#if PLATFORM_WINDOWS
#include "Windows/WindowsCiriticalSection.h"
#endif
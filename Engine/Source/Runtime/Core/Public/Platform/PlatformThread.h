#pragma once

#include "Definitions.h"

class CORE_API FGenericPlatformThread
{
public:
};

#if PLATFORM_WINDOWS
#include "Windows/WindowsThread.h"
#endif
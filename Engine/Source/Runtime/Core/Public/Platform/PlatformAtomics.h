#pragma once

#include "Definitions.h"

class CORE_API FGenericPlatformAtomics
{

};

#if PLATFORM_WINDOWS
#include "Windows/WindowsAtomics.h"
#endif
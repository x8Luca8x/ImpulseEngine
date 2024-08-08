#pragma once

#include "CoreDefinitions.h"

#pragma warning(disable : 4275)
#pragma warning(disable : 4251)

#if defined(CORE_EXPORTS) && CORE_EXPORTS

#define CORE_API DLLEXPORT

#else

#define CORE_API DLLIMPORT

#endif
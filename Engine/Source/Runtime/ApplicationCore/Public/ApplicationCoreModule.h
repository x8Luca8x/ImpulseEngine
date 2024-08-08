#pragma once

#include "CoreMinimal.h"

#if defined(APPLICATIONCORE_EXPORTS) && APPLICATIONCORE_EXPORTS
#define APPCORE_API DLLEXPORT
#else
#define APPCORE_API DLLIMPORT
#endif
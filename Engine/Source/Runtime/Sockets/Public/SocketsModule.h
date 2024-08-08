#pragma once

#include "CoreMinimal.h"

#if SOCKETS_EXPORTS
#define SOCKETS_API DLLEXPORT
#else
#define SOCKETS_API DLLIMPORT
#endif
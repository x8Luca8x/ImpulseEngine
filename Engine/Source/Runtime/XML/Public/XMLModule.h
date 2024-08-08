#pragma once

#include "Definitions.h"

#if defined(XML_EXPORTS) && XML_EXPORTS
#define XML_API DLLEXPORT
#else
#define XML_API DLLIMPORT
#endif
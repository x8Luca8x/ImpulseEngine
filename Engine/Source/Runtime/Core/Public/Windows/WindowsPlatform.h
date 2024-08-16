#pragma once

#include "Platform/Platform.h"

#if PLATFORM_WINDOWS

#define WINAPI __stdcall

#define PLATFORM_LITTLE_ENDIAN 1

class CORE_API FWindowsPlatform
{
public:
};

class CORE_API FWindowsPlatformTypes : public FGenericPlatformTypes
{
public:

	typedef unsigned long DWORD;
};

typedef FWindowsPlatform FPlatform;
typedef FWindowsPlatformTypes FPlatformTypes;

#endif
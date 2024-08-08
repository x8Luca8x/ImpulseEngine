#pragma once

#include "CoreModule.h"

#include "Delegate/MulticastDelegate.h"

typedef TMulticastDelegate<uint32 /* ExitCode */> FRequestExitDelegate;

class CORE_API FGenericPlatformMisc
{
public:

	/** Collection of delegates to call when the application is requesting exit */
	static FRequestExitDelegate RequestExitDelegates;
};

#if PLATFORM_WINDOWS
#include "Windows/WindowsMisc.h"
#endif
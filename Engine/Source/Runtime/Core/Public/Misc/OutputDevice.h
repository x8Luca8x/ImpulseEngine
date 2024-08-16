#pragma once

#include "Containers/ImpulseString.h"

namespace ELogVerbosity
{
	enum Type
	{
		NoLogging = 0,
		Fatal,
		Error,
		Warning,
		Display,
		Log,
		Verbose,
		VeryVerbose,
		All
	};
}

class CORE_API FOutputDevice
{
public:
};
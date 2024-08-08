#pragma once

#include "CoreModule.h"

/**
* Holds essential information about the environment the application is running in.
* E.G. command line arguments, environment variables, etc.
*/
class FEnvironment
{
public:

	FEnvironment();
	FEnvironment(const FEnvironment&) = delete;
	FEnvironment(FEnvironment&&) = delete;

	FEnvironment& operator=(const FEnvironment&) = delete;
	FEnvironment& operator=(FEnvironment&&) = delete;

	/** Whether the app is single threaded or not. */
	inline bool IsSingleThreaded() const { return bIsSingleThreaded; }

private:

	/** Commandline: -singlethreaded */
	bool bIsSingleThreaded = false;
};

extern CORE_API FEnvironment GEnvironment;
#include "Environment.h"

#include "Misc/Parse.h"
#include "Misc/CommandLine.h"

FEnvironment GEnvironment;

FEnvironment::FEnvironment()
{
	bIsSingleThreaded = FParse::Param(FCommandLine::Get(), TEXT("singlethreaded"));
}
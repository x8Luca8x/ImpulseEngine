#pragma once

#include "Containers/ImpulseString.h"

extern CORE_API FString GInternalProjectName;

struct FAppStatics
{
	static FString GetProjectName()
	{
		return GInternalProjectName;
	}
};

#define SET_PROJECT_NAME(ProjName) GInternalProjectName = ProjName;
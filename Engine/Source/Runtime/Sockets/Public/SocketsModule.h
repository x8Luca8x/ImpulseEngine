#pragma once

#include "CoreMinimal.h"
#include "Module/ModuleManager.h"

class ISocketSubsystem;

#if SOCKETS_EXPORTS
#define SOCKETS_API DLLEXPORT
#else
#define SOCKETS_API DLLIMPORT
#endif

class SOCKETS_API FSocketsModule : public IModuleInterface
{
public:

	static inline FSocketsModule& Get()
	{
		return FModuleManager::Get().LoadModuleChecked<FSocketsModule>(TEXT("Sockets"));
	}

public:

	virtual ISocketSubsystem* GetSocketSubsystem(const FName SubsystemName) const = 0;
};
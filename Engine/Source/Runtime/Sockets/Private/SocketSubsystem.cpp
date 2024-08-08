#include "SocketSubsystem.h"

FSocketSubsystemRegistry& FSocketSubsystemRegistry::Get()
{
	static FSocketSubsystemRegistry Singleton;
	return Singleton;
}

FSocketSubsystemRegistry::FSocketSubsystemRegistry()
{
}

void FSocketSubsystemRegistry::RegisterSocketSubsystem(const FName SubsystemName, ISocketSubsystem* Subsystem)
{
	SocketSubsystems.Add(SubsystemName, Subsystem);
}

void FSocketSubsystemRegistry::UnregisterSocketSubsystem(const FName SubsystemName)
{
	SocketSubsystems.Remove(SubsystemName);
}
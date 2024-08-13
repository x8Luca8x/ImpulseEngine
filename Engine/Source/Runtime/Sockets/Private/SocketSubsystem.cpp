#include "SocketSubsystem.h"

#include "SocketsModule.h"

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

ISocketSubsystem* ISocketSubsystem::Get(const FName SubsystemName)
{
	return FSocketsModule::Get().GetSocketSubsystem(SubsystemName);
}
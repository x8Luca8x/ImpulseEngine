#include "SocketsModule.h"
#include "Module/ModuleInterface.h"

#include "Windows/WindowsSocketSubsystem.h"

class FSocketsModulePrivate : public FSocketsModule
{
public:

	virtual void OnStartup() override
	{
#if PLATFORM_WINDOWS
		SocketSubsystemRegistry.RegisterSocketSubsystem(NAME_WindowsSocketSubsystem, FWindowsSocketSubsystem::Create());
#endif
	}

	virtual void OnShutdown() override
	{
		for (const TPair<FName, ISocketSubsystem*>& pair : SocketSubsystemRegistry.SocketSubsystems)
		{
			pair.Value->Shutdown();
			delete pair.Value;
		}

		SocketSubsystemRegistry.SocketSubsystems.Clear();
	}

public:

	ISocketSubsystem* FSocketsModule::GetSocketSubsystem(const FName SubsystemName) const override
	{
		ISocketSubsystem* const * subsystemPtr = SocketSubsystemRegistry.SocketSubsystems.Find(SubsystemName);
		return subsystemPtr ? *subsystemPtr : nullptr;
	}

private:

	FSocketSubsystemRegistry SocketSubsystemRegistry;
};

IMPLEMENT_MODULE(FSocketsModulePrivate);
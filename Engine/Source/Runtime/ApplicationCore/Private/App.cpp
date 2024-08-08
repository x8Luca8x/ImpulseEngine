#include "App.h"

volatile bool FApp::s_bInitialized = false;
FDelegateHandle FApp::s_RequestExitDelegateHandle;
FAppCommandsArgs FApp::s_CommandLineArgs;
IApplication* FApp::s_CurrentApplication = nullptr;

void FApp::Init()
{
	if (s_bInitialized)
		return;

	// Call FPlatformMisc::Seconds() to initialize the time

	FPlatformMisc::Seconds();

	// Bind delegates

	s_RequestExitDelegateHandle = FPlatformMisc::RequestExitDelegates.Add(FRequestExitDelegate::FDelegate::CreateStatic(&ExitRequested));

	// Init command line

	FCommandLine::Init();
	s_CommandLineArgs.Init(FCommandLine::GetAsArray());

	// Initialization successful

	s_bInitialized = true;
}

void FApp::Shutdown()
{
	if (!s_bInitialized)
		return;

	// Unbind delegates

	FPlatformMisc::RequestExitDelegates.Remove(s_RequestExitDelegateHandle);
	s_RequestExitDelegateHandle.Reset();

	RemoveApplication(true);
	FStringCastRegistry::Clear();

	s_bInitialized = false;
}

void FApp::DestroyApplication(IApplication* Application)
{
	if (!Application || Application == s_CurrentApplication)
		return;

	Application->Destroy();
	delete Application;
}

void FApp::SetApplication(IApplication* Application)
{
	if (!Application || s_CurrentApplication)
		return;

	Application->PreInit();

	s_CurrentApplication = Application;

	s_CurrentApplication->PostInit();
	s_CurrentApplication->Run();
}

void FApp::RemoveApplication(bool bDestroy)
{
	if (!s_CurrentApplication)
		return;

	s_CurrentApplication->PreShutdown();

	IApplication* application = s_CurrentApplication;
	s_CurrentApplication = nullptr;

	application->PostShutdown();

	if (bDestroy)
		DestroyApplication(application);
}

void FApp::ExitRequested(uint32 ExitCode)
{
	Shutdown();
}

void FAppCommandsArgs::Init(const TArray<FString>& InCommandLine)
{
	for (const FString& string : InCommandLine)
	{
		if (string.Equals(TEXT("-norender"), ESearchCase::IgnoreCase))
			bCanRender = false;
	}
}
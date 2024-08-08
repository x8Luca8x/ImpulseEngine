#pragma once

#include "Platform/PlatformEntryPoint.h"

#if PLATFORM_WINDOWS

#include "Windows/WindowsAPI.h"
#include "Windows/WindowsWindow.h"
#include "Windows/WindowsApplication.h"

#if defined(_M_CEE_PURE)
#define ENTRYPOINT_WINAPI __clrcall
#else
#define ENTRYPOINT_WINAPI WINAPI
#endif

/**
* Winapi entry point
* @param hInstance - The instance of the application
* @param hPrevInstance - The previous instance of the application (unused)
* @param lpCmdLine - The command line arguments
* @param nShowCmd - The show command
*/
int ENTRYPOINT_WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nShowCmd)
{
	FCommandLine::Init(lpCmdLine);
	FApp::Init();

	FWindowsPlatformMisc::CoInitialize(GEnvironment.IsSingleThreaded());
	FWindowsWindow::RegisterWindowClass();

	FApp::SetApplication(EntryPointGetApplication());

	FWindowsPlatformMisc::CoUninitialize();
	FApp::Shutdown();

	return 0;
}

#endif
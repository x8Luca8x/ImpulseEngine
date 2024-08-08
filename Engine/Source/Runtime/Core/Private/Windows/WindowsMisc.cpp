#include "Windows/WindowsMisc.h"

#if PLATFORM_WINDOWS

#include <Windows/WindowsAPI.h>

#pragma push_macro("GetCommandLine")
#undef GetCommandLine

#include <memory>

#include "Misc/StringConverter.h"

bool FWindowsPlatformMisc::bRequestedExit = false;

FModuleHandle FWindowsPlatformMisc::LoadDllHandle(const TCHAR* Path)
{
	return LoadLibraryW(Path);
}

void FWindowsPlatformMisc::FreeDllHandle(FModuleHandle DllHandle)
{
	FreeLibrary(DllHandle);
}

const TCHAR* FWindowsPlatformMisc::GetCommandLine()
{
	return GetCommandLineW();
}

void FWindowsPlatformMisc::SystemTime(FPlatformTime& OutTime)
{
	GetSystemTime((LPSYSTEMTIME)&OutTime);
}

void FWindowsPlatformMisc::RequestExit(uint32 ExitCode, bool bForce)
{
	if (bRequestedExit)
		return;

	bRequestedExit = true;
	if (!bForce)
	{
		// If we're not forcing the exit, we notify the user that we're shutting down
		// Make a copy of the delegates, as they may be modified by the delegate itself

		FRequestExitDelegate copy = RequestExitDelegates;
		copy.Broadcast(ExitCode);
	}

	TerminateProcess(GetCurrentProcess(), ExitCode);
}

double FWindowsPlatformMisc::Seconds()
{
	static LARGE_INTEGER Frequency{};
	static LARGE_INTEGER StartSeconds{};
	static bool bInitialized = false;

	if (!bInitialized)
	{
		QueryPerformanceFrequency(&Frequency);
		QueryPerformanceCounter(&StartSeconds);

		bInitialized = true;
		return 0.0;
	}

	LARGE_INTEGER CurrentSeconds{};
	QueryPerformanceCounter(&CurrentSeconds);

	return (double)(CurrentSeconds.QuadPart - StartSeconds.QuadPart) / (double)Frequency.QuadPart;
}

bool FWindowsPlatformMisc::CoInitialize(bool bSingleThreaded)
{
	const DWORD flags = bSingleThreaded ? COINIT_APARTMENTTHREADED : COINIT_MULTITHREADED;
	const HRESULT r = CoInitializeEx(NULL, flags);

	return SUCCEEDED(r) || r == S_FALSE; // S_FALSE means it was already initialized
}

void FWindowsPlatformMisc::CoUninitialize()
{
	::CoUninitialize();
}

void* FWindowsPlatformMisc::GetDllExport(FModuleHandle DllHandle, const TCHAR* ProcName)
{
	auto converter = TCHAR_TO_ANSI(ProcName);
	return (void*)GetProcAddress(DllHandle, converter.Get());
}

#pragma pop_macro("GetCommandLine")

#endif
#pragma once

#include "Platform/PlatformMisc.h"

#if PLATFORM_WINDOWS

#include "Definitions.h"

#include "Templates/Function.h"

#include <Windows/WindowsAPI.h>

#pragma push_macro("GetCommandLine")
#undef GetCommandLine

typedef HMODULE FModuleHandle;

/** Platform time structure. DO NOT USE THIS TO SAVE TIME STAMPS TO DISK! THIS CAN BE A DIFFERENT SIZE ON DIFFERENT PLATFORMS! USE DATETIME INSTEAD! */
typedef SYSTEMTIME FPlatformTime;

#define INVALID_MODULEHANDLE nullptr

class CORE_API FWindowsPlatformMisc : public FGenericPlatformMisc
{
public:

	/**
	* Loads a DLL.
	* @param Path - Path to the DLL to load.
	* @return Handle to the loaded DLL, or nullptr if the DLL could not be loaded.
	*/
	static FModuleHandle LoadDllHandle(const TCHAR* Path);

	/**
	* Unloads a DLL.
	* @param DllHandle - Handle to the DLL to unload.
	*/
	static void FreeDllHandle(FModuleHandle DllHandle);

	/**
	* Gets a function pointer from a DLL.
	* @param DllHandle - Handle to the DLL to get the function pointer from.
	* @param ProcName - Name of the function to get the pointer to.
	* @return Pointer to the function, or nullptr if the function could not be found.
	*/
	static void* GetDllExport(FModuleHandle DllHandle, const TCHAR* ProcName);

	/**
	* Gets a function pointer from a DLL.
	* @param DllHandle - Handle to the DLL to get the function pointer from.
	* @param ProcName - Name of the function to get the pointer to.
	* @return TFunction object wrapping the function, or nullptr if the function could not be found.
	*/
	template <typename FunctionType>
	static TFunction<FunctionType> GetDllExport(FModuleHandle DllHandle, const TCHAR* ProcName)
	{
		return TFunction<FunctionType>(reinterpret_cast<FunctionType*>(GetDllExport(DllHandle, ProcName)));
	}

	/**
	* Gets the command line.
	*/
	static const TCHAR* GetCommandLine();

	/**
	* Gets the system time
	*/
	static void SystemTime(FPlatformTime& OutTime);

	/**
	* Request exit.
	*/
	static void RequestExit(uint32 ExitCode = 0, bool bForce = false);

	/**
	* Gets the number of seconds since the first time this function was called.
	* First call will return 0.0.
	* @return Seconds since the first time this function was called.
	*/
	static double Seconds();

	/**
	* WINDOWS ONLY!
	* Initializes COM.
	* @param bSingleThreaded - Whether to initialize COM as single-threaded.
	* @return true if COM was successfully initialized, false otherwise.
	*/
	static bool CoInitialize(bool bSingleThreaded);

	/**
	* WINDOWS ONLY!
	* Uninitializes COM.
	*/
	static void CoUninitialize();

private:

	static bool bRequestedExit;

};

typedef FWindowsPlatformMisc FPlatformMisc;

#endif
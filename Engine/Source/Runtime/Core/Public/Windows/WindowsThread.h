#pragma once

#include "Platform/PlatformThread.h"

#if PLATFORM_WINDOWS

#include "WindowsAPI.h"

#include "Templates/Function.h"
#include "Templates/ImpulseTemplates.h"

#define INVALID_THREAD_ID 0
#define INVALID_THREAD_HANDLE nullptr

typedef uint32 FThreadId;
typedef HANDLE FThreadHandle;

struct FWinThread
{
	FWinThread() : ThreadHandle(nullptr), ThreadId(0) {}
	FWinThread(HANDLE InThreadHandle, uint32 InThreadId) : ThreadHandle(InThreadHandle), ThreadId(InThreadId) {}

	FThreadId ThreadId;
	FThreadHandle ThreadHandle;
};

typedef FWinThread FThread;
typedef TFunction<void(*)(void*)> FThreadFunction;

class CORE_API FWindowsThread : public FGenericPlatformThread
{
public:

	/**
	* Creates and initializes a new thread.
	* @param ThreadFunction - The function to run on the new thread.
	* @param ThreadParameter - The parameter to pass to the thread function.
	* @param bCreateSuspended - Whether to create the thread in a suspended state.
	* @return The handle to the new thread.
	*/
	static FThread CreateThread(FThreadFunction ThreadFunction, void* ThreadParameter, bool bCreateSuspended = false);

	/**
	* Suspends the execution of the given thread.
	* @param ThreadHandle - The handle to the thread to suspend.
	* @return true if the thread was successfully suspended, false otherwise.
	*/
	static bool SuspendThread(FThreadHandle ThreadHandle);

	/**
	* Resumes the execution of the given thread.
	* @param ThreadHandle - The handle to the thread to resume.
	* @return True if the thread was successfully resumed, false otherwise.
	*/
	static bool ResumeThread(FThreadHandle ThreadHandle);

	/**
	* Waits for the given thread to finish execution.
	* @param ThreadHandle - The handle to the thread to wait for.
	* @param WaitTime - The time to wait for the thread to finish (0 means infinite).
	* @return True if the thread was successfully waited on, false otherwise.
	*/
	static bool WaitForThread(FThreadHandle ThreadHandle, uint64 WaitTime = 0);
};

typedef FWindowsThread FPlatformThread;

#endif
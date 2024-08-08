#include "Windows/WindowsThread.h"

#if PLATFORM_WINDOWS

#include "Windows/WindowsMemory.h"

struct FThreadData
{
	FThreadFunction ThreadFunction;
	void* ThreadParameter;
};

static void ThreadEntry(void* Parameter)
{
	FThreadData* data = (FThreadData*)Parameter;

	data->ThreadFunction(data->ThreadParameter);

	FWindowsMemory::Free(data);
}

FThread FWindowsThread::CreateThread(FThreadFunction ThreadFunction, void* ThreadParameter, bool bCreateSuspended)
{
	FThreadData* data = (FThreadData*)FWindowsMemory::Malloc(sizeof(FThreadData));

	data->ThreadFunction = ThreadFunction;
	data->ThreadParameter = ThreadParameter;

	const DWORD flags = bCreateSuspended ? CREATE_SUSPENDED : 0;

	DWORD threadId{};
	HANDLE threadHandle = ::CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ThreadEntry, data, flags, &threadId);

	return FThread(threadHandle, threadId);
}

bool FWindowsThread::SuspendThread(FThreadHandle Thread)
{
	return ::SuspendThread(Thread) != DWORD(-1);
}

bool FWindowsThread::ResumeThread(FThreadHandle Thread)
{
	return ::ResumeThread(Thread) != DWORD(-1);
}

bool FWindowsThread::WaitForThread(FThreadHandle ThreadHandle, uint64 WaitTime)
{
	return WaitForSingleObject(ThreadHandle, WaitTime == 0 ? INFINITE : (DWORD)WaitTime) == WAIT_OBJECT_0;
}

#endif
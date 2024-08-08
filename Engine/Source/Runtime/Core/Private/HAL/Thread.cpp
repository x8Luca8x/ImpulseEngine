#include "HAL/ImpulseThread.h"

void FThreadBase::Join()
{
	FPlatformThread::WaitForThread(GetHandle());
}

void FThreadBase::Detach()
{
	Thread.ThreadId = INVALID_THREAD_ID;
	Thread.ThreadHandle = INVALID_THREAD_HANDLE;
}

void FThreadBase::Terminate()
{
	// TODO: Implement
}
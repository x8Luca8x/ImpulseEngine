#include "Misc/RefCounter.h"

void FThreadSafeCounter::AddRef()
{
	FPlatformAtomics::InterlockedIncrement(&Counter);
}

void FThreadSafeCounter::AddWeakRef()
{
	FPlatformAtomics::InterlockedIncrement(&WeakCounter);
}

void FThreadSafeCounter::Release()
{
	FPlatformAtomics::InterlockedDecrement(&Counter);
}

void FThreadSafeCounter::ReleaseWeakRef()
{
	FPlatformAtomics::InterlockedDecrement(&WeakCounter);
}

int32 FThreadSafeCounter::GetRefCount() const
{
	return Counter;
}

int32 FThreadSafeCounter::GetWeakRefCount() const
{
	return WeakCounter;
}

bool FThreadSafeCounter::IsThreadSafe() const
{
	return true;
}

void FNotThreadSafeCounter::AddRef()
{
	++Counter;
}

void FNotThreadSafeCounter::AddWeakRef()
{
	++WeakCounter;
}

void FNotThreadSafeCounter::Release()
{
	--Counter;
}

void FNotThreadSafeCounter::ReleaseWeakRef()
{
	--WeakCounter;
}

int32 FNotThreadSafeCounter::GetRefCount() const
{
	return Counter;
}

int32 FNotThreadSafeCounter::GetWeakRefCount() const
{
	return WeakCounter;
}

bool FNotThreadSafeCounter::IsThreadSafe() const
{
	return false;
}
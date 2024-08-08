#include "Misc/ScopeLock.h"

#include "Debug/ImpulseDebug.h"

FScopeLock::FScopeLock(FCriticalSection* InCriticalSection)
{
	checkf(InCriticalSection, TEXT("InCriticalSection is nullptr"));

	m_CriticalSection = InCriticalSection;
	m_CriticalSection->Lock();
}

FScopeLock::~FScopeLock()
{
	m_CriticalSection->Unlock();
}
#include "Windows/WindowsCiriticalSection.h"

#if PLATFORM_WINDOWS

FWindowsCriticalSection::FWindowsCriticalSection()
{
	InitializeCriticalSection(&m_CriticalSection);
}

FWindowsCriticalSection::~FWindowsCriticalSection()
{
	DeleteCriticalSection(&m_CriticalSection);
}

void FWindowsCriticalSection::Lock()
{
	EnterCriticalSection(&m_CriticalSection);
}

void FWindowsCriticalSection::Unlock()
{
	LeaveCriticalSection(&m_CriticalSection);
}

bool FWindowsCriticalSection::TryLock()
{
	return TryEnterCriticalSection(&m_CriticalSection);
}

#endif
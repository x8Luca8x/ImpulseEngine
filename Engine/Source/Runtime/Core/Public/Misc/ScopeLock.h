#pragma once

#include "Platform/PlatformCirticalSection.h"

class CORE_API FScopeLock
{
public:

	FScopeLock(FCriticalSection* InCriticalSection);
	FScopeLock(const FScopeLock&) = delete;
	FScopeLock& operator=(const FScopeLock&) = delete;

	virtual ~FScopeLock();

private:

	FCriticalSection* m_CriticalSection;
};
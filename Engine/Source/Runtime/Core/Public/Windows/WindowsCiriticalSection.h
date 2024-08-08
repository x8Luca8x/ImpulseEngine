#pragma once

#ifndef WINDOWS_CRITICALSECTION_H
#define WINDOWS_CRITICALSECTION_H

#include "Platform/PlatformCirticalSection.h"

#if PLATFORM_WINDOWS

#include <Windows/WindowsAPI.h>

class CORE_API FWindowsCriticalSection : public FGenericPlatformCriticalSection
{
public:

	FWindowsCriticalSection();
	FWindowsCriticalSection(const FWindowsCriticalSection&) = delete;
	FWindowsCriticalSection& operator=(const FWindowsCriticalSection&) = delete;

	virtual ~FWindowsCriticalSection();

	/**
	* Locks the critical section.
	* If another thread has already locked the critical section, this call will block until the lock is released.
	*/
	void Lock();

	/**
	* Unlocks the critical section.
	*/
	void Unlock();

	/**
	* Try to lock the critical section, returning whether it was successful or not.
	* @return true if the lock was acquired, false otherwise.
	*/
	bool TryLock();

public:

	/**
	* WINDOWS ONLY!
	* Gets the critical section.
	* @return The windows critical section.
	*/
	FORCEINLINE CRITICAL_SECTION& GetCriticalSection() { return m_CriticalSection; }

	/**
	* WINDOWS ONLY!
	* Gets the critical section.
	* @return The windows critical section.
	*/
	FORCEINLINE const CRITICAL_SECTION& GetCriticalSection() const { return m_CriticalSection; }

private:

	CRITICAL_SECTION m_CriticalSection;
};

typedef FWindowsCriticalSection FCriticalSection;

#endif // PLATFORM_WINDOWS
#endif // WINDOWS_CRITICALSECTION_H
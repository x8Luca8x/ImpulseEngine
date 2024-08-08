#pragma once

#include "Definitions.h"
#include "Platform/PlatformAtomics.h"

#define PLATFORM_DEFAULT_SMART_POINTER_CLASS ESPMode::Fast

enum class ESPMode : uint8
{
	/// The default smart pointer type for the platform.
	Default = 3,
	/// A shared reference, with thread-safe reference counting.
	ThreadSafe = 1,
	/// A shared reference, with thread-unsafe reference counting.
	NotThreadSafe = 2,
	/// A shared reference, with reference counting disabled.
	Fast = 3,
};

class CORE_API IRefCounterBase
{
public:

	/**
	* Adds a shared reference to the counter.
	*/
	virtual void AddRef() = 0;

	/**
	* Adds a weak reference to the counter.
	*/
	virtual void AddWeakRef() = 0;

	/**
	* Releases a shared reference to the counter.
	*/
	virtual void Release() = 0;

	/**
	* Releases a weak reference to the counter.
	*/
	virtual void ReleaseWeakRef() = 0;

	/**
	* Gets the number of shared references to the counter.
	* @return The number of shared references to the counter.
	*/
	virtual int32 GetRefCount() const = 0;

	/**
	* Gets the number of weak references to the counter.
	* @return The number of weak references to the counter.
	*/
	virtual int32 GetWeakRefCount() const = 0;

	/**
	* Whether the counter is thread-safe.
	* @return True if the counter is thread-safe, false otherwise.
	*/
	virtual bool IsThreadSafe() const = 0;

public:

	/**
	* Gets the total number of references to the counter.
	* @return The total number of references to the counter.
	*/
	int32 GetTotalRefCount() const { return GetRefCount() + GetWeakRefCount(); }
};

/**
* Thread-safe reference counter object.
* Thread-safe counters are more expensive than their non-thread-safe counterparts.
*/
class CORE_API FThreadSafeCounter : public IRefCounterBase
{
public:

	void AddRef() override;
	void AddWeakRef() override;
	void Release() override;
	void ReleaseWeakRef() override;
	int32 GetRefCount() const override;
	int32 GetWeakRefCount() const override;
	bool IsThreadSafe() const override;

private:

	volatile FAtomic Counter{};
	volatile FAtomic WeakCounter{};
};

/**
* Non-thread-safe reference counter object.
* Non-thread-safe counters are less expensive than their thread-safe counterparts.
*/
class CORE_API FNotThreadSafeCounter : public IRefCounterBase
{
public:

	void AddRef() override;
	void AddWeakRef() override;
	void Release() override;
	void ReleaseWeakRef() override;
	int32 GetRefCount() const override;
	int32 GetWeakRefCount() const override;
	bool IsThreadSafe() const override;

private:

	int32 Counter{};
	int32 WeakCounter{};
};

template<ESPMode Mode>
struct TRefCountTraits;

template<>
struct TRefCountTraits<ESPMode::ThreadSafe>
{
	typedef FThreadSafeCounter Type;
};

template<>
struct TRefCountTraits<ESPMode::NotThreadSafe>
{
	typedef FNotThreadSafeCounter Type;
};

template<>
struct TRefCountTraits<ESPMode::Fast>
{
	// Never used, but need to be defined.
	typedef FNotThreadSafeCounter Type;
};
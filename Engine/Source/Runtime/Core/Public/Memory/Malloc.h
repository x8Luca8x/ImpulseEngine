#pragma once

#include "Memory.h"

class CORE_API FMalloc
{
public:

	FMalloc() = default;
	FMalloc(uint64 InMaxAllocationSize) : MaxAllocationSize(InMaxAllocationSize) {}

	FMalloc(const FMalloc&) = delete;
	FMalloc(FMalloc&&) = delete;
	FMalloc& operator=(const FMalloc&) = delete;
	FMalloc& operator=(FMalloc&&) = delete;

	virtual ~FMalloc() = default;

	// @return The global malloc instance.
	static FMalloc* Get();

public:

	void* Malloc(uint64 Size);
	void* Realloc(void* Ptr, uint64 NewSize);
	void Free(void* Ptr);

private:

	// Total memory allocated by this instance.
	uint64 TotalAllocationSize = 0;

	// The maximum size that can be allocated by this instance. (0 if no limit)
	const uint64 MaxAllocationSize = 0;
};
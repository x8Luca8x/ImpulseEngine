#pragma once

#include "CoreModule.h"
#include "Definitions.h"

#include "Memory/NonCopyable.h"

struct FGenericPlatformMemoryConstants
{
	/** The amount of actual physical memory, in bytes (needs to handle >4GB for 64-bit devices running 32-bit code). */
	uint64 TotalPhysical = 0;

	/** The amount of virtual memory, in bytes. */
	uint64 TotalVirtual = 0;

	// AddressLimit - Second parameter is estimate of the range of addresses expected to be returns by BinnedAllocFromOS(). Binned
	// Malloc will adjust its internal structures to make lookups for memory allocations O(1) for this range. 
	// It is ok to go outside this range, lookups will just be a little slower
	const uint64 AddressLimit = (uint64)0xffffffff + 1;
};

struct FGenericPlatformMemoryStats : public FGenericPlatformMemoryConstants
{
	/** The amount of physical memory currently available, in bytes. */
	uint64 AvailablePhysical = 0;

	/** The amount of virtual memory currently available, in bytes. */
	uint64 AvailableVirtual = 0;

	FORCEINLINE uint64 GetUsedPhysical() const
	{
		return TotalPhysical - AvailablePhysical;
	}

	FORCEINLINE uint64 GetUsedVirtual() const
	{
		return TotalVirtual - AvailableVirtual;
	}
};

class CORE_API FBaseVirtualMemoryBlock
{
public:

	FBaseVirtualMemoryBlock()
		: Ptr(nullptr), VMSizeDivVirtualSizeAlignment(0) {}

	FBaseVirtualMemoryBlock(void* InPtr, uint32 InVMSizeDivVirtualSizeAlignment)
		: Ptr(InPtr), VMSizeDivVirtualSizeAlignment(InVMSizeDivVirtualSizeAlignment) {}

	FBaseVirtualMemoryBlock& operator=(const FBaseVirtualMemoryBlock&) = default;
	FBaseVirtualMemoryBlock& operator=(FBaseVirtualMemoryBlock&&) = default;

	FORCEINLINE uint32 GetActualSizeInPages() const
	{
		return VMSizeDivVirtualSizeAlignment;
	}

	FORCEINLINE void* GetVirtualPointer() const
	{
		return Ptr;
	}

private:

	void* Ptr;
	uint32 VMSizeDivVirtualSizeAlignment;
};

class CORE_API FGenericPlatformMemory : public FStaticClass
{
public:

	// Allocates a block of memory of the given size.
	static void* Malloc(size_t Size);

	// Reallocates a block of memory of the given size.
	static void* Realloc(void* Original, size_t Size);

	// Frees a block of memory that was allocated with Malloc or Realloc.
	static void Free(void* Original);

	// Copies a block of memory from one location to another.
	static void* Memcpy(void* Dest, const void* Src, size_t Count);

	// Copies a block of memory from one location to another, allowing the blocks to overlap.
	static void* Memmove(void* Dest, const void* Src, size_t Count);

	// Sets a block of memory to the specified value.
	static void* Memset(void* Dest, uint8 Value, size_t Count);

	// Sets a block of memory to zero.
	static void* Memzero(void* Dest, size_t Count);

	// Compares two blocks of memory.
	static int32 Memcmp(const void* Buf1, const void* Buf2, size_t Count);

	// Compares two blocks of memory ignoring case.
	static int32 Memicmp(const void* Buf1, const void* Buf2, size_t Count);

	static uint64 GetAllocSize(void* Original);
};

typedef FGenericPlatformMemoryStats FPlatformMemoryStats;
typedef FGenericPlatformMemoryConstants FPlatformMemoryConstants;
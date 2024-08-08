#include "Memory/Malloc.h"

FMalloc* FMalloc::Get()
{
	static FMalloc m;
	return &m;
}

void* FMalloc::Malloc(uint64 Size)
{
	if (TotalAllocationSize + Size > MaxAllocationSize)
		return nullptr;

	void* ptr = FMemory::Malloc(Size);
	if (ptr)
		TotalAllocationSize += Size;

	return ptr;
}

void* FMalloc::Realloc(void* Ptr, uint64 NewSize)
{
	if (!Ptr)
		return nullptr;

	uint64 oldSize = FMemory::GetAllocSize(Ptr);
	if (TotalAllocationSize - oldSize + NewSize > MaxAllocationSize)
		return nullptr;

	void* ptr = FMemory::Realloc(Ptr, NewSize);
	if (ptr)
		TotalAllocationSize = TotalAllocationSize - oldSize + NewSize;

	return ptr;
}

void FMalloc::Free(void* Ptr)
{
	if (!Ptr)
		return;

	uint64 oldSize = FMemory::GetAllocSize(Ptr);

	FMemory::Free(Ptr);
	TotalAllocationSize -= oldSize;
}
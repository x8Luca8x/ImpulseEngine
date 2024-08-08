#include "Platform/PlatformMemory.h"

#include <memory>

void* FGenericPlatformMemory::Malloc(size_t Size)
{
	return malloc(Size);
}

void* FGenericPlatformMemory::Realloc(void* Original, size_t Size)
{
	return realloc(Original, Size);
}

void FGenericPlatformMemory::Free(void* Original)
{
	free(Original);
}

void* FGenericPlatformMemory::Memcpy(void* Dest, const void* Src, size_t Count)
{
	return memcpy(Dest, Src, Count);
}

void* FGenericPlatformMemory::Memmove(void* Dest, const void* Src, size_t Count)
{
	return memmove(Dest, Src, Count);
}

void* FGenericPlatformMemory::Memset(void* Dest, uint8 Value, size_t Count)
{
	return memset(Dest, Value, Count);
}

void* FGenericPlatformMemory::Memzero(void* Dest, size_t Count)
{
	return memset(Dest, 0, Count);
}

int32 FGenericPlatformMemory::Memcmp(const void* Buf1, const void* Buf2, size_t Count)
{
	return memcmp(Buf1, Buf2, Count);
}

int32 FGenericPlatformMemory::Memicmp(const void* Buf1, const void* Buf2, size_t Count)
{
	return _memicmp(Buf1, Buf2, Count);
}

uint64 FGenericPlatformMemory::GetAllocSize(void* Original)
{
	return _msize(Original);
}
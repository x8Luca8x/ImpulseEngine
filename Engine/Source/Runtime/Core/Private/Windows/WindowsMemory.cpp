#include "Windows/WindowsMemory.h"

#if PLATFORM_WINDOWS

void* FWindowsMemory::Malloc(uint64 Size)
{
#if USE_WIN_HEAP

	constexpr DWORD flags = WIN_ALLOC_ZEROED ? HEAP_ZERO_MEMORY : 0;
	return WinMalloc(WinGetProcessHeap(), flags, Size);

#else

	return FGenericPlatformMemory::Malloc(Size);

#endif
}

void* FWindowsMemory::Realloc(void* Original, uint64 Size)
{
#if USE_WIN_HEAP

	return WinRealloc(WinGetProcessHeap(), 0, Original, Size);

#else

	return FGenericPlatformMemory::Realloc(Original, Size);

#endif
}

void FWindowsMemory::Free(void* Original)
{
#if USE_WIN_HEAP

	WinFree(WinGetProcessHeap(), 0, Original);

#else

	FGenericPlatformMemory::Free(Original);

#endif
}

uint64 FWindowsMemory::GetAllocSize(void* Original)
{
#if USE_WIN_HEAP

	return WinSize(WinGetProcessHeap(), 0, Original);

#else

	return FGenericPlatformMemory::GetAllocSize(Original);

#endif
}

bool FWindowsMemory::GetStats(FPlatformMemoryStats& OutStats)
{
	MEMORYSTATUSEX status = {};
	if (!WinGetGlobalMemoryStatus(status))
		return false;

	OutStats.TotalPhysical = status.ullTotalPhys;
	OutStats.TotalVirtual = status.ullTotalVirtual;
	OutStats.AvailablePhysical = status.ullAvailPhys;
	OutStats.AvailableVirtual = status.ullAvailVirtual;

	return true;
}

bool FWindowsMemory::FillConstants(FPlatformMemoryConstants& Constants)
{
	MEMORYSTATUSEX status = {};
	if (!WinGetGlobalMemoryStatus(status))
		return false;

	Constants.TotalPhysical = status.ullTotalPhys;
	Constants.TotalVirtual = status.ullTotalVirtual;

	return true;
}

HANDLE FWindowsMemory::WinCreateHeap(DWORD Flags, SIZE_T InitialSize, SIZE_T MaxSize)
{
	return HeapCreate(Flags, InitialSize, MaxSize);
}

bool FWindowsMemory::WinDestroyHeap(HANDLE Heap)
{
	return HeapDestroy(Heap);
}

void* FWindowsMemory::WinMalloc(HANDLE Heap, DWORD Flags, SIZE_T Bytes)
{
	return HeapAlloc(Heap, Flags, Bytes);
}

bool FWindowsMemory::WinFree(HANDLE Heap, DWORD Flags, void* Mem)
{
	return HeapFree(Heap, Flags, Mem);
}

SIZE_T FWindowsMemory::WinSize(HANDLE Heap, DWORD Flags, void* Mem)
{
	return HeapSize(Heap, Flags, Mem);
}

void* FWindowsMemory::WinRealloc(HANDLE Heap, DWORD Flags, void* Mem, SIZE_T NewSize)
{
	return HeapReAlloc(Heap, Flags, Mem, NewSize);
}

HANDLE FWindowsMemory::WinGetProcessHeap()
{
	return GetProcessHeap();
}

bool FWindowsMemory::WinGetGlobalMemoryStatus(MEMORYSTATUSEX& OutStatus)
{
	OutStatus.dwLength = sizeof(OutStatus);
	return GlobalMemoryStatusEx((LPMEMORYSTATUSEX)&OutStatus);
}

#endif
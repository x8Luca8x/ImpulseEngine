#pragma once

#include "Platform/PlatformMemory.h"

#if PLATFORM_WINDOWS

#include <Windows/WindowsAPI.h>

class CORE_API FWindowsMemory : public FGenericPlatformMemory
{
public:

	// Allocates a block of memory.
	static void* Malloc(uint64 Size);

	// Reallocates a block of memory.
	static void* Realloc(void* Original, uint64 Size);

	// Frees a block of memory.
	static void Free(void* Original);

	// Calculates the size of a memory allocation.
	static uint64 GetAllocSize(void* Original);

	static bool GetStats(FPlatformMemoryStats& OutStats);

	static bool FillConstants(FPlatformMemoryConstants& Constants);

public:

	// WINDOWS ONLY!!!

	// Windows only
	static HANDLE WinCreateHeap(DWORD Flags, SIZE_T InitialSize, SIZE_T MaxSize);

	// Windows only
	static bool WinDestroyHeap(HANDLE Heap);

	// Windows only
	static void* WinMalloc(HANDLE Heap, DWORD Flags, SIZE_T Bytes);

	// Windows only
	static bool WinFree(HANDLE Heap, DWORD Flags, void* Mem);

	// Windows only
	static SIZE_T WinSize(HANDLE Heap, DWORD Flags, void* Mem);

	// Windows only
	static void* WinRealloc(HANDLE Heap, DWORD Flags, void* Mem, SIZE_T NewSize);

	// Windows only
	static HANDLE WinGetProcessHeap();

	// Windows only
	static bool WinGetGlobalMemoryStatus(MEMORYSTATUSEX& OutStatus);
};

typedef FWindowsMemory FPlatformMemory;

#endif
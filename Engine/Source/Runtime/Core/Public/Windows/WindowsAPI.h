#pragma once

#include "Definitions.h"

#if PLATFORM_WINDOWS

#pragma push_macro("TEXT")
#undef TEXT

#include <Windows.h>
#undef TEXT

#pragma push_macro("CreateDirectory")
#undef CreateDirectory

#pragma push_macro("CreateFile")
#undef CreateFile

#pragma push_macro("DeleteFile")
#undef DeleteFile

#pragma push_macro("MoveFile")
#undef MoveFile

#pragma push_macro("CopyFile")
#undef CopyFile

#pragma push_macro("CreateFile")
#undef CreateFile

#pragma push_macro("GetCurrentDirectory")
#undef GetCurrentDirectory

#pragma push_macro("SetCurrentDirectory")
#undef SetCurrentDirectory

#pragma push_macro("GetFileAttributes")
#undef GetFileAttributes

#pragma push_macro("SetFileAttributes")
#undef SetFileAttributes

#pragma push_macro("FindFirstFile")
#undef FindFirstFile

#pragma push_macro("FindNextFile")
#undef FindNextFile

#pragma push_macro("FindClose")
#undef FindClose

#pragma push_macro("GetFileAttributesEx")
#undef GetFileAttributesEx

#pragma push_macro("GetFullPathName")
#undef GetFullPathName

#pragma push_macro("InterlockedIncrement")
#undef InterlockedIncrement

#pragma push_macro("InterlockedDecrement")
#undef InterlockedDecrement

#pragma push_macro("InterlockedExchange")
#undef InterlockedExchange

#pragma push_macro("InterlockedAdd")
#undef InterlockedAdd

#pragma push_macro("InterlockedCompareExchange")
#undef InterlockedCompareExchange

#pragma push_macro("InterlockedAnd")
#undef InterlockedAnd

#pragma push_macro("InterlockedOr")
#undef InterlockedOr

#pragma push_macro("InterlockedXor")
#undef InterlockedXor

#pragma push_macro("InterlockedIncrement64")
#undef InterlockedIncrement64

#pragma push_macro("InterlockedDecrement64")
#undef InterlockedDecrement64

#pragma push_macro("InterlockedExchange64")
#undef InterlockedExchange64

#pragma push_macro("InterlockedAdd64")
#undef InterlockedAdd64

#pragma push_macro("InterlockedCompareExchange64")
#undef InterlockedCompareExchange64

#pragma push_macro("InterlockedAnd64")
#undef InterlockedAnd64

#pragma push_macro("InterlockedOr64")
#undef InterlockedOr64

#pragma push_macro("InterlockedXor64")
#undef InterlockedXor64

#pragma push_macro("GetObject")
#undef GetObject

#pragma pop_macro("TEXT")

namespace Windows
{
	FORCEINLINE BOOL IsValidHandle(HANDLE Handle)
	{
		return Handle && Handle != INVALID_HANDLE_VALUE;
	}
}

#endif
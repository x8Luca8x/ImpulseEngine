#include "Windows/WindowsDebug.h"

#if PLATFORM_WINDOWS

#include "Platform/PlatformString.h"
#include "Memory/Memory.h"

#include <cstdarg>

FWindowsDebug GDebug;

#define ASSERTION_FAILED_FORMAT_STRING "%s (%d) Assertion failed (%s): %s\n"
#define ASSERTION_FAILED_FORMAT_STRINGW _CRT_WIDE(ASSERTION_FAILED_FORMAT_STRING)

void FWindowsDebug::LowLevelDebugBreak()
{
	DebugBreak();
}

void FWindowsDebug::LowLevelOutputDebugString(const ANSICHAR* String)
{
	OutputDebugStringA(String);
}

void FWindowsDebug::LowLevelOutputDebugStringW(const WIDECHAR* String)
{
	OutputDebugStringW(String);
}

void FWindowsDebug::AssertFailed(const ANSICHAR* Expr, const ANSICHAR* File, int32 Line, const ANSICHAR* Format, ...)
{
	ANSICHAR buffer1[MAX_ASSERTION_MESSAGE_LENGTH];
	ANSICHAR buffer2[MAX_ASSERTION_FORMAT_LENGTH];

	FMemory::Memzero(buffer1, MAX_ASSERTION_MESSAGE_LENGTH);
	FMemory::Memzero(buffer2, MAX_ASSERTION_FORMAT_LENGTH);

	va_list args;
	va_start(args, Format);

	FPlatformANSIString::Vsnprintf(buffer2, MAX_ASSERTION_FORMAT_LENGTH, Format, args);

	va_end(args);

	FPlatformANSIString::Sprintf(buffer1, MAX_ASSERTION_MESSAGE_LENGTH, ASSERTION_FAILED_FORMAT_STRING, File, Line, Expr, buffer2);
	LowLevelOutputDebugString(buffer1);
}

void FWindowsDebug::AssertFailed(const WIDECHAR* Expr, const WIDECHAR* File, int32 Line, const WIDECHAR* Format, ...)
{
	WIDECHAR buffer1[MAX_ASSERTION_MESSAGE_LENGTH];
	WIDECHAR buffer2[MAX_ASSERTION_FORMAT_LENGTH];

	va_list args;
	va_start(args, Format);

	FPlatformWIDEString::Vsnprintf(buffer2, MAX_ASSERTION_FORMAT_LENGTH, Format, args);

	va_end(args);

	FPlatformWIDEString::Sprintf(buffer1, MAX_ASSERTION_MESSAGE_LENGTH, ASSERTION_FAILED_FORMAT_STRINGW, File, Line, Expr, buffer2);
	LowLevelOutputDebugStringW(buffer1);
}

#endif
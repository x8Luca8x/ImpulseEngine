#pragma once

#include "CoreModule.h"
#include "Definitions.h"

#include "Memory/NonCopyable.h"

#include <corecrt.h>

#define MAX_ASSERTION_FORMAT_LENGTH 128
#define MAX_ASSERTION_MESSAGE_LENGTH 4096

#define check(expr) if(!(!!expr)) { GDebug.AssertFailed(TEXT(#expr), __FILEW__, __LINE__, TEXT(#expr)); __debugbreak(); }
#define checkf(expr, format, ...) if(!(expr)) { GDebug.AssertFailed(TEXT(#expr), __FILEW__, __LINE__, format, ##__VA_ARGS__); __debugbreak(); }

class CORE_API FDebug
{
public:

	virtual void LowLevelDebugBreak() = 0;

	virtual void LowLevelOutputDebugString(const ANSICHAR* String) = 0;
	virtual void LowLevelOutputDebugStringW(const WIDECHAR* String) = 0;

	virtual void AssertFailed(const ANSICHAR* Expr, const ANSICHAR* File, int32 Line, const ANSICHAR* Format = nullptr, ...) = 0;
	virtual void AssertFailed(const WIDECHAR* Expr, const WIDECHAR* File, int32 Line, const WIDECHAR* Format = nullptr, ...) = 0;
};

#if PLATFORM_WINDOWS
#include "Windows/WindowsDebug.h"
#endif
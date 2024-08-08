#pragma once

#include "Debug/ImpulseDebug.h"

#if PLATFORM_WINDOWS

#include "WindowsAPI.h"

class CORE_API FWindowsDebug : public FDebug
{
public:

	virtual void LowLevelDebugBreak() override;

	virtual void LowLevelOutputDebugString(const ANSICHAR* String) override;
	virtual void LowLevelOutputDebugStringW(const WIDECHAR* String) override;

	virtual void AssertFailed(const ANSICHAR* Expr, const ANSICHAR* File, int32 Line, const ANSICHAR* Format = nullptr, ...) override;
	virtual void AssertFailed(const WIDECHAR* Expr, const WIDECHAR* File, int32 Line, const WIDECHAR* Format = nullptr, ...) override;
};

typedef FWindowsDebug FPlatformDebug;

extern CORE_API FPlatformDebug GDebug;

#endif
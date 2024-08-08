#include "Windows/WindowsAtomics.h"

#if PLATFORM_WINDOWS

#include <Windows/WindowsAPI.h>

FAtomic FWindowsAtomics::InterlockedIncrement(FAtomic volatile* ValuePtr)
{
	return _InterlockedIncrement(ValuePtr);
}

FAtomic FWindowsAtomics::InterlockedDecrement(FAtomic volatile* ValuePtr)
{
	return ::_InterlockedDecrement(ValuePtr);
}

FAtomic FWindowsAtomics::InterlockedExchange(FAtomic volatile* ValuePtr, FAtomic Value)
{
	return ::_InterlockedExchange(ValuePtr, Value);
}

FAtomic FWindowsAtomics::InterlockedAdd(FAtomic volatile* ValuePtr, FAtomic Value)
{
	return ::_InlineInterlockedAdd(ValuePtr, Value);
}

FAtomic FWindowsAtomics::InterlockedCompareExchange(FAtomic volatile* ValuePtr, FAtomic Exchange, FAtomic Comperand)
{
	return ::_InterlockedCompareExchange(ValuePtr, Exchange, Comperand);
}

FAtomic FWindowsAtomics::InterlockedAnd(FAtomic volatile* ValuePtr, FAtomic Value)
{
	return ::_InterlockedAnd(ValuePtr, Value);
}

FAtomic FWindowsAtomics::InterlockedOr(FAtomic volatile* ValuePtr, FAtomic Value)
{
	return ::_InterlockedOr(ValuePtr, Value);
}

FAtomic FWindowsAtomics::InterlockedXor(FAtomic volatile* ValuePtr, FAtomic Value)
{
	return ::_InterlockedXor(ValuePtr, Value);
}

FAtomic64 FWindowsAtomics::InterlockedIncrement64(FAtomic64 volatile* ValuePtr)
{
	return ::_InterlockedIncrement64(ValuePtr);
}

FAtomic64 FWindowsAtomics::InterlockedDecrement64(FAtomic64 volatile* ValuePtr)
{
	return ::_InterlockedDecrement64(ValuePtr);
}

FAtomic64 FWindowsAtomics::InterlockedExchange64(FAtomic64 volatile* ValuePtr, FAtomic64 Value)
{
	return ::_InterlockedExchange64(ValuePtr, Value);
}

FAtomic64 FWindowsAtomics::InterlockedAdd64(FAtomic64 volatile* ValuePtr, FAtomic64 Value)
{
	return ::_InlineInterlockedAdd64(ValuePtr, Value);
}

FAtomic64 FWindowsAtomics::InterlockedCompareExchange64(FAtomic64 volatile* ValuePtr, FAtomic64 Exchange, FAtomic64 Comperand)
{
	return ::_InterlockedCompareExchange64(ValuePtr, Exchange, Comperand);
}

FAtomic64 FWindowsAtomics::InterlockedAnd64(FAtomic64 volatile* ValuePtr, FAtomic64 Value)
{
	return ::_InterlockedAnd64(ValuePtr, Value);
}

FAtomic64 FWindowsAtomics::InterlockedOr64(FAtomic64 volatile* ValuePtr, FAtomic64 Value)
{
	return ::_InterlockedOr64(ValuePtr, Value);
}

FAtomic64 FWindowsAtomics::InterlockedXor64(FAtomic64 volatile* ValuePtr, FAtomic64 Value)
{
	return ::_InterlockedXor64(ValuePtr, Value);
}

#endif
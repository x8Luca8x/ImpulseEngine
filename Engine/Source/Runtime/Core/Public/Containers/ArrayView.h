#pragma once

#include "CoreModule.h"

template<typename T>
class TArrayView
{
public:
	TArrayView(T* InData, size_t InSize)
		: Data(InData)
		, Size(InSize) {}

	T* begin() const
	{
		return Data;
	}

	T* end() const
	{
		return Data + Size;
	}

	T& operator[](int32 Index) const
	{
		return Data[Index];
	}

	int32 Num() const
	{
		return Size;
	}

private:

	T* Data;
	size_t Size;
};
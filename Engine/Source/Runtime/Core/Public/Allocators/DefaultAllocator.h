#pragma once

#include "Memory/Memory.h"

#include <iostream>

template<typename T>
class TDefaultAllocator
{
public:

	TDefaultAllocator() = default;
	TDefaultAllocator(const TDefaultAllocator&) = delete;

	TDefaultAllocator(TDefaultAllocator&& Other) noexcept
	{
		m_Size = Other.m_Size;
		m_Allocation = Other.m_Allocation;

		Other.m_Size = 0;
		Other.m_Allocation = nullptr;
	}

	~TDefaultAllocator()
	{
		Free();
	}

	TDefaultAllocator& operator=(const TDefaultAllocator&) = delete;
	TDefaultAllocator& operator=(TDefaultAllocator&& Other) noexcept
	{
		Free();

		m_Size = Other.m_Size;
		m_Allocation = Other.m_Allocation;

		Other.m_Size = 0;
		Other.m_Allocation = nullptr;

		return *this;

	}

	TDefaultAllocator& operator=(NULLPTR_T)
	{
		return *this;
	}

	void Allocate(uint64 Size)
	{
		m_Size = Size;
		m_Allocation = (T*)FMemory::Malloc(Size);
	}

	void Reallocate(uint64 NewSize)
	{
		m_Size = NewSize;
		m_Allocation = (T*)FMemory::Realloc(m_Allocation, NewSize);
	}

	void Free()
	{
		FMemory::Free(m_Allocation);

		m_Size = 0;
		m_Allocation = nullptr;
	}

	uint64 GetSize() const
	{
		return m_Size;
	}

	T* GetAllocation() const
	{
		return m_Allocation;
	}

private:

	uint64 m_Size = 0;
	T* m_Allocation = nullptr;
};
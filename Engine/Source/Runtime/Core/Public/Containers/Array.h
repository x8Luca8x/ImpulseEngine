#pragma once

#include "Math/Math.h"
#include "Templates/ImpulseTemplates.h"
#include "Allocators/DefaultAllocator.h"

#include "Windows/WindowsDebug.h"

#include <initializer_list>

template<typename T>
class TArrayIterator
{
public:

	TArrayIterator(T* InPtr) : m_Ptr(InPtr) {}

	FORCEINLINE TArrayIterator& operator++() { ++m_Ptr; return *this; }
	FORCEINLINE TArrayIterator& operator--() { --m_Ptr; return *this; }

	FORCEINLINE TArrayIterator operator++(int32) { TArrayIterator temp = *this; ++m_Ptr; return temp; }
	FORCEINLINE TArrayIterator operator--(int32) { TArrayIterator temp = *this; --m_Ptr; return temp; }

	FORCEINLINE T& operator*() const { return *m_Ptr; }
	FORCEINLINE T* operator->() const { return m_Ptr; }

	FORCEINLINE bool operator==(const TArrayIterator& Other) const { return m_Ptr == Other.m_Ptr; }
	FORCEINLINE bool operator!=(const TArrayIterator& Other) const { return m_Ptr != Other.m_Ptr; }

private:

	T* m_Ptr = nullptr;
};

template<typename T>
class TArrayConstIterator
{
public:

	TArrayConstIterator(const T* InPtr) : m_Ptr(InPtr) {}

	FORCEINLINE TArrayConstIterator& operator++() { ++m_Ptr; return *this; }
	FORCEINLINE TArrayConstIterator& operator--() { --m_Ptr; return *this; }

	FORCEINLINE TArrayConstIterator operator++(int32) { TArrayConstIterator temp = *this; ++m_Ptr; return temp; }
	FORCEINLINE TArrayConstIterator operator--(int32) { TArrayConstIterator temp = *this; --m_Ptr; return temp; }

	FORCEINLINE const T& operator*() const { return *m_Ptr; }
	FORCEINLINE const T* operator->() const { return m_Ptr; }

	FORCEINLINE bool operator==(const TArrayConstIterator& Other) const { return m_Ptr == Other.m_Ptr; }
	FORCEINLINE bool operator!=(const TArrayConstIterator& Other) const { return m_Ptr != Other.m_Ptr; }

private:

	const T* m_Ptr = nullptr;
};

template<typename T, typename Allocator = TDefaultAllocator<T>>
class TArray
{
public:

	TArray() = default;
	TArray(const TArray& Other);
	TArray(TArray&& Other) noexcept;
	TArray(std::initializer_list<T> InitList) { Init(InitList.begin(), static_cast<int32>(InitList.size())); }
	TArray(const T* InData, int32 InCount) { Init(InData, InCount); }
	~TArray();

	TArray& operator=(const TArray& Other);
	TArray& operator=(TArray&& Other) noexcept;

	// @return Number of elements that are actually stored in the array.
	FORCEINLINE int32 Num() const { return m_Num; }

	// @return Number of elements that can be stored in the array without reallocation.
	FORCEINLINE int32 Capacity() const { return GetAllocatedNum(); }

	/**
	* Checks if the given address is inside the array.
	* @param Address - Address to check.
	*/
	FORCEINLINE bool ContainsAddress(const void* Address) const
	{
		const uint8* start = reinterpret_cast<const uint8*>(m_Allocator.GetAllocation());
		const uint8* end = start + m_Num * sizeof(T);

		return Address >= start && Address < end;
	}

	/**
	* Checks if the given address is inside the array.
	* If is, an assertion is triggered.
	* @param Address - Address to check.
	*/
	FORCEINLINE void CheckAddress(const void* Address) const
	{
		checkf(!ContainsAddress(Address), TEXT("Address '%p' is inside the array!"), Address);
	}

	// Initializes an empty array.
	void Init(int32 InReserve = 0);

	// Initializes an empty array with data.
	// @param Data Data to initialize the array with.
	// @param Count Number of elements in the data.
	void Init(const T* InData, int32 InCount);

	// Releases all memory used by the array.
	void Empty(int32 InSlack = 0);

	// Shrinks the array to fit the number of elements actually stored in it.
	void Shrink();

	// Reserves memory for the specified number of elements.
	// This function is supposed to used with "Add" function or "Emplace".
	// @param Reserve Number of elements to reserve memory for.
	void Reserve(int32 InReserve);

	// Sets the number of elements that are actually stored in the array and initializes them.
	// @param Num Number of elements to set.
	void SetNum(int32 InNum);

	// Sets the number of elements that are actually stored in the array without initializing them.
	// You must call "ConstructItems" before emptying the array!
	// @param Num Number of elements to set.
	void SetNumUninitialized(int32 Num);

	// Inserts an element at the specified index.
	// @param Index - Index to insert the element at.
	// @param Element - Element to insert.
	void Insert(int32 Index, const T& Element);

	// Inserts an element at the specified index.
	// @param Index - Index to insert the element at.
	// @param Element - Element to insert.
	void Insert(int32 Index, T&& Element);

	// Adds an uninitialized element to the end of the array.
	// @return Index of the added element.
	int32 AddUninitialized();

	// Adds an element to the end of the array.
	// @param Element - Element to add.
	// @return Index of the added element.
	int32 Add(const T& Element);

	// Adds an element to the end of the array.
	// @param Element - Element to add.
	// @return Index of the added element.
	int32 Add(T&& Element);

	// Removes an element
	// @param Element - Element to remove.
	// @return Number of elements removed.
	int32 Remove(const T& Element);

	// Removes an element at the specified index.
	// @param Index - Index to remove the element at.
	void RemoveAt(int32 Index);

	// Removes an element at the specified index and replaces it with the last element in the array.
	// This function is faster then "RemoveAt" but does not preserve the order of elements and reserved memory.
	// @param Index - Index to remove the element at.
	void RemoveAtSwap(int32 Index);

	// Finds an element in the array.
	// @param Element - Element to find.
	// @return Index of the element or -1 if the element was not found.
	int32 Find(const T& Element) const;

	// Finds all elements in the array.
	// @param Element - Element to find.
	// @return Array of indices of the elements or empty array if the element was not found.
	TArray<int32> FindAll(const T& Element) const;

	// Checks if the array contains the specified element.
	// @param Element - Element to check.
	// @return True if the array contains the element.
	FORCEINLINE bool Contains(const T& Element) const { return Find(Element) != INDEX_NONE; }

	// Constructs an element to the end of the array.
	// Difference between this function and "Add" is that this function only constructs the element and does not fit the array to the number of elements.
	// @param Args - Arguments to construct the element with.
	// @return Index of the added element.
	template<typename... Args>
	FORCEINLINE int32 Emplace(Args&&... InArgs)
	{
		const int32 index = AddUninitialized();

		new (m_Allocator.GetAllocation() + index) T(Forward<Args>(InArgs)...);
		return index;
	}

	// Constructs an element to the specified index.
	// Difference between this function and "Insert" is that this function only constructs the element and does not destruct the element that was previously stored at the index.
	// @param Index - Index to construct the element at.
	// @param Args - Arguments to construct the element with.
	// @return Index of the added element.
	template<typename... Args>
	FORCEINLINE int32 EmplaceAt(int32 Index, Args&&... InArgs)
	{
		CheckRange(Index);

		new (m_Allocator.GetAllocation() + Index) T(Forward<Args>(InArgs)...);
		return Index;
	}

	// Checks if an index is in range of the array.
	// @param Index Index to check.
	// @return True if the index is in range of the array.
	FORCEINLINE bool IsValidIndex(int32 Index) const { return Index >= 0 && Index < m_Num; }

	// Checks if an index is in range of the array.
	// If not, an assertion is triggered.
	// @param Index Index to check.
	FORCEINLINE void CheckRange(int32 Index) const { checkf(IsValidIndex(Index), TEXT("Index %d is out of range of array with %d elements."), Index, m_Num); }

	// @return True if the array is empty.
	bool IsEmpty() const { return m_Allocator.GetAllocation() == nullptr; }

	// @return A pointer to the first element in the array.
	T* GetData() { return m_Allocator.GetAllocation(); }

	// @return A pointer to the first element in the array.
	const T* GetData() const { return m_Allocator.GetAllocation(); }

	// @return A reference to the first element in the array.
	T& First() { return m_Allocator.GetAllocation()[0]; }

	// @return A reference to the first element in the array.
	const T& First() const { return m_Allocator.GetAllocation()[0]; }

	T& operator[](int32 Index) { CheckRange(Index); return m_Allocator.GetAllocation()[Index]; }
	const T& operator[](int32 Index) const { CheckRange(Index); return m_Allocator.GetAllocation()[Index]; }

	bool operator==(const TArray& Other) const
	{
		if (m_Num != Other.m_Num)
			return false;

		for (int32 i = 0; i < m_Num; ++i)
			if (m_Allocator.GetAllocation()[i] != Other.m_Allocator.GetAllocation()[i])
				return false;

		return true;
	}

	// @return An iterator to the first element in the array.
	TArrayIterator<T> begin() { return TArrayIterator<T>(m_Allocator.GetAllocation()); }

	// @return An iterator to the first element in the array.
	TArrayConstIterator<T> begin() const { return TArrayConstIterator<T>(m_Allocator.GetAllocation()); }

	// @return An iterator to the element after the last element in the array.
	TArrayIterator<T> end() { return TArrayIterator<T>(m_Allocator.GetAllocation() + m_Num); }

	// @return An iterator to the element after the last element in the array.
	TArrayConstIterator<T> end() const { return TArrayConstIterator<T>(m_Allocator.GetAllocation() + m_Num); }

private:

	// Allocates memory for the specified number of elements.
	// If the array has already been allocated, it will be reallocated.
	// If InNewNum is less then the number of elements already stored in the array, the array will be destructed and truncated.
	void Allocate(int32 InNum);

	// @return True if the array needs to be resized to fit the specified number of elements.
	bool NeedsResize(int32 InNum) const;

	static void CopyToEmpty(T* This, const T* Other, int32 Count);
	static void MoveToEmpty(T* This, T* Other, int32 Count);

	static void CopyToExisting(T* This, const T* Other, int32 Count);
	static void MoveToExisting(T* This, T* Other, int32 Count);

	// @return Number of elements that can be theorethically stored in currently allocated memory.
	int32 GetAllocatedNum() const { return static_cast<int32>(m_Allocator.GetSize() / sizeof(T)); }

	// @return Number of elements that can be added to the array without reallocation.
	int32 GetReservedNum() const { return GetAllocatedNum() - m_Num; }

private:

	// Number of elements that are actually stored in the array.
	int32 m_Num = 0;

	// Allocator used to allocate memory for the array.
	Allocator m_Allocator;
};

template<typename T, typename Allocator>
inline TArray<T, Allocator>::TArray(const TArray& Other)
{
	Init(Other.GetData(), Other.Num());
}

template<typename T, typename Allocator>
inline TArray<T, Allocator>::TArray(TArray&& Other) noexcept
{
	m_Num = Other.m_Num;
	m_Allocator = MoveTemp(Other.m_Allocator);

	Other.m_Num = 0;
	m_Allocator = nullptr;
}

template<typename T, typename Allocator>
inline TArray<T, Allocator>::~TArray()
{
	Empty();
}

template<typename T, typename Allocator>
inline TArray<T, Allocator>& TArray<T, Allocator>::operator=(const TArray& Other)
{
	if (this != &Other)
	{
		Empty();
		Init(Other.GetData(), Other.Num());
	}

	return *this;
}

template<typename T, typename Allocator>
inline TArray<T, Allocator>& TArray<T, Allocator>::operator=(TArray&& Other) noexcept
{
	if (this != &Other)
	{
		Empty();

		m_Num = Other.m_Num;
		m_Allocator = MoveTemp(Other.m_Allocator);

		Other.m_Num = 0;
		m_Allocator = nullptr;
	}

	return *this;
}

template<typename T, typename Allocator>
inline void TArray<T, Allocator>::Init(int32 InReserve)
{
	checkf(IsEmpty(), TEXT("Array is already initialized."));
	checkf(InReserve > 0, TEXT("Reserve must be greater then zero."));

	Reserve(InReserve);
}

template<typename T, typename Allocator>
inline void TArray<T, Allocator>::Init(const T* InData, int32 InCount)
{
	checkf(IsEmpty(), TEXT("Array is already initialized."));

	if (!InData || InCount <= 0)
		return;

	SetNumUninitialized(InCount);
	CopyToEmpty(m_Allocator.GetAllocation(), InData, InCount);
}

template<typename T, typename Allocator>
inline void TArray<T, Allocator>::Empty(int32 InSlack)
{
	// Destruct all elements.

	DestructItems<T>(m_Allocator.GetAllocation(), m_Num);

	// Free memory.

	m_Num = 0;
	m_Allocator.Free();

	// If InSlack is greater then zero, reserve memory for the specified number of elements.

	if (InSlack > 0)
		Reserve(InSlack);
}

template<typename T, typename Allocator>
inline void TArray<T, Allocator>::Shrink()
{
	const int32 shrinkSize = GetReservedNum();
	if (shrinkSize > 0)
		Allocate(m_Num - shrinkSize);
}

template<typename T, typename Allocator>
inline void TArray<T, Allocator>::Reserve(int32 InReserve)
{
	checkf(InReserve > 0, TEXT("Reserve must be greater then zero."));

	const int32 allocNum = GetAllocatedNum() + InReserve;
	Allocate(allocNum);
}

template<typename T, typename Allocator>
inline void TArray<T, Allocator>::SetNum(int32 InNum)
{
	const int32 oldNum = m_Num;

	SetNumUninitialized(InNum);
	if (InNum > oldNum)
		ConstructItems<T>(m_Allocator.GetAllocation() + oldNum, InNum - oldNum);
}

template<typename T, typename Allocator>
inline void TArray<T, Allocator>::SetNumUninitialized(int32 InNum)
{
	Allocate(InNum);
	m_Num = InNum;
}

template<typename T, typename Allocator>
inline void TArray<T, Allocator>::Insert(int32 Index, const T& Element)
{
	CheckRange(Index);
	CheckAddress(&Element);

	DestructItem<T>(m_Allocator.GetAllocation() + Index);
	CopyToEmpty(m_Allocator.GetAllocation() + Index, &Element, 1);
}

template<typename T, typename Allocator>
inline void TArray<T, Allocator>::Insert(int32 Index, T&& Element)
{
	CheckRange(Index);
	CheckAddress(&Element);

	DestructItem<T>(m_Allocator.GetAllocation() + Index);
	MoveToEmpty(m_Allocator.GetAllocation() + Index, &Element, 1);
}

template<typename T, typename Allocator>
inline int32 TArray<T, Allocator>::AddUninitialized()
{
	if (GetReservedNum() <= 0)
		Reserve(1);

	return m_Num++;
}

template<typename T, typename Allocator>
inline int32 TArray<T, Allocator>::Add(const T& Element)
{
	CheckAddress(&Element);

	return Emplace(Element);
}

template<typename T, typename Allocator>
inline int32 TArray<T, Allocator>::Add(T&& Element)
{
	CheckAddress(&Element);
	return Emplace(MoveTempIfPossible(Element));
}

template<typename T, typename Allocator>
inline int32 TArray<T, Allocator>::Remove(const T& Element)
{
	TArray<int32> indices = FindAll(Element);

	for (int32 i = 0; i < indices.Num(); ++i)
		RemoveAtSwap(indices[i] - i);

	return indices.Num();
}

template<typename T, typename Allocator>
inline void TArray<T, Allocator>::RemoveAt(int32 Index)
{
	CheckRange(Index);

	DestructItem<T>(m_Allocator.GetAllocation() + Index);

	FMemory::Memmove(m_Allocator.GetAllocation() + Index, m_Allocator.GetAllocation() + Index + 1, (m_Num - Index - 1) * sizeof(T));
	--m_Num;

	Allocate(m_Num);
}

template<typename T, typename Allocator>
inline void TArray<T, Allocator>::RemoveAtSwap(int32 Index)
{
	CheckRange(Index);

	DestructItem<T>(m_Allocator.GetAllocation() + Index);

	Swap(m_Allocator.GetAllocation()[Index], m_Allocator.GetAllocation()[m_Num - 1]);

	--m_Num;
	Allocate(m_Num);
}

template<typename T, typename Allocator>
inline int32 TArray<T, Allocator>::Find(const T& Element) const
{
	for (int32 i = 0; i < m_Num; ++i)
		if (m_Allocator.GetAllocation()[i] == Element)
			return i;

	return INDEX_NONE;
}

template<typename T, typename Allocator>
inline TArray<int32> TArray<T, Allocator>::FindAll(const T& Element) const
{
	TArray<int32> indices;

	for (int32 i = 0; i < m_Num; ++i)
		if (m_Allocator.GetAllocation()[i] == Element)
			indices.Add(i);

	return indices;
}

template<typename T, typename Allocator>
inline void TArray<T, Allocator>::Allocate(int32 InNum)
{
	if(InNum <= 0)
	{
		Empty();
		return;
	}

	// If new number of elements is equal to the number of elements that can be stored in currently allocated memory, do nothing.

	if (InNum == GetAllocatedNum())
		return;

	if (IsEmpty())
		// Allocate memory for the specified number of elements.
		m_Allocator.Allocate(InNum * sizeof(T));
	else
	{
		// Before reallocation, destruct overflowing elements.

		if (InNum < m_Num)
			DestructItems<T>(m_Allocator.GetAllocation() + InNum, m_Num - InNum);

		m_Allocator.Reallocate(InNum * sizeof(T));
	}
}

template<typename T, typename Allocator>
inline bool TArray<T, Allocator>::NeedsResize(int32 InNum) const
{
	return InNum > GetReservedNum();
}

template<typename T, typename Allocator>
inline void TArray<T, Allocator>::CopyToEmpty(T* This, const T* Other, int32 Count)
{
	for (int32 i = 0; i < Count; ++i)
		ConstructItem<T>(This + i, CopyTemp(Other[i]));
}

template<typename T, typename Allocator>
inline void TArray<T, Allocator>::MoveToEmpty(T* This, T* Other, int32 Count)
{
	for (int32 i = 0; i < Count; ++i)
		ConstructItem<T>(This + i, MoveTemp(Other[i]));
}

template<typename T, typename Allocator>
inline void TArray<T, Allocator>::CopyToExisting(T* This, const T* Other, int32 Count)
{
	for (int32 i = 0; i < Count; ++i)
		This[i] = CopyTemp(Other[i]);
}

template<typename T, typename Allocator>
inline void TArray<T, Allocator>::MoveToExisting(T* This, T* Other, int32 Count)
{
	for (int32 i = 0; i < Count; ++i)
		This[i] = MoveTemp(Other[i]);
}
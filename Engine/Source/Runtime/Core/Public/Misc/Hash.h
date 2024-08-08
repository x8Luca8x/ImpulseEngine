#pragma once

#include "Definitions.h"
#include "Templates/ImpulseTemplates.h"

namespace IE::Private
{
	FORCEINLINE uint32 MurmurFinalize32(uint32 Hash)
	{
		Hash ^= Hash >> 16;
		Hash *= 0x85ebca6b;
		Hash ^= Hash >> 13;
		Hash *= 0xc2b2ae35;
		Hash ^= Hash >> 16;
		return Hash;
	}
}

/**
 * Combines two hash values to get a third.
 * Note - this function is not commutative.
 *
 * WARNING!  This function is subject to change and should only be used for creating
 *           combined hash values which don't leave the running process,
 *           e.g. GetTypeHash() overloads.
 */
inline uint32 HashCombineFast(uint32 A, uint32 B)
{
	return A ^ (B + 0x9e3779b9 + (A << 6) + (A >> 2));
}

inline uint32 PointerHash(const void* Ptr)
{
	// Ignoring the lower 4 bits since they are likely zero anyway.
	// Higher bits are more significant in 64 bit builds.
	const UPTRINT PtrInt = reinterpret_cast<UPTRINT>(Ptr) >> 4;
	return IE::Private::MurmurFinalize32((uint32)PtrInt);
}

inline uint32 PointerHash(const void* Key, uint32 C)
{
	// we can use HashCombineFast here because pointers are non-persistent
	return HashCombineFast(PointerHash(Key), C);
}

template<typename ScalarType, TEnableIf<TIsScalar<ScalarType>::Value && !ARE_TYPES_EQUAL(ScalarType, TCHAR*) && !ARE_TYPES_EQUAL(ScalarType, const TCHAR*)>* = nullptr>
inline uint32 GetTypeHash(ScalarType Value)
{
	if constexpr (TIsIntegral<ScalarType>::Value)
	{
		if constexpr (sizeof(ScalarType) <= 4)
			return Value;
		else if constexpr (sizeof(ScalarType) == 8)
			return (uint32)Value + ((uint32)(Value >> 32) * 23);
		else if constexpr (sizeof(ScalarType) == 16)
		{
			const uint64 low = (uint64)Value;
			const uint64 high = (uint64)(Value >> 64);

			return GetTypeHash(low) ^ GetTypeHash(high);
		}
		else
		{
			static_assert(sizeof(ScalarType) == 0, "Unsupported integral type");
		}
	}
	else if constexpr (TIsFloatingPoint<ScalarType>::Value)
	{
		if constexpr (ARE_TYPES_EQUAL(ScalarType, float))
			return *(uint32*)&Value;
		else if constexpr (ARE_TYPES_EQUAL(ScalarType, double))
			return GetTypeHash(*(uint64*)&Value);
		else
		{
			static_assert(sizeof(ScalarType) == 0, "Unsupported floating point type");
		}
	}
	else if constexpr (TIsEnum<ScalarType>::Value)
	{
		static_assert(sizeof(ScalarType) <= 4, "Unsupported enum type");
		return GetTypeHash((uint32)Value);
	}
	else if constexpr (TIsPointer<ScalarType>::Value)
	{
		return PointerHash(Value);
	}
	else
	{
		static_assert(sizeof(ScalarType) == 0, "Unsupported scalar type");
	}
}
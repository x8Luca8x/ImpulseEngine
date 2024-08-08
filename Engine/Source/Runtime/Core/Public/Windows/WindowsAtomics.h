#pragma once

#include "Platform/PlatformAtomics.h"

#if PLATFORM_WINDOWS

/** Platform specific atomic data type */
typedef long FAtomic;
typedef long long FAtomic64;

class CORE_API FWindowsAtomics : public FGenericPlatformAtomics
{
public:

	/**
	* Increments the value of an atomic variable.
	* @param ValuePtr Pointer to the atomic variable to increment.
	* @return The resulting incremented value.
	*/
	static FAtomic InterlockedIncrement(FAtomic volatile* ValuePtr);

	/**
	* Decrements the value of an atomic variable.
	* @param ValuePtr Pointer to the atomic variable to decrement.
	* @return The resulting decremented value.
	*/
	static FAtomic InterlockedDecrement(FAtomic volatile* ValuePtr);

	/**
	* Sets a value to the atomic variable.
	* @param ValuePtr Pointer to the atomic variable to set.
	* @param Value The value to set.
	* @return The resulting value.
	*/
	static FAtomic InterlockedExchange(FAtomic volatile* ValuePtr, FAtomic Value);

	/**
	* Adds a value to the atomic variable.
	* @param ValuePtr Pointer to the atomic variable to add to.
	* @param Value The value to add.
	* @return The resulting value.
	*/
	static FAtomic InterlockedAdd(FAtomic volatile* ValuePtr, FAtomic Value);

	/**
	* Compares an atomic variable with a value for equality and, if they are equal, replaces the former with a third value.
	* @param ValuePtr Pointer to the atomic variable to compare.
	* @param Exchange The value to exchange with if the comparison is successful.
	* @param Comperand The value to compare to.
	* @return The resulting value.
	*/
	static FAtomic InterlockedCompareExchange(FAtomic volatile* ValuePtr, FAtomic Exchange, FAtomic Comperand);

	/**
	* Performs an atomic bitwise AND operation on an atomic variable and a value.
	* @param ValuePtr Pointer to the atomic variable to AND with.
	* @param Value The value to AND with.
	* @return The resulting value.
	*/
	static FAtomic InterlockedAnd(FAtomic volatile* ValuePtr, FAtomic Value);

	/**
	* Performs an atomic bitwise OR operation on an atomic variable and a value.
	* @param ValuePtr Pointer to the atomic variable to OR with.
	* @param Value The value to OR with.
	* @return The resulting value.
	*/
	static FAtomic InterlockedOr(FAtomic volatile* ValuePtr, FAtomic Value);

	/**
	* Performs an atomic bitwise XOR operation on an atomic variable and a value.
	* @param ValuePtr Pointer to the atomic variable to XOR with.
	* @param Value The value to XOR with.
	* @return The resulting value.
	*/
	static FAtomic InterlockedXor(FAtomic volatile* ValuePtr, FAtomic Value);

public:

	/**
	* Increments the value of an atomic variable.
	* @param ValuePtr Pointer to the atomic variable to increment.
	* @return The resulting incremented value.
	*/
	static FAtomic64 InterlockedIncrement64(FAtomic64 volatile* ValuePtr);

	/**
	* Decrements the value of an atomic variable.
	* @param ValuePtr Pointer to the atomic variable to decrement.
	* @return The resulting decremented value.
	*/
	static FAtomic64 InterlockedDecrement64(FAtomic64 volatile* ValuePtr);

	/**
	* Sets a value to the atomic variable.
	* @param ValuePtr Pointer to the atomic variable to set.
	* @param Value The value to set.
	* @return The resulting value.
	*/
	static FAtomic64 InterlockedExchange64(FAtomic64 volatile* ValuePtr, FAtomic64 Value);

	/**
	* Adds a value to the atomic variable.
	* @param ValuePtr Pointer to the atomic variable to add to.
	* @param Value The value to add.
	* @return The resulting value.
	*/
	static FAtomic64 InterlockedAdd64(FAtomic64 volatile* ValuePtr, FAtomic64 Value);

	/**
	* Compares an atomic variable with a value for equality and, if they are equal, replaces the former with a third value.
	* @param ValuePtr Pointer to the atomic variable to compare.
	* @param Exchange The value to exchange with if the comparison is successful.
	* @param Comperand The value to compare to.
	* @return The resulting value.
	*/
	static FAtomic64 InterlockedCompareExchange64(FAtomic64 volatile* ValuePtr, FAtomic64 Exchange, FAtomic64 Comperand);

	/**
	* Performs an atomic bitwise AND operation on an atomic variable and a value.
	* @param ValuePtr Pointer to the atomic variable to AND with.
	* @param Value The value to AND with.
	* @return The resulting value.
	*/
	static FAtomic64 InterlockedAnd64(FAtomic64 volatile* ValuePtr, FAtomic64 Value);

	/**
	* Performs an atomic bitwise OR operation on an atomic variable and a value.
	* @param ValuePtr Pointer to the atomic variable to OR with.
	* @param Value The value to OR with.
	* @return The resulting value.
	*/
	static FAtomic64 InterlockedOr64(FAtomic64 volatile* ValuePtr, FAtomic64 Value);

	/**
	* Performs an atomic bitwise XOR operation on an atomic variable and a value.
	* @param ValuePtr Pointer to the atomic variable to XOR with.
	* @param Value The value to XOR with.
	* @return The resulting value.
	*/
	static FAtomic64 InterlockedXor64(FAtomic64 volatile* ValuePtr, FAtomic64 Value);

};

typedef FWindowsAtomics FPlatformAtomics;

#endif
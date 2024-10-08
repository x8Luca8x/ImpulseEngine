#pragma once

#include "Definitions.h"

#include "EnableIf.h"
#include "IsPointer.h"
#include "TypeTraits.h"
#include "RemoveConst.h"
#include "IsDerivedFrom.h"
#include "AreTypesEqual.h"
#include "VardicElement.h"
#include "RemoveReference.h"
#include "TypeCompatibleBytes.h"

#include <type_traits>

// Type traits

template<typename T>
struct TIsScalar { enum { Value = std::is_scalar_v<T> }; };

template<typename T>
struct TIsArithmetic { enum { Value = std::is_arithmetic_v<T> }; };

template<typename T>
struct TIsFloatingPoint { enum { Value = std::is_floating_point_v<T> }; };

template<typename T>
struct TIsEnum { enum { Value = std::is_enum_v<T> }; };

template<typename T>
struct TIsSigned { enum { Value = std::is_signed_v<T> }; };

template<typename T>
struct TIsUnsigned { enum { Value = std::is_unsigned_v<T> }; };

template<typename T>
struct TIsConst { enum { Value = false }; };
template<typename T>
struct TIsConst<const T> { enum { Value = true }; };

// Templates

template<typename T>
struct TIslValueReference { enum { Value = std::is_lvalue_reference_v<T> }; };

template <typename T> struct TIdentity { typedef T Type; };

template <typename T>
FORCEINLINE T&& Forward(typename TRemoveReference<T>::Type& Obj)
{
	return (T&&)Obj;
}

template <typename T>
FORCEINLINE T&& Forward(typename TRemoveReference<T>::Type&& Obj)
{
	static_assert(!TIslValueReference<T>::Value, "bad forward call");
	return (T&&)Obj;
}

/**
 * MoveTemp will cast a reference to an rvalue reference.
 * This is UE's equivalent of std::move except that it will not compile when passed an rvalue or
 * const object, because we would prefer to be informed when MoveTemp will have no effect.
 */
template <typename T>
FORCEINLINE typename TRemoveReference<T>::Type&& MoveTemp(T&& Obj)
{
	typedef typename TRemoveReference<T>::Type CastType;

	// Validate that we're not being passed an rvalue or a const object - the former is redundant, the latter is almost certainly a mistake
	static_assert(TIslValueReference<T>::Value, "MoveTemp called on an rvalue");
	static_assert(!std::is_same_v<CastType&, const CastType&>, "MoveTemp called on a const object");

	return (CastType&&)Obj;
}

/**
 * MoveTempIfPossible will cast a reference to an rvalue reference.
 * This is UE's equivalent of std::move.  It doesn't static assert like MoveTemp, because it is useful in
 * templates or macros where it's not obvious what the argument is, but you want to take advantage of move semantics
 * where you can but not stop compilation.
 */
template <typename T>
FORCEINLINE typename TRemoveReference<T>::Type&& MoveTempIfPossible(T&& Obj)
{
	typedef typename TRemoveReference<T>::Type CastType;
	return (CastType&&)Obj;
}

template<typename T>
const typename TRemoveReference<T>::Type& CopyTemp(const T& Cp) { return static_cast<const typename TRemoveReference<T>::Type&>(Cp); }
template<typename T>
const typename TRemoveReference<T>::Type& CopyTemp(T& Cp) { return static_cast<const typename TRemoveReference<T>::Type&>(Cp); }

/**
 * A traits class which specifies whether a Swap of a given type should swap the bits or use a traditional value-based swap.
 */
template <typename T>
struct TUseBitwiseSwap
{
	// We don't use bitwise swapping for 'register' types because this will force them into memory and be slower.
	enum { Value = !(std::is_enum_v<T> || std::is_pointer_v<T> || std::is_arithmetic_v<T>) };
};


/**
 * Swap two values.  Assumes the types are trivially relocatable.
 */
template <typename T>
inline void Swap(T& A, T& B)
{
	if constexpr (TUseBitwiseSwap<T>::Value)
	{
		TTypeCompatibleBytes<T> Temp;
		*(TTypeCompatibleBytes<T>*) &Temp = *(TTypeCompatibleBytes<T>*) &A;
		*(TTypeCompatibleBytes<T>*) &A = *(TTypeCompatibleBytes<T>*) &B;
		*(TTypeCompatibleBytes<T>*) &B = *(TTypeCompatibleBytes<T>*) &Temp;
	}
	else
	{
		T Temp = MoveTemp(A);
		A = MoveTemp(B);
		B = MoveTemp(Temp);
	}
}

template <typename T>
inline void Exchange(T& A, T& B)
{
	Swap(A, B);
}

template<typename T>
struct TIsNumeric { enum { Value = TIsIntegral<T>::Value || TIsFloat<T>::Value }; };

#define IS_NUMERIC(T) TIsNumeric<T>::Value

template<typename T, typename... Args>
void ConstructItem(T* Ptr, Args&&... ConstructArgs)
{
	new (Ptr) T(Forward<Args>(ConstructArgs)...);
}

template<typename T>
void DestructItem(T* Ptr)
{
	Ptr->~T();
}

template<typename T, typename... Args>
void ConstructItems(T* Ptr, int32 Count, Args&&... ConstructArgs)
{
	for (int32 i = 0; i < Count; ++i)
		new (Ptr + i) T(Forward<Args>(ConstructArgs)...);
}

template<typename T>
void DestructItems(T* Ptr, int32 Count)
{
	for (int32 i = 0; i < Count; ++i)
		(Ptr + i)->~T();
}

/**
 * Uses implicit conversion to create an instance of a specific type.
 * Useful to make things clearer or circumvent unintended type deduction in templates.
 * Safer than C casts and static_casts, e.g. does not allow down-casts
 *
 * @param Obj  The object (usually pointer or reference) to convert.
 *
 * @return The object converted to the specified type.
 */
template <typename T>
FORCEINLINE T ImplicitConv(typename TIdentity<T>::Type Obj)
{
	return Obj;
}

/** Test if value can make a lossless static_cast roundtrip via OutType without a sign change */
template<typename OutType, typename InType>
constexpr bool IntFitsIn(InType In)
{
	static_assert(std::is_integral_v<InType> && std::is_integral_v<OutType>, "Only integers supported");

	OutType Out = static_cast<OutType>(In);
	bool bRoundtrips = In == static_cast<InType>(Out);

	// Signed <-> unsigned cast requires sign test, signed -> smaller signed is covered by roundtrip sign-extension.
	if constexpr ((static_cast<InType>(-1) < InType{}) != (static_cast<OutType>(-1) < OutType{}))
	{
		return bRoundtrips && (In < InType{} == Out < OutType{});
	}
	else
	{
		return bRoundtrips;
	}
}

/** Cast and check that value fits in OutType */
template<typename OutType, typename InType>
OutType IntCastChecked(InType In)
{
	if constexpr (std::is_signed_v<InType>)
	{
		checkf(IntFitsIn<OutType>(In), TEXT("Loss of data caused by narrowing conversion, In = %" INT64_FMT), (int64)In);
	}
	else
	{
		checkf(IntFitsIn<OutType>(In), TEXT("Loss of data caused by narrowing conversion, In = %" UINT64_FMT), (uint64)In);
	}
	return static_cast<OutType>(In);
}

/** Test if value can make a static_cast roundtrip via OutType whilst maintaining precision */
template<typename OutType, typename InType>
constexpr bool FloatFitsIn(InType In, InType Precision)
{
	static_assert(std::is_floating_point_v<InType> && std::is_floating_point_v<OutType>, "Only floating point supported");

	OutType Out = static_cast<OutType>(In);
	return fabs(static_cast<InType>(Out) - In) <= Precision;
}

template<typename OutType, typename InType>
OutType FloatCastChecked(InType In, InType Precision)
{
	checkf(FloatFitsIn<OutType>(In, Precision), TEXT("Loss of data caused by narrowing conversion"));
	return static_cast<OutType>(In);
}
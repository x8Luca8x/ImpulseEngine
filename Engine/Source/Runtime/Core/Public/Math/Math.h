#pragma once

#include "CoreModule.h"
#include "Templates/ImpulseTemplates.h"

#include <math.h>

class CORE_API FMath
{
public:

	static FORCEINLINE float Fmod(float X, float Y)
	{
		return fmodf(X, Y);
	}

	static FORCEINLINE double Fmod(double X, double Y)
	{
		return fmod(X, Y);
	}

	static FORCEINLINE float Floor(float X)
	{
		return floorf(X);
	}

	static FORCEINLINE double Floor(double X)
	{
		return floor(X);
	}

	static FORCEINLINE float Ceil(float X)
	{
		return ceilf(X);
	}

	static FORCEINLINE double Ceil(double X)
	{
		return ceil(X);
	}

	static FORCEINLINE float Round(float X)
	{
		return roundf(X);
	}

	static FORCEINLINE double Round(double X)
	{
		return round(X);
	}

	static FORCEINLINE int32 TruncToInt(float F)
	{
		return static_cast<int32>(F);
	}

	static FORCEINLINE int64 TruncToInt(double F)
	{
		return static_cast<int64>(F);
	}

	template<typename T>
	static FORCEINLINE T Pow(T Base, T Exponent)
	{
		return pow(Base, Exponent);
	}

	// @return The minimum of A and B
	template<typename T>
	FORCEINLINE static constexpr T Min(const T A, const T B)
	{
		static_assert(IS_NUMERIC(T), "Min is only defined for primitive types.");
		return (A <= B) ? A : B;
	}

	// @return The maximum of A and B
	template<typename T>
	FORCEINLINE static constexpr T Max(const T A, const T B)
	{
		static_assert(IS_NUMERIC(T), "Max is only defined for primitive types.");
		return (A >= B) ? A : B;
	}

	// @return The absolute value of A
	template<typename T>
	FORCEINLINE static constexpr T Abs(const T A)
	{
		static_assert(IS_NUMERIC(T), "Abs is only defined for primitive types.");
		static_assert(TIsSigned<T>::Value, "Abs is only defined for signed types.");

		return (A >= (T)0) ? A : -A;
	}

	// Clamps X to be between Min and Max, inclusive
	template<typename T>
	FORCEINLINE static constexpr T Clamp(const T InX, const T InMin, const T InMax)
	{
		static_assert(IS_NUMERIC(T), "Clamp is only defined for primitive types.");
		return Max<T>(InMin, Min<T>(InMax, InX));
	}
};
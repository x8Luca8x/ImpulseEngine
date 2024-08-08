#pragma once

template<typename T>
struct TIsEnumClass
{
	enum { Value = __is_enum(T) && !__is_convertible_to(T, int) };
};
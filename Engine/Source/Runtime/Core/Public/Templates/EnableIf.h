#pragma once

template<bool Predicate, typename T = void>
struct TEnableIf {};

template<typename T>
struct TEnableIf<true, T>
{
	typedef T type;
	typedef T Type;
};
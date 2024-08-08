#pragma once

template<bool bSelectA, typename A, typename B>
struct TSelect;

template<typename A, typename B>
struct TSelect<true, A, B>
{
	typedef A Result;
};

template<typename A, typename B>
struct TSelect<false, A, B>
{
	typedef B Result;
};
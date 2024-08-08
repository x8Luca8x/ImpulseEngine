#pragma once

#include "Templates/ImpulseTemplates.h"

template<size_t Index, typename... Types>
struct TVardicElement;

template<typename First, typename... InRest>
struct TVardicElement<0, First, InRest...>
{
	typedef First Type;
};
template<size_t Index, typename First, typename... Rest>
struct TVardicElement<Index, First, Rest...> : public TVardicElement<Index - 1, Rest...> {};

struct FVardicHelper
{
public:
	template<size_t Index, typename... Types>
	static const typename TVardicElement<Index, Types...>::Type* Get(const Types&... Arguments)
	{
		return GetInternal<Index, typename TVardicElement<Index, Types...>::Type>(Arguments...);
	}

	/*template<typename RetValType, typename... Types>
	static const RetValType* Get(size_t Index, const Types&... Arguments)
	{
		size_t currentIndex = 0;
		return GetInternal<RetValType, Types...>(Index, currentIndex,  Arguments...);
	}*/

private:
	FVardicHelper() = delete;

	template<size_t Index, typename RetValType, typename First, typename... Rest>
	static const RetValType* GetInternal(const First& FirstArgument, const Rest&... RestArguments)
	{
		if constexpr (Index == 0)
			return &FirstArgument;
		else
			return GetInternal<Index - 1, RetValType, Rest...>(RestArguments...);
	}

	/*template<typename RetValType, typename First, typename... Rest>
	static const RetValType* GetInternal(const size_t& Index, size_t& CurrentIndex, const First& FirstArgument, const Rest&... RestArgmuents)
	{
		if (CurrentIndex == Index)
			return (const RetValType*)&FirstArgument;
		else
		{
			if constexpr (sizeof...(Rest) == 0)
				return nullptr;

			++CurrentIndex;
			return GetInternal<RetValType, Rest...>(Index, CurrentIndex, RestArgmuents...);
		}
	}*/
};
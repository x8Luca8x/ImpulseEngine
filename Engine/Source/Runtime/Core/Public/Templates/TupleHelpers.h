#pragma once

#include "FunctionHelpers.h"

class FTupleHelper
{
public:

	template<int...>
	struct Sequence {};
	template<int N, int... S>
	struct SequenceGenerator : SequenceGenerator<N - 1, N - 1, S...> {};
	template<int... S>
	struct SequenceGenerator<0, S...> { typedef Sequence<S...> Type; };

private:

	template<typename FuncType, typename... ParamTypes>
	struct FCallHelper
	{
		FCallHelper(const TFunction<FuncType>& InFunction, const TTuple<ParamTypes...>& InTuple)
			: Tuple(InTuple), Function(InFunction) {}

		template<int... S>
		typename TFunctionRetValType<FuncType>::Type Call(Sequence<S...>)
		{
			return Function(FTupleHelper::Get<S>(Tuple)...);
		}

		template<int... S>
		static typename TFunctionRetValType<FuncType>::Type StaticCall(const TFunction<FuncType>& Function, const TTuple<ParamTypes...>& Tuple, Sequence<S...>)
		{
			return Function(FTupleHelper::Get<S>(Tuple)...);
		}

		const TTuple<ParamTypes...>& Tuple;
		const TFunction<FuncType>& Function;
	};

public:

	/*template<typename Type, typename... Types>
	static Type* Get(const TTuple<Types...>& Tuple, size_t Index)
	{
		size_t currentIndex = 0;
		return GetInternal<Type>(Tuple, Index, currentIndex);
	}

	template<size_t Index, typename... Types>
	static typename TTupleElement<Index, TTuple<Types...>>::Type Get(const TTuple<Types...>& Tuple)
	{
		return *Get<typename TTupleElement<Index, TTuple<Types...>>::Type>(Tuple, Index);
	}*/

	template<size_t Index, typename... Types>
	static constexpr typename TTupleElement<Index, TTuple<Types...>>::Type Get(const TTuple<Types...>& Tuple)
	{
		if constexpr (Index == 0)
			return Tuple.Value;
		else
			return Get<Index - 1>(Tuple.GetBase());
	}

	template<typename FuncType, typename... ParamTypes>
	static typename TFunctionRetValType<FuncType>::Type Call(TFunction<FuncType> Function, const TTuple<ParamTypes...>& Tuple)
	{
		/*FCallHelper<FuncType, ParamTypes...> helper(Function, Tuple);
		return helper.Call(typename SequenceGenerator<sizeof...(ParamTypes)>::Type{});*/

		return FCallHelper<FuncType, ParamTypes...>::StaticCall(Function, Tuple, typename SequenceGenerator<sizeof...(ParamTypes)>::Type{});
	}

protected:

	/*template<typename Type, typename... Types>
	static Type* GetInternal(const TTuple<Types...>& Tuple, const size_t& Index, size_t& CurrentIndex)
	{
		if (Tuple.IsEmpty())
			return nullptr;
		if (CurrentIndex == Index)
			return (Type*)&Tuple.Value;

		++CurrentIndex;
		return GetInternal<Type>(Tuple.GetBase(), Index, CurrentIndex);
	}*/
};
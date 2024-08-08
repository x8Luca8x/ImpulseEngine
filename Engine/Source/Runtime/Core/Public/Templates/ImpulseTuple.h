#pragma once

#include "Memory/Memory.h"
#include "ImpulseTemplates.h"
#include "Function.h"

template<typename... Types>
class TTuple;

template<>
class TTuple<>
{
public:
	using Base = TTuple<>;

	constexpr TTuple() {}
	constexpr TTuple(const TTuple&) {}
	constexpr TTuple(TTuple&& Tuple) noexcept {}

	TTuple& operator=(const TTuple&) { return *this; }
	TTuple& operator=(TTuple&&) noexcept { return *this; }

	template<typename First2, typename... Rest2>
	constexpr TTuple(First2 FirstElement, Rest2... _Rest) {}

	static consteval bool IsEmpty() { return true; }
	static consteval size_t Size() { return 0; }

	TTuple& GetBase() { return *this; }
	const TTuple& GetBase() const { return *this; }
};

template<typename First, typename... Rest>
class TTuple<First, Rest...> : private TTuple<Rest...>
{
public:
	using Base = TTuple<Rest...>;

	constexpr TTuple() = default;
	constexpr TTuple(const TTuple& Tuple)
		: Base(static_cast<const TTuple<Rest...>&>(Tuple.GetBase())), Value(Tuple.Value) {}
	constexpr TTuple(TTuple&& Tuple) noexcept
		: Base(FMemory::Move(Tuple.GetBase())), Value(FMemory::Move(Tuple.Value)) {}

	TTuple& operator=(const TTuple& Tuple)
	{
		Value = Tuple.Value;
		GetBase() = Tuple.GetBase();

		return *this;
	}
	TTuple& operator=(TTuple&& Tuple) noexcept
	{
		Value = FMemory::Move(Tuple.Value);
		GetBase() = FMemory::Move(Tuple.GetBase());

		return *this;
	}

	constexpr TTuple(First FirstElement, Rest... _Rest)
		: Base(_Rest...), Value(FirstElement) {}

	static consteval bool IsEmpty() { return false; }
	static consteval size_t Size() { return sizeof...(Rest) + 1; }

	Base& GetBase() { return *this; }
	const Base& GetBase() const { return *this; }

	First Value;
};

template<size_t Index, typename... Types>
struct TTupleElement;

template<typename First, typename... Rest>
struct TTupleElement<0, TTuple<First, Rest...>>
{
	typedef First Type;
};
template<size_t Index, typename First, typename... Rest>
struct TTupleElement<Index, TTuple<First, Rest...>> : public TTupleElement<Index - 1, TTuple<Rest...>> {};

template<typename... ParamTypes>
TTuple<ParamTypes...> MakeTuple(ParamTypes... Params)
{
	return TTuple<ParamTypes...>(Params...);
}
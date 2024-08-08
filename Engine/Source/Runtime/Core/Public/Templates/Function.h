#pragma once

template<typename Type>
struct TIsMemberFunctionPointer;

template<typename Type>
struct TIsMemberFunctionPointer { enum { bValue = false }; };

template<typename InRetValType, typename Class, typename... ParamTypes>
struct TIsMemberFunctionPointer<InRetValType(Class::*)(ParamTypes...)> { enum { bValue = true }; };

#define IS_MEMBER_FUNCTION_POINTER_RAW(Type) TIsMemberFunctionPointer<Type>::bValue
#define IS_MEMBER_FUNCTION_POINTER(Type) IS_MEMBER_FUNCTION_POINTER_RAW(decltype(Type))

// A tempalte class that can store any function pointer type named TFunction
template<typename FuncType>
class TFunction
{
public:
	// Default constructor
	TFunction() = default;

	// Constructor that takes a function pointer
	TFunction(FuncType Function)
		: Function(Function) {}

	TFunction(const TFunction& Function)
		: Function(Function.Function) {}
	TFunction(TFunction&& Function)
	{
		this->Function = Function.Function;
		Function.Function = nullptr;
	}

	TFunction& operator=(const TFunction& Function)
	{
		this->Function = Function.Function;
		return *this;
	}
	TFunction& operator=(TFunction&& Function)
	{
		this->Function = Function.Function;
		Function.Function = nullptr;
		return *this;
	}

	TFunction& operator=(FuncType Function)
	{
		this->Function = Function;
		return *this;
	}

	// Invokes the function pointer
	auto operator()() const
	{
		static_assert(!IS_MEMBER_FUNCTION_POINTER_RAW(FuncType), "TFunction: Cannot invoke member function pointer with no parameters!");
		return Function();
	}

	// Invokes the function pointer
	template<typename FirstType, typename... ParamTypes>
	auto operator()(FirstType First, ParamTypes... Params) const
	{
		if constexpr (IS_MEMBER_FUNCTION_POINTER_RAW(FuncType))
			return (First->*Function)(Params...);
		else
			return Function(First, Params...);
	}

	// Returns true if the function pointer is valid
	bool IsValid() const
	{
		return Function != nullptr;
	}
	
private:
	// The function pointer
	FuncType Function = nullptr;
};

template<typename FuncType>
TFunction<FuncType> MakeFunction(FuncType Function) { return TFunction<FuncType>(Function); }

template<typename FuncType>
consteval TFunction<FuncType> FunctionType(FuncType Function) { return TFunction<FuncType>(); }

template<typename FuncType>
consteval FuncType GetFunctor(FuncType Function) { return Function; }
template<typename FuncType>
consteval FuncType GetFunctor(TFunction<FuncType> Function) { return FuncType(); }

#define DECL_FUNC(FunctionPtr) decltype(FunctionType(FunctionPtr))
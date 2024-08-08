#pragma once

#include "Templates/ImpulseTemplates.h"

/**
* Helper to declare a member function pointer type from a given function signature.
*/
template<bool bConst, typename InRetValType, typename InBase, typename... InParamTypes>
struct TMemFunPtrType;

template<typename InRetValType, typename InBase, typename... InParamTypes>
struct TMemFunPtrType<false, InRetValType, InBase, InParamTypes...>
{
	typedef InRetValType(InBase::* Type)(InParamTypes...);
};

template<typename InRetValType, typename InBase, typename... InParamTypes>
struct TMemFunPtrType<true, InRetValType, InBase, InParamTypes...>
{
	typedef InRetValType(InBase::* Type)(InParamTypes...) const;
};

/**
* Helper to declare a static function pointer type from a given function signature.
*/
template<typename InRetValType, typename... InParamTypes>
struct TFuncPtrType
{
	typedef InRetValType(*Type)(InParamTypes...);
};

/**
* Helper to declare a lambda function pointer type from a given function signature.
*/
template<typename Signature>
struct TLambdaFuncPtrType
{
	typedef Signature Type;
};

/**
* Delegate instances are the objects bound to delegates.
* They are the objects that are actually executed when a delegate is called.
*/
namespace IE::Private::Delegate
{
	class IDelegateInstanceInterface
	{
	public:

		/**
		* Checks if the delegate instance is bound to a particular user object.
		* @param InUserObject - The user object to check for.
		* @return True if the delegate instance is bound to the user object, false otherwise.
		*/
		virtual bool HasSameObject(const void* InUserObject) const = 0;

		/**
		* Makes a copy of the delegate instance.
		* @return The copy of the delegate instance.
		*/
		virtual IDelegateInstanceInterface* Clone() const = 0;
	};

	template<typename InRetValType, typename... InParamTypes>
	class TDelegateInstanceBase : public IDelegateInstanceInterface
	{
	public:

		/**
		* Execute the delegate instance.
		* @param Params - The parameters to pass to the delegate instance.
		*/
		virtual InRetValType Execute(InParamTypes&&... Params) const = 0;
	};

	template<bool bConst, typename InRetValType, typename InUserClass, typename... InParamTypes>
	class TMemDelegateInstance : public TDelegateInstanceBase<InRetValType, InParamTypes...>
	{

		using FuncType = typename TMemFunPtrType<bConst, InRetValType, InUserClass, InParamTypes...>::Type;

	public:

		TMemDelegateInstance(InUserClass* InUserObject, FuncType InUserMethod)
			: UserObject(InUserObject)
			, UserMethod(InUserMethod) {}

		virtual InRetValType Execute(InParamTypes&&... Params) const override
		{
			return (UserObject->*UserMethod)(Forward<InParamTypes>(Params)...);
		}

		virtual bool HasSameObject(const void* InUserObject) const override
		{
			return UserObject == InUserObject;
		}

		virtual IDelegateInstanceInterface* Clone() const override
		{
			return new TMemDelegateInstance<bConst, InRetValType, InUserClass, InParamTypes...>(UserObject, UserMethod);
		}

	private:

		InUserClass* UserObject;
		FuncType UserMethod;
	};

	template<typename InRetValType, typename... InParamTypes>
	class TFuncDelegateInstance : public TDelegateInstanceBase<InRetValType, InParamTypes...>
	{

		using FuncType = typename TFuncPtrType<InRetValType, InParamTypes...>::Type;

	public:

		TFuncDelegateInstance(FuncType InFunc)
			: Func(InFunc) {}

		virtual InRetValType Execute(InParamTypes&&... Params) const override
		{
			return Func(Forward<InParamTypes>(Params)...);
		}

		virtual bool HasSameObject(const void* InUserObject) const override
		{
			return false;
		}

		virtual IDelegateInstanceInterface* Clone() const override
		{
			return new TFuncDelegateInstance<InRetValType, InParamTypes...>(Func);
		}

	private:

		FuncType Func;
	};

	template<typename LambdaSignature, typename InRetValType, typename... InParamTypes>
	class TLambdaDelegateInstance : public TDelegateInstanceBase<InRetValType, InParamTypes...>
	{

		using FuncType = typename TLambdaFuncPtrType<LambdaSignature>::Type;

	public:

		TLambdaDelegateInstance(FuncType InFunc)
			: Func(InFunc) {}

		virtual InRetValType Execute(InParamTypes&&... Params) const override
		{
			return Func(Forward<InParamTypes>(Params)...);
		}

		virtual bool HasSameObject(const void* InUserObject) const override
		{
			return false;
		}

		virtual IDelegateInstanceInterface* Clone() const override
		{
			return new TLambdaDelegateInstance<LambdaSignature, InRetValType, InParamTypes...>(Func);
		}

	private:

		mutable FuncType Func;
	};
}
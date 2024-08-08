#pragma once

#include "DelegateInstance.h"

#include "Debug/ImpulseDebug.h"

#include "Platform/PlatformAtomics.h"

#include "Misc/Hash.h"

class FDelegateHandle
{
public:

	enum EGenerateNewHandleType
	{
		GenerateNewHandle
	};

public:

	FDelegateHandle() = default;
	FDelegateHandle(const FDelegateHandle&) = default;
	FDelegateHandle(FDelegateHandle&&) = default;
	FDelegateHandle& operator=(const FDelegateHandle&) = default;
	FDelegateHandle& operator=(FDelegateHandle&&) = default;

	FDelegateHandle(EGenerateNewHandleType)
		: Handle(NextHandle()) {}

	FORCEINLINE void Reset() { Handle = 0; }
	FORCEINLINE bool IsValid() const { return Handle != 0; }

public:

	friend uint32 GetTypeHash(const FDelegateHandle& InHandle)
	{
		return GetTypeHash(InHandle.Handle);
	}

private:

	static FORCEINLINE FAtomic64 NextHandle()
	{
		static FAtomic64 handle{};
		return FPlatformAtomics::InterlockedIncrement64(&handle);
	}

	FAtomic64 Handle{};
};

namespace IE::Private::Delegate
{
	class IDelegateBase
	{
	public:

		virtual ~IDelegateBase()
		{
			FreeInstance();
		}

		/**
		* Gets the handle of the delegate.
		* @return The handle of the delegate.
		*/
		FORCEINLINE FDelegateHandle GetHandle() const
		{
			return Handle;
		}

		/**
		* Checks whether the delegate is bound.
		* @return True if the delegate is bound, false otherwise.
		*/
		FORCEINLINE bool IsBound() const
		{
			return InstancePtr != nullptr;
		}

		/**
		* Executes the delegate.
		*/
		template<typename InRetValType, typename... InParamTypes>
		FORCEINLINE InRetValType BaseExecution(InParamTypes... InParams)
		{
			checkf(InstancePtr, TEXT("Trying to execute an unbound delegate."));
			return static_cast<TDelegateInstanceBase<InRetValType, InParamTypes...>*>(InstancePtr)->Execute(Forward<InParamTypes>(InParams)...);
		}

	protected:

		IDelegateBase()
			: InstancePtr(nullptr), Handle() {}
		IDelegateBase(IDelegateInstanceInterface* InInstancePtr)
			: InstancePtr(InInstancePtr), Handle(FDelegateHandle::GenerateNewHandle) {}
		IDelegateBase(IDelegateInstanceInterface* InInstancePtr, FDelegateHandle InHandle)
			: InstancePtr(InInstancePtr), Handle(InHandle) {}

		IDelegateInstanceInterface* CloneInstance() const
		{
			return InstancePtr ? InstancePtr->Clone() : nullptr;
		}

		void FreeInstance()
		{
			if (InstancePtr)
				delete InstancePtr;

			InstancePtr = nullptr;
		}

	protected:

		FDelegateHandle Handle;
		IDelegateInstanceInterface* InstancePtr;
	};
}

template<typename InRetValType, typename... InParamTypes>
class TDelegate
{
	static_assert(sizeof(InRetValType) == 0, "Use TDelegate<InRetValType(InParamTypes...)> instead of TDelegate<InRetValType, InParamTypes...>.");
};

template<typename InRetValType, typename... InParamTypes>
class TDelegate<InRetValType(InParamTypes...)> : public IE::Private::Delegate::IDelegateBase
{

	using Base = IE::Private::Delegate::IDelegateBase;

public:

	TDelegate()
		: Base() {}

	TDelegate(const TDelegate& Other)
		: Base(Other.CloneInstance(), Other.Handle) {}

	TDelegate(TDelegate&& Other) noexcept
		: Base(Other.InstancePtr, Other.Handle)
	{
		Other.Handle.Reset();
		Other.InstancePtr = nullptr;
	}

	TDelegate& operator=(const TDelegate& Other)
	{
		if (this != &Other)
		{
			FreeInstance();

			Handle = Other.Handle;
			InstancePtr = Other.CloneInstance();
		}

		return *this;
	}

	TDelegate& operator=(TDelegate&& Other) noexcept
	{
		if (this != &Other)
		{
			FreeInstance();

			Handle = Other.Handle;
			Other.Handle.Reset();

			InstancePtr = Other.InstancePtr;
			Other.InstancePtr = nullptr;
		}

		return *this;

	}

	~TDelegate() = default;

protected:

	TDelegate(IE::Private::Delegate::IDelegateInstanceInterface* InInstancePtr)
		: Base(InInstancePtr) {}

public:

	/**
	* Executes the delegate.
	*/
	FORCEINLINE InRetValType Execute(InParamTypes... InParams)
	{
		return BaseExecution<InRetValType, InParamTypes...>(Forward<InParamTypes>(InParams)...);
	}

	/**
	* Executes the delegate if it is bound.
	*/
	FORCEINLINE InRetValType ExecuteIfBound(InParamTypes... InParams)
	{
		return IsBound() ? BaseExecution<InRetValType, InParamTypes...>(Forward<InParamTypes>(InParams)...) : InRetValType{};
	}

public:

	/**
	* Creates a delegate that is bound to a static function.
	* @param InFunc - The static function to bind to.
	* @return The created delegate.
	*/
	static TDelegate CreateStatic(typename TFuncPtrType<InRetValType, InParamTypes...>::Type InFunc)
	{
		return TDelegate(new IE::Private::Delegate::TFuncDelegateInstance<InRetValType, InParamTypes...>(InFunc));
	}

	/**
	* Creates a delegate that is bound to a member function.
	* @param InUserObject - The object to bind to.
	* @param InUserMethod - The member function to bind to.
	* @return The created delegate.
	*/
	template<typename InUserClass>
	static TDelegate Create(InUserClass* InUserObject, typename TMemFunPtrType<false, InRetValType, InUserClass, InParamTypes...>::Type InUserMethod)
	{
		return TDelegate(new IE::Private::Delegate::TMemDelegateInstance<false, InRetValType, InUserClass, InParamTypes...>(InUserObject, InUserMethod));
	}

	/**
	* Creates a delegate that is bound to a const member function.
	* @param InUserObject - The object to bind to.
	* @param InUserMethod - The const member function to bind to.
	* @return The created delegate.
	*/
	template<typename InUserClass>
	static TDelegate CreateConst(const InUserClass* InUserObject, typename TMemFunPtrType<true, InRetValType, const InUserClass, InParamTypes...>::Type InUserMethod)
	{
		return TDelegate(new IE::Private::Delegate::TMemDelegateInstance<true, InRetValType, const InUserClass, InParamTypes...>(InUserObject, InUserMethod));
	}

	/**
	* Creates a delegate that is bound to a lambda function.
	* @param InLambda - The lambda function to bind to.
	* @return The created delegate.
	*/
	template<typename InLambdaSignature>
	static TDelegate CreateLambda(InLambdaSignature&& InLambda)
	{
		return TDelegate(new IE::Private::Delegate::TLambdaDelegateInstance<InLambdaSignature, InRetValType, InParamTypes...>(Forward<InLambdaSignature>(InLambda)));
	}
};
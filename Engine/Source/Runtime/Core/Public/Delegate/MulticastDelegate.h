#pragma once

#include "Delegate.h"

#include "Containers/Map.h"

#include "Platform/PlatformAtomics.h"

template<typename... ParamTypes>
class TMulticastDelegate
{
public:

	using FDelegate = TDelegate<void(ParamTypes...)>;

public:

	/**
	* Adds a new delegate to the multicast delegate.
	 *
	 * @param Delegate The delegate to add.
	 * @return A handle to the delegate.
	*/
	FDelegateHandle Add(const FDelegate& Delegate)
	{
		Delegates.Add(Delegate.GetHandle(), Delegate);
		return Delegate.GetHandle();
	}

	/**
	* Removes a delegate from the multicast delegate.
	 *
	 * @param Handle The handle to the delegate to remove.
	*/
	void Remove(FDelegateHandle Handle)
	{
		Delegates.Remove(Handle);
	}

	/**
	* Removes all delegates from the multicast delegate.
	*/
	void Clear()
	{
		Delegates.Clear();
	}

	/**
	* Invokes all delegates in the multicast delegate.
	 *
	 * @param Params The parameters to pass to the delegates.
	*/
	void Broadcast(ParamTypes... Params)
	{
		for (auto& Delegate : Delegates)
		{
			Delegate.GetValue().Execute(Forward<ParamTypes>(Params)...);
		}
	}

	/**
	* Checks whether the multicast delegate is empty.
	 *
	 * @return true if the multicast delegate is empty, false otherwise.
	*/
	bool IsEmpty() const
	{
		return Delegates.Num() == 0;
	}

	/**
	* Checks whether the multicast delegate is bound to a single delegate.
	 *
	 * @return true if the multicast delegate is bound to a single delegate, false otherwise.
	*/
	bool IsBound() const
	{
		return Delegates.Num() == 1;
	}

	/**
	* Checks whether the multicast delegate is bound to more than one delegate.
	 *
	 * @return true if the multicast delegate is bound to more than one delegate, false otherwise.
	*/
	bool IsBoundToMany() const
	{
		return Delegates.Num() > 1;
	}

	/**
	* Gets the number of delegates in the multicast delegate.
	 *
	 * @return The number of delegates in the multicast delegate.
	*/
	int32 GetNumDelegates() const
	{
		return Delegates.Num();
	}

private:

	TMap<FDelegateHandle, FDelegate> Delegates;
};
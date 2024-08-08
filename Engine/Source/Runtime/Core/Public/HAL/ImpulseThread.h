#pragma once

#include "Platform/PlatformThread.h"

#include "Templates/ImpulseTuple.h"
#include "Templates/TupleHelpers.h"

/**
* Base class of the template class Thread.
*/
class CORE_API FThreadBase
{
public:

	void Join();
	void Detach();

	void Terminate();

	FThreadId GetId() const { return Thread.ThreadId; }
	FThreadHandle GetHandle() const { return Thread.ThreadHandle; }

protected:

	FThread Thread;
};

/**
* Wrapper class for a thread.
*/
template<typename Functor, typename... VarTypes>
class TThread : public FThreadBase
{
public:

	TThread(TFunction<Functor> InFunctor, VarTypes&&... InVars)
		: Functor(InFunctor)
		, Tuple(MakeTuple<VarTypes...>(InVars...))
	{
		Init();
	}

private:

	void Init()
	{
		Thread = FPlatformThread::CreateThread(&ThreadFun, this);
	}

	static void ThreadFun(void* Data)
	{
		TThread* Thread = (TThread*)Data;
		FTupleHelper::Call(Thread->Functor, Thread->Tuple);
	}

	TFunction<Functor> Functor;
	TTuple<VarTypes...> Tuple;
};

template<typename Functor, typename... VarTypes>
TThread<Functor, VarTypes...> CreateThread(TFunction<Functor> InFunction, VarTypes&&... InVars)
{
	return TThread<Functor, VarTypes...>(InFunction, Forward<VarTypes>(InVars)...);
}
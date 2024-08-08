#pragma once

#include "Misc/RefCounter.h"

#include "Select.h"

template<typename ObjectType, ESPMode Mode>
class TSharedPtr;
template<typename ObjectType, ESPMode Mode>
class TWeakPtr;
template<typename ObjectType, ESPMode Mode>
class TSharedFromThis;

namespace SharedPointer
{
	template<typename ObjectType, ESPMode Mode, typename = Tor>
	constexpr void EnableSharedFromThis(TSharedPtr<ObjectType, Mode>* InSharedPtr, ObjectType* InObject, TSharedFromThis<ObjectType, Mode>* InShareable)
	{
		if (InShareable)
			InShareable->UpdateWeakReference(InSharedPtr);
	}
}

template<typename ObjectType, ESPMode Mode = PLATFORM_DEFAULT_SMART_POINTER_CLASS>
class TSharedPtr final
{
	friend class TWeakPtr<ObjectType, Mode>;

	using RefCounterType = typename TRefCountTraits<Mode>::Type;
	using WeakPtr = TWeakPtr<ObjectType, Mode>;

public:

	TSharedPtr() = default;

	template<typename OtherType, typename = std::enable_if_t<std::is_convertible<OtherType*, ObjectType*>::value>>
	TSharedPtr(OtherType* InObject)
		: Object(InObject)
	{
		AddRef();
		SharedPointer::EnableSharedFromThis(this, InObject, InObject);
	}

	TSharedPtr(const TSharedPtr& Other)
		: Object(Other.Object), RefCounter(Other.RefCounter)
	{
		AddRef();
		SharedPointer::EnableSharedFromThis(this, Object, Object);
	}

	TSharedPtr(TSharedPtr&& Other) noexcept
		: Object(Other.Object), RefCounter(Other.RefCounter)
	{
		Other.Object = nullptr;
		Other.RefCounter = nullptr;

		SharedPointer::EnableSharedFromThis(this, Object, Object);
	}

	template<typename OtherType, typename = std::enable_if_t<std::is_convertible<OtherType*, ObjectType*>::value>>
	TSharedPtr(const TSharedPtr<OtherType, Mode>& Other)
		: Object(Other.Get()), RefCounter(Other.GetRefCounter())
	{
		AddRef();
		SharedPointer::EnableSharedFromThis(this, Object, Object);
	}

	TSharedPtr(const WeakPtr& InWeakPtr)
		: Object(InWeakPtr.Object), RefCounter(InWeakPtr.RefCounter)
	{
		AddRef();
		SharedPointer::EnableSharedFromThis(this, Object, Object);
	}

	TSharedPtr(TYPE_OF_NULLPTR)
		: Object(nullptr) {}

	TSharedPtr& operator=(const TSharedPtr& Other)
	{
		if (this != &Other)
		{
			Release();

			Object = Other.Object;
			RefCounter = Other.GetRefCounter();

			AddRef();
		}

		return *this;
	}

	TSharedPtr& operator=(TSharedPtr&& Other) noexcept
	{
		if (this != &Other)
		{
			Release();

			Object = Other.Object;
			RefCounter = Other.GetRefCounter();

			Other.Object = nullptr;
			Other.RefCounter = nullptr;
		}

		return *this;
	}

	template<typename OtherType>
	TSharedPtr& operator=(const TSharedPtr<OtherType, Mode>& Other)
	{
		static_assert(std::is_convertible<OtherType*, ObjectType*>::value, "Incompatible types");
		if (this != &Other)
		{
			Release();

			Object = Other.Get();
			RefCounter = Other.GetRefCounter();

			AddRef();
		}

		return *this;
	}

	TSharedPtr& operator=(const WeakPtr& InWeakPtr)
	{
		if (this != &InWeakPtr)
		{
			Release();

			Object = InWeakPtr.Object;
			RefCounter = InWeakPtr.RefCounter;

			AddRef();
		}

		return *this;
	}

	TSharedPtr& operator=(TYPE_OF_NULLPTR)
	{
		Release();
		Object = nullptr;
		RefCounter = nullptr;

		return *this;
	}

	ObjectType* operator->() const
	{
		return Object;
	}

	ObjectType& operator*() const
	{
		return *Object;
	}

	operator bool() const
	{
		return Object != nullptr;
	}

	bool operator==(const TSharedPtr& Other) const
	{
		return Object == Other.Object;
	}

	bool operator!=(const TSharedPtr& Other) const
	{
		return Object != Other.Object;
	}

	bool operator==(const ObjectType* InObject) const
	{
		return Object == InObject;
	}

	bool operator!=(const ObjectType* InObject) const
	{
		return Object != InObject;
	}

	~TSharedPtr()
	{
		Release();
	}

public:

	/**
	* Get the object this smart pointer is referencing.
	* @return The object this smart pointer is referencing.
	*/
	ObjectType* Get() { return Object; }

	/**
	* Get the object this smart pointer is referencing.
	* @return The object this smart pointer is referencing.
	*/
	const ObjectType* Get() const { return Object; }

	/**
	* Get the reference counter for this smart pointer.
	* This should only be used for debugging purposes.
	* @return The reference counter for this smart pointer.
	*/
	IRefCounterBase* GetRefCounter() const { return RefCounter; }

protected:

	void AddRef()
	{
		if constexpr (Mode == ESPMode::Fast)
			return;

		if (RefCounter)
			RefCounter->AddRef();
		else
		{
			if (Object)
			{
				RefCounter = new RefCounterType();
				RefCounter->AddRef();
			}
		}
	}

	void Release()
	{
		if constexpr (Mode == ESPMode::Fast)
			return;

		if (RefCounter)
		{
			RefCounter->Release();
			if (RefCounter->GetRefCount() == 0)
			{
				delete Object;
				if (RefCounter->GetTotalRefCount() == 0)
				{
					delete RefCounter;
					RefCounter = nullptr;
				}
			}
		}
	}

protected:

	ObjectType* Object = nullptr;
	IRefCounterBase* RefCounter = nullptr;
};

/**
* Impulse engine equivalent of std::weak_ptr.
*/
template<typename ObjectType, ESPMode Mode = PLATFORM_DEFAULT_SMART_POINTER_CLASS>
class TWeakPtr
{

	friend class TSharedPtr<ObjectType, Mode>;

	using RefCounterType = typename TRefCountTraits<Mode>::Type;

public:

	TWeakPtr() = default;

	TWeakPtr(const TWeakPtr& InWeakPtr)
		: Object(InWeakPtr.Object), RefCounter(InWeakPtr.RefCounter)
	{
		if (RefCounter)
			RefCounter->AddWeakRef();
	}

	TWeakPtr(TWeakPtr&& InWeakPtr) noexcept
		: Object(InWeakPtr.Object), RefCounter(InWeakPtr.RefCounter)
	{
		InWeakPtr.Object = nullptr;
		InWeakPtr.RefCounter = nullptr;
	}

	TWeakPtr(const TSharedPtr<ObjectType, Mode>& InSharedPtr)
		: Object(InSharedPtr.Object), RefCounter(InSharedPtr.RefCounter)
	{
		if (RefCounter)
			RefCounter->AddWeakRef();
	}

	~TWeakPtr()
	{
		if (RefCounter)
			RefCounter->ReleaseWeakRef();
	}

	TWeakPtr& operator=(const TWeakPtr& InWeakPtr)
	{
		if (this != &InWeakPtr)
		{
			Release();

			Object = InWeakPtr.Object;
			RefCounter = InWeakPtr.RefCounter;

			if (RefCounter)
				RefCounter->AddWeakRef();
		}

		return *this;
	}

	TWeakPtr& operator=(TWeakPtr&& InWeakPtr) noexcept
	{
		if (this != &InWeakPtr)
		{
			Release();

			Object = InWeakPtr.Object;
			RefCounter = InWeakPtr.RefCounter;

			InWeakPtr.Object = nullptr;
			InWeakPtr.RefCounter = nullptr;
		}

		return *this;
	}

	TWeakPtr& operator=(const TSharedPtr<ObjectType, Mode>& InSharedPtr)
	{
		Release();

		Object = InSharedPtr.Object;
		RefCounter = InSharedPtr.RefCounter;

		if (RefCounter)
			RefCounter->AddWeakRef();

		return *this;
	}

public:

	TSharedPtr<ObjectType, Mode> Pin() const
	{
		return TSharedPtr<ObjectType, Mode>(*this);
	}

public:

	inline bool IsValid() const { return RefCounter && RefCounter->GetRefCount() > 0; }

public:

	inline operator bool() const { return IsValid(); }

private:

	/**
	* Initializes the ref counter.
	*/

	/**
	* Relases the interal resources.
	*/
	void Release()
	{
		Object = nullptr;
		if (RefCounter)
		{
			RefCounter->ReleaseWeakRef();
			if (RefCounter->GetTotalRefCount() == 0)
			{
				delete RefCounter;
				RefCounter = nullptr;
			}
		}
	}

private:

	ObjectType* Object = nullptr;
	IRefCounterBase* RefCounter = nullptr;
};

/**
* Shared from this is useful to get an existing shared pointer from a raw pointer.
* NOTE: AsShared can only be called if the object is already managed by a shared pointer!
*/
template<typename ObjectType, ESPMode Mode = PLATFORM_DEFAULT_SMART_POINTER_CLASS>
class TSharedFromThis
{
public:

	inline TSharedPtr<ObjectType, Mode> AsShared()
	{
		TSharedPtr<ObjectType, Mode> result = WeakThis.Pin();
		// check(result.Get() == this);

		return result;
	}

	inline TSharedPtr<const ObjectType, Mode> AsShared() const
	{
		TSharedPtr<const ObjectType, Mode> result = WeakThis.Pin();
		// check(result.Get() == this);

		return result;
	}

	inline TWeakPtr<ObjectType, Mode> AsWeak() const
	{
		check(WeakThis.IsValid());
		return WeakThis;
	}

public:

	/**
	* For internal use only!
	*/
	void UpdateWeakReference(TSharedPtr<ObjectType, Mode>* InSharedPtr) const
	{
		if (!WeakThis.IsValid())
			WeakThis = *InSharedPtr;
	}

private:

	mutable TWeakPtr<ObjectType, Mode> WeakThis;
};

template<typename ObjectType, ESPMode Mode = PLATFORM_DEFAULT_SMART_POINTER_CLASS>
static inline TSharedPtr<ObjectType, Mode> MakeShared()
{
	return TSharedPtr<ObjectType, Mode>(new ObjectType());
}

template<typename ObjectType, ESPMode Mode = PLATFORM_DEFAULT_SMART_POINTER_CLASS>
static inline TSharedPtr<ObjectType, Mode> MakeShared(ObjectType* InOther)
{
	return TSharedPtr<ObjectType, Mode>(InOther);
}

template<typename ObjectType, ESPMode Mode = PLATFORM_DEFAULT_SMART_POINTER_CLASS>
static inline TSharedPtr<ObjectType, Mode> MakeShared(const TWeakPtr<ObjectType, Mode>& InWeakPtr)
{
	return TSharedPtr<ObjectType, Mode>(InWeakPtr);
}

template<typename To, typename From, ESPMode Mode>
TSharedPtr<To> StaticCastSharedPtr(const TSharedPtr<From, Mode>& InSharedPtr)
{
	return TSharedPtr<To, Mode>(static_cast<To*>(InSharedPtr.Get()));
}



template<typename ObjectType, ESPMode Mode = PLATFORM_DEFAULT_SMART_POINTER_CLASS>
static inline TWeakPtr<ObjectType, Mode> MakeWeak(const TSharedPtr<ObjectType, Mode>& InSharedPtr)
{
	return TWeakPtr<ObjectType, Mode>(InSharedPtr);
}
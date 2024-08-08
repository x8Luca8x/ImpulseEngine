#pragma once

#include "Definitions.h"

#include "Containers/ImpulseString.h"

namespace EVariantType
{
	enum Type
	{
		Unknown,
		Bool,
		Int8,
		Int16,
		Int32,
		Int64,
		UInt8,
		UInt16,
		UInt32,
		UInt64,
		Float,
		Double,
		String,
	};
}

class CORE_API IVariantData
{
public:

	virtual void Serialize(FArchive& Ar) = 0;
	virtual IVariantData* MakeCopy() = 0;
};

/**
* Variant for data that is stored on the stack.
*/
template<typename T>
class CORE_API FStackVariantData : public IVariantData
{
public:

	FStackVariantData(const T& InData)
	{
		FMemory::Memcpy(&Data, &InData, sizeof(T));
	}

	virtual void Serialize(FArchive& Ar) override
	{
		Ar.Serialize(&Data, sizeof(T));
	}

	virtual IVariantData* MakeCopy() override
	{
		return new FStackVariantData<T>(*((T*)Data.Bytes));
	}

	TTypeCompatibleBytesPtr<T> Data;
};

/**
* Variant data for strings.
*/
class CORE_API FStringVariantData : public IVariantData
{
public:

	FStringVariantData(const FString& InData)
		: Data(InData)
	{
	}

	virtual void Serialize(FArchive& Ar) override
	{
		Ar << Data;
	}

	virtual IVariantData* MakeCopy() override
	{
		return new FStringVariantData(Data);
	}

	FString Data;
};

template<typename T>
struct TVariantType
{
	static_assert(sizeof(T) == 0, "Unsupported type");
};

template<>
struct TVariantType<bool>
{
	static constexpr EVariantType::Type Type = EVariantType::Bool;
	typedef FStackVariantData<bool> TData;
};

template<>
struct TVariantType<int8>
{
	static constexpr EVariantType::Type Type = EVariantType::Int8;
	typedef FStackVariantData<int8> TData;
};

template<>
struct TVariantType<int16>
{
	static constexpr EVariantType::Type Type = EVariantType::Int16;
	typedef FStackVariantData<int16> TData;
};

template<>
struct TVariantType<int32>
{
	static constexpr EVariantType::Type Type = EVariantType::Int32;
	typedef FStackVariantData<int32> TData;
};

template<>
struct TVariantType<int64>
{
	static constexpr EVariantType::Type Type = EVariantType::Int64;
	typedef FStackVariantData<int64> TData;
};

template<>
struct TVariantType<uint8>
{
	static constexpr EVariantType::Type Type = EVariantType::UInt8;
	typedef FStackVariantData<uint8> TData;
};

template<>
struct TVariantType<uint16>
{
	static constexpr EVariantType::Type Type = EVariantType::UInt16;
	typedef FStackVariantData<uint16> TData;
};

template<>
struct TVariantType<uint32>
{
	static constexpr EVariantType::Type Type = EVariantType::UInt32;
	typedef FStackVariantData<uint32> TData;
};

template<>
struct TVariantType<uint64>
{
	static constexpr EVariantType::Type Type = EVariantType::UInt64;
	typedef FStackVariantData<uint64> TData;
};

template<>
struct TVariantType<float>
{
	static constexpr EVariantType::Type Type = EVariantType::Float;
	typedef FStackVariantData<float> TData;
};

template<>
struct TVariantType<double>
{
	static constexpr EVariantType::Type Type = EVariantType::Double;
	typedef FStackVariantData<double> TData;
};

template<>
struct TVariantType<FString>
{
	static constexpr EVariantType::Type Type = EVariantType::String;
	typedef FStringVariantData TData;
};

/**
* Variant that can hold any type of data.
*/
class CORE_API FVariant
{
public:

	FVariant() = default;
	FVariant(const FVariant& Other);
	FVariant(FVariant&& Other) noexcept;

	template<typename T>
	FVariant(const T& InData);

	~FVariant();

	FVariant& operator=(const FVariant& Other);
	FVariant& operator=(FVariant&& Other) noexcept;

	template<typename T>
	FVariant& operator=(const T& InData);

	template<typename T>
	T& GetValue();
	template<typename T>
	const T& GetValue() const;

	FORCEINLINE EVariantType::Type GetType() const { return Type; }

private:

	void FreeData();

	IVariantData* Data = nullptr;
	EVariantType::Type Type = EVariantType::Unknown;
};

template<typename T>
inline FVariant::FVariant(const T& InData)
{
	typedef typename TVariantType<T>::TData TData;

	Data = new TData(InData);
	Type = TVariantType<T>::Type;
}

template<typename T>
inline FVariant& FVariant::operator=(const T& InData)
{
	typedef typename TVariantType<T>::TData TData;

	FreeData();

	Data = new TData(InData);
	Type = TVariantType<T>::Type;

	return *this;
}

template<typename T>
inline T& FVariant::GetValue()
{
	typedef typename TVariantType<T>::TData TData;

	checkf(Type == TVariantType<T>::Type, TEXT("Invalid type"));
	return static_cast<TData*>(Data)->Data;
}

template<typename T>
inline const T& FVariant::GetValue() const
{
	typedef typename TVariantType<T>::TData TData;

	checkf(Type == TVariantType<T>::Type, TEXT("Invalid type"));
	return static_cast<TData*>(Data)->Data;
}
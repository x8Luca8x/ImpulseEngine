#pragma once

#include "Containers/Map.h"
#include "Containers/Array.h"
#include "Containers/Variant.h"
#include "Containers/ImpulseString.h"

class FConfigValue;

//enum class EConfigValueType : uint8
//{
//	Bool,
//	Int,
//	Float,
//	String,
//	Array,
//	Object
//};
//
//template<typename T>
//struct TConfigValueTraits
//{
//	static_assert(sizeof(T) == 0, "Unsupported type for config value.");
//	static_assert(ARE_TYPES_EQUAL(T, uint8), "Unsigned integers are not supported. Use signed integers instead.");
//};
//
//template<>
//struct TConfigValueTraits<bool>
//{
//	static constexpr EConfigValueType Type = EConfigValueType::Bool;
//};
//
//template<>
//struct TConfigValueTraits<int8>
//{
//	static constexpr EConfigValueType Type = EConfigValueType::Int;
//};
//template<>
//struct TConfigValueTraits<int16>
//{
//	static constexpr EConfigValueType Type = EConfigValueType::Int;
//};
//template<>
//struct TConfigValueTraits<int32>
//{
//	static constexpr EConfigValueType Type = EConfigValueType::Int;
//};
//template<>
//struct TConfigValueTraits<int64>
//{
//	static constexpr EConfigValueType Type = EConfigValueType::Int;
//};
//
//template<>
//struct TConfigValueTraits<float>
//{
//	static constexpr EConfigValueType Type = EConfigValueType::Float;
//};
//
//template<>
//struct TConfigValueTraits<double>
//{
//	static constexpr EConfigValueType Type = EConfigValueType::Float;
//};
//
//template<>
//struct TConfigValueTraits<FString>
//{
//	static constexpr EConfigValueType Type = EConfigValueType::String;
//};
//
//template<>
//struct TConfigValueTraits<TArray<FConfigValue*>>
//{
//	static constexpr EConfigValueType Type = EConfigValueType::Array;
//};
//
//template<>
//struct TConfigValueTraits<TMap<FString, FConfigValue*>>
//{
//	static constexpr EConfigValueType Type = EConfigValueType::Object;
//};
//
///**
//* Class that holds a specific value type.
//*/
//class CORE_API FConfigValue
//{
//public:
//
//	/**
//	* Gets the value as config formatted string.
//	* @return The value as string.
//	*/
//	virtual FString ToString() const = 0;
//
//	/**
//	* Gets the config value type.
//	* @return The config value type.
//	*/
//	virtual EConfigValueType GetType() const = 0;
//
//	/**
//	* Checks if this value is of a specific type.
//	* @param InType The type to check.
//	* @return True if the value is of the specified type.
//	*/
//	bool IsOfType(EConfigValueType InType) const;
//
//	/**
//	* Sets the value.
//	* @param InValue The value to set.
//	*/
//	template<typename T>
//	void SetValue(const T& InValue)
//	{
//		constexpr EConfigValueType Type = TConfigValueTraits<T>::Type;
//		checkf(IsOfType(Type), TEXT("Invalid type for value."));
//
//		LowLevelSetValue(&InValue, sizeof(T));
//	}
//
//protected:
//
//	/**
//	* Sets the value.
//	* @param InValue - The value to set.
//	* @param InSize - The size of the value.
//	*/
//	virtual void LowLevelSetValue(const void* InValue, uint64 InSize) = 0;
//};
//
///**
//* Boolean config value.
//*/
//class CORE_API FConfigBoolValue : public FConfigValue
//{
//public:
//
//	//~ Begin FConfigValue Interface
//
//	FString ToString() const override;
//	EConfigValueType GetType() const override { return EConfigValueType::Bool; }
//
//	//~ End FConfigValue Interface
//
//protected:
//
//	//~ Begin FConfigValue Interface
//
//	void LowLevelSetValue(const void* InValue, uint64) override;
//
//	//~ End FConfigValue Interface
//
//private:
//
//	bool bValue = false;
//};
//
///**
//* Integer config value.
//*/
//class CORE_API FConfigIntValue : public FConfigValue
//{
//public:
//
//	//~ Begin FConfigValue Interface
//
//	FString ToString() const override;
//	EConfigValueType GetType() const override { return EConfigValueType::Int; }
//
//	//~ End FConfigValue Interface
//
//protected:
//
//	//~ Begin FConfigValue Interface
//
//	void LowLevelSetValue(const void* InValue, uint64 InSize) override;
//
//	//~ End FConfigValue Interface
//
//private:
//
//	int64 Value = 0;
//};
//
///**
//* Float config value.
//*/
//class CORE_API FConfigFloatValue : public FConfigValue
//{
//public:
//
//	//~ Begin FConfigValue Interface
//
//	FString ToString() const override;
//	EConfigValueType GetType() const override { return EConfigValueType::Float; }
//
//	//~ End FConfigValue Interface
//
//protected:
//
//	//~ Begin FConfigValue Interface
//
//	void LowLevelSetValue(const void* InValue, uint64 InSize) override;
//
//	//~ End FConfigValue Interface
//
//private:
//
//	double Value = 0.0;
//};
//
///**
//* Array config value.
//*/
//class CORE_API FConfigArrayValue : public FConfigValue
//{
//public:
//
//	//~ Begin FConfigValue Interface
//
//	FString ToString() const override;
//	EConfigValueType GetType() const override { return EConfigValueType::Array; }
//
//	//~ End FConfigValue Interface
//
//protected:
//
//	//~ Begin FConfigValue Interface
//
//	void LowLevelSetValue(const void* InValue, uint64 InSize) override;
//
//	//~ End FConfigValue Interface
//
//public:
//
//	/**
//	* Gets the array mutable.
//	* @return The array.
//	*/
//	inline TArray<FConfigValue*>& GetArray() { return Value; }
//
//	/**
//	* Gets the array.
//	* @return The array.
//	*/
//	inline const TArray<FConfigValue*>& GetArray() const { return Value; }
//
//private:
//
//	TArray<FConfigValue*> Value;
//};
//
///**
//* Object config value.
//*/
//class CORE_API FConfigObjectValue : public FConfigValue
//{
//public:
//
//	//~ Begin FConfigValue Interface
//
//	FString ToString() const override;
//	EConfigValueType GetType() const override { return EConfigValueType::Object; }
//
//	//~ End FConfigValue Interface
//
//protected:
//
//	//~ Begin FConfigValue Interface
//
//	void LowLevelSetValue(const void* InValue, uint64 InSize) override;
//
//	//~ End FConfigValue Interface
//
//public:
//
//	/**
//	* Gets the object mutable.
//	* @return The object.
//	*/
//	inline TMap<FString, FConfigValue*>& GetObject() { return Value; }
//
//	/**
//	* Gets the object.
//	* @return The object.
//	*/
//	inline const TMap<FString, FConfigValue*>& GetObject() const { return Value; }
//
//
//private:
//
//	TMap<FString, FConfigValue*> Value;
//
//};
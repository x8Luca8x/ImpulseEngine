#include "Config/ConfigValues.h"

//bool FConfigValue::IsOfType(EConfigValueType InType) const
//{
//	return GetType() == InType;
//}
//
//FString FConfigBoolValue::ToString() const
//{
//	return bValue ? TEXT("True") : TEXT("False");
//}
//
//void FConfigBoolValue::LowLevelSetValue(const void* InValue, uint64)
//{
//	bValue = *reinterpret_cast<const bool*>(InValue);
//}
//
//FString FConfigIntValue::ToString() const
//{
//	return FString::FromInt64(Value);
//}
//
//void FConfigIntValue::LowLevelSetValue(const void* InValue, uint64 InSize)
//{
//	if (InSize == sizeof(int8))
//		Value = *reinterpret_cast<const int8*>(InValue);
//	else if (InSize == sizeof(int16))
//		Value = *reinterpret_cast<const int16*>(InValue);
//	else if (InSize == sizeof(int32))
//		Value = *reinterpret_cast<const int32*>(InValue);
//	else if (InSize == sizeof(int64))
//		Value = *reinterpret_cast<const int64*>(InValue);
//	else
//		checkf(false, TEXT("Invalid size for int value: %d"), InSize);
//}
//
//FString FConfigFloatValue::ToString() const
//{
//	return FString::FromDouble(Value);
//}
//
//void FConfigFloatValue::LowLevelSetValue(const void* InValue, uint64 InSize)
//{
//	if (InSize == sizeof(float))
//		Value = *reinterpret_cast<const float*>(InValue);
//	else if (InSize == sizeof(double))
//		Value = *reinterpret_cast<const double*>(InValue);
//	else
//		checkf(false, TEXT("Invalid size for float value: %d"), InSize);
//}
//
//FString FConfigArrayValue::ToString() const
//{
//	
//}
//
//void FConfigArrayValue::LowLevelSetValue(const void* InValue, uint64 InSize)
//{
//	Value = *reinterpret_cast<const TArray<FConfigValue*>*>(InValue);
//}
//
//FString FConfigObjectValue::ToString() const
//{
//	return FString();
//}
//
//void FConfigObjectValue::LowLevelSetValue(const void* InValue, uint64 InSize)
//{
//	Value = *reinterpret_cast<const TMap<FString, FConfigValue*>*>(InValue);
//}
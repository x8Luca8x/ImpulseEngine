#include "Containers/ImpulseString.h"

FString FString::DefaultTrimChars = TEXT(" \t\r\n\v\f");

FString::FString(FString&& Other) noexcept
{
	m_Data = MoveTemp(Other.m_Data);
	m_Length = Other.m_Length;

	Other.m_Length = 0;
}

FString::FString(const TCHAR* InData)
{
	m_Length = FPlatformString::Strlen(InData);
	Copy(InData, m_Length);
}

FString& FString::operator=(FString&& Other) noexcept
{
	m_Data = MoveTemp(Other.m_Data);
	m_Length = Other.m_Length;

	Other.m_Length = 0;

	return *this;
}

FString& FString::operator=(const TCHAR* InData)
{
	m_Length = FPlatformString::Strlen(InData);
	Copy(InData, m_Length);

	return *this;
}

int32 FString::Length() const
{
	return m_Length;
}

int32 FString::Size() const
{
	return m_Data.Capacity();
}

FString FString::Left(int32 InIndex) const
{
	checkf(InIndex > 0 && InIndex < m_Length, TEXT("Invalid index."));

	// Prepare result

	FString result;
	result.Allocate(InIndex);

	// Copy data

	result.Copy(m_Data.GetData(), InIndex);

	return result;
}

FString FString::Right(int32 InIndex) const
{
	checkf(InIndex >= 0 && InIndex < (m_Length - 1), TEXT("Invalid index."));

	// Real index is +1 because we don't want to copy the index itself
	const int32 realIndex = InIndex + 1;

	// Prepare result

	FString result;
	result.Allocate(m_Length - realIndex);

	// Copy data

	result.Copy(m_Data.GetData() + realIndex, m_Length - realIndex);

	return result;
}

FString FString::Mid(int32 InStartIndex, int32 InLength) const
{
	checkf(InStartIndex >= 0 && InStartIndex < m_Length, TEXT("Invalid start index."));
	checkf(InLength > 0 && InStartIndex + InLength <= m_Length, TEXT("Invalid length."));

	// Prepare result

	FString result;
	result.Allocate(InLength);

	// Copy data

	result.Copy(m_Data.GetData() + InStartIndex, InLength);

	return result;
}

int32 FString::Find(const FString& InSubStr, ESearchCase InSearchCase, ESearchDir InSearchDir, int32 InStartIndex) const
{
	const int32 subStrLength = InSubStr.Length();
	if (subStrLength == 0)
		return INDEX_NONE;

	const TCHAR* subStr = *InSubStr;

	if (InSearchDir == ESearchDir::FromStart)
		return FindForward(subStr, subStrLength, InStartIndex, InSearchCase);
	else
		return FindBackward(subStr, subStrLength, InStartIndex, InSearchCase);
}

TArray<int32> FString::FindAll(const FString& InSubStr, ESearchCase InSearchCase, ESearchDir InSearchDir, int32 InStartIndex) const
{
	TArray<int32> result;

	const int32 subStrLength = InSubStr.Length();
	if (subStrLength == 0)
		return result;

	const TCHAR* subStr = *InSubStr;

	if (InSearchDir == ESearchDir::FromStart)
	{
		int32 index = FindForward(subStr, subStrLength, InStartIndex, InSearchCase);
		while (index != INDEX_NONE)
		{
			result.Add(index);
			index = FindForward(subStr, subStrLength, index + subStrLength, InSearchCase);
		}
	}
	else
	{
		int32 index = FindBackward(subStr, subStrLength, InStartIndex, InSearchCase);
		while (index != INDEX_NONE)
		{
			result.Add(index);
			index = FindBackward(subStr, subStrLength, m_Length - index + subStrLength, InSearchCase);
		}
	}

	return result;
}

bool FString::Contains(const FString& InSubStr, ESearchCase InSearchCase) const
{
	return Find(InSubStr, InSearchCase) != INDEX_NONE;
}

bool FString::ContainsChar(TCHAR InChar, ESearchCase InSearchCase) const
{
	for (int32 i = 0; i < m_Length; ++i)
	{
		if (FPlatformString::Compare(&m_Data[i], &InChar, 1, InSearchCase))
			return true;
	}

	return false;
}

bool FString::StartsWith(const FString& InSubStr, ESearchCase InSearchCase) const
{
	if (InSubStr.Length() > m_Length)
		return false;

	return FPlatformString::Compare(m_Data.GetData(), *InSubStr, InSubStr.Length(), InSearchCase);
}

bool FString::EndsWith(const FString& InSubStr, ESearchCase InSearchCase) const
{
	if (InSubStr.Length() > m_Length)
		return false;

	return FPlatformString::Compare(m_Data.GetData() + (m_Length - InSubStr.Length()), *InSubStr, InSubStr.Length(), InSearchCase);
}

bool FString::Equals(const FString& InOther, ESearchCase InSearchCase) const
{
	if (m_Length != InOther.Length())
		return false;

	return FPlatformString::Compare(m_Data.GetData(), *InOther, m_Length, InSearchCase);
}

bool FString::IsEmpty() const
{
	return m_Data.Num() == 0;
}

void FString::Empty()
{
	m_Data.Empty();
	m_Length = 0;
}

bool FString::IsWhiteSpace() const
{
	for (int32 i = 0; i < m_Length; ++i)
	{
		if (!TChar<TCHAR>::IsWhiteSpace(m_Data[i]))
			return false;
	}

	return true;
}

void FString::InlineTrimStart(const TCHAR* InTrimChars)
{
	int32 trimCharsLength = 0;
	if (InTrimChars)
		trimCharsLength = FPlatformString::Strlen(InTrimChars);
	else
	{
		InTrimChars = *DefaultTrimChars;
		trimCharsLength = DefaultTrimChars.Length();
	}

	for (int32 i = 0; i < m_Length; ++i)
	{
		bool found = false;
		for (int32 j = 0; j < trimCharsLength; ++j)
		{
			if (m_Data[i] == InTrimChars[j])
			{
				found = true;
				break;
			}
		}

		if (!found)
		{
			// We found the first non-trim char, so we can stop here
			// and copy the rest of the string

			if (i == 0)
				return;

			const int32 newLength = m_Length - i;
			FMemory::Memmove(m_Data.GetData(), &m_Data[i], newLength * sizeof(TCHAR));

			m_Data[newLength] = 0;
			m_Length = newLength;

			Allocate(m_Length);

			return;
		}
	}
}

void FString::InlineTrimEnd(const TCHAR* InTrimChars)
{
	int32 trimCharsLength = 0;
	if (InTrimChars)
		trimCharsLength = FPlatformString::Strlen(InTrimChars);
	else
	{
		InTrimChars = *DefaultTrimChars;
		trimCharsLength = DefaultTrimChars.Length();
	}

	for (int32 i = m_Length - 1; i >= 0; --i)
	{
		bool found = false;
		for (int32 j = 0; j < trimCharsLength; ++j)
		{
			if (m_Data[i] == InTrimChars[j])
			{
				found = true;
				break;
			}
		}

		if (!found)
		{
			// We found the first non-trim char, so we can stop here
			// and copy the rest of the string

			if (i == m_Length - 1)
				return;

			const int32 newLength = i + 1;
			m_Data[newLength] = 0;
			m_Length = newLength;

			Allocate(m_Length);

			return;
		}
	}
}

void FString::InlineTrim(const TCHAR* InTrimChars)
{
	InlineTrimStart(InTrimChars);
	InlineTrimEnd(InTrimChars);
}

FString FString::TrimStart(const TCHAR* InTrimChars) const
{
	FString copy = *this;
	copy.InlineTrimStart(InTrimChars);

	return copy;
}

FString FString::TrimEnd(const TCHAR* InTrimChars) const
{
	FString copy = *this;
	copy.InlineTrimEnd(InTrimChars);

	return copy;
}

FString FString::Trim(const TCHAR* InTrimChars) const
{
	FString copy = *this;
	copy.InlineTrim(InTrimChars);

	return copy;
}

void FString::InlineAppend(const FString& InOther)
{
	Allocate(m_Length + InOther.Length());

	FMemory::Memcpy(m_Data.GetData() + m_Length, *InOther, InOther.Length() * sizeof(TCHAR));
	m_Length += InOther.Length();

	m_Data[m_Length] = 0;
}

FString FString::Append(const FString& InOther) const
{
	FString result = *this;
	result.InlineAppend(InOther);

	return result;
}

void FString::InlineAppendChar(TCHAR InChar)
{
	Allocate(m_Length + 1);

	m_Data[m_Length++] = InChar;
	m_Data[m_Length] = 0;
}

FString FString::AppendChar(TCHAR InChar) const
{
	FString result = *this;
	result.InlineAppendChar(InChar);

	return result;
}

void FString::Allocate(int32 InNum)
{
	const int32 allocNum = InNum + 1;
	if (allocNum == Size())
		return;

	m_Data.SetNumUninitialized(allocNum);
}

void FString::Copy(const TCHAR* InData, int32 InNum)
{
	Allocate(InNum);
	FMemory::Memcpy(m_Data.GetData(), InData, InNum * sizeof(TCHAR));

	m_Data[InNum] = 0;
	m_Length = InNum;
}

int32 FString::FindForward(const TCHAR* InSubStr, int32 InSubStrLength, int32 InStartIndex, ESearchCase SearchCase) const
{
	const int32 len = m_Length - InSubStrLength;
	for (int32 i = InStartIndex; i < len; ++i)
	{
		if (FPlatformString::Compare(&m_Data.GetData()[i], InSubStr, InSubStrLength, SearchCase))
			return i;
	}

	return INDEX_NONE;
}

int32 FString::FindBackward(const TCHAR* InSubStr, int32 InSubStrLength, int32 InStartIndex, ESearchCase SearchCase) const
{
	const int32 start = m_Length - InStartIndex;
	for (int32 i = start; i >= 0; --i)
	{
		if (FPlatformString::Compare(&m_Data.GetData()[i], InSubStr, InSubStrLength, SearchCase))
			return i;
	}

	return INDEX_NONE;
}

FString FString::Printf(const TCHAR* InFormat, ...)
{
	TCHAR stringBuffer[STRING_PRINTF_BUFFER_SIZE];
	TCHAR* buffer = stringBuffer;

	int32 resultCode = -1;
	int32 bufferSize = STRING_PRINTF_BUFFER_SIZE;

	va_list args;
	va_start(args, InFormat);

	resultCode = FPlatformString::Vsnprintf(buffer, bufferSize, InFormat, args);

	// If resultCode is -1, it means that the buffer was too small
	// So we increase the buffer until it's big enough

	if (resultCode == -1)
	{
		buffer = nullptr;
		while (resultCode == -1);
		{
			bufferSize *= 2;
			buffer = (TCHAR*)FMemory::Realloc(buffer, bufferSize * sizeof(TCHAR));

			resultCode = FPlatformString::Vsnprintf(buffer, bufferSize, InFormat, args);
		}
	}

	va_end(args);

	buffer[resultCode] = 0;
	FString result(buffer);

	if (bufferSize != STRING_PRINTF_BUFFER_SIZE)
		FMemory::Free(buffer);

	return result;
}

bool FString::ToInt8Save(int8& OutValue) const
{
	return FPlatformString::ToInt8(**this, Length(), OutValue);
}

bool FString::ToInt16Save(int16& OutValue) const
{
	return FPlatformString::ToInt16(**this, Length(), OutValue);
}

bool FString::ToInt32Save(int32& OutValue) const
{
	return FPlatformString::ToInt32(**this, Length(), OutValue);
}

bool FString::ToInt64Save(int64& OutValue) const
{
	return FPlatformString::ToInt64(**this, Length(), OutValue);
}

bool FString::ToUInt8Save(uint8& OutValue) const
{
	return FPlatformString::ToUInt8(**this, Length(), OutValue);
}

bool FString::ToUInt16Save(uint16& OutValue) const
{
	return FPlatformString::ToUInt16(**this, Length(), OutValue);
}

bool FString::ToUInt32Save(uint32& OutValue) const
{
	return FPlatformString::ToUInt32(**this, Length(), OutValue);
}

bool FString::ToUInt64Save(uint64& OutValue) const
{
	return FPlatformString::ToUInt64(**this, Length(), OutValue);
}

bool FString::ToFloatSave(float& OutValue) const
{
	return FPlatformString::ToFloat(**this, Length(), OutValue);
}

bool FString::ToDoubleSave(double& OutValue) const
{
	return FPlatformString::ToDouble(**this, Length(), OutValue);
}

int8 FString::ToInt8() const
{
	int8 result{};
	checkf(ToInt8Save(result), TEXT("Failed to convert string to int8."));

	return result;
}

int16 FString::ToInt16() const
{
	int16 result{};
	checkf(ToInt16Save(result), TEXT("Failed to convert string to int16."));

	return result;
}

int32 FString::ToInt32() const
{
	int32 result{};
	checkf(ToInt32Save(result), TEXT("Failed to convert string to int32."));

	return result;
}

int64 FString::ToInt64() const
{
	int64 result{};
	checkf(ToInt64Save(result), TEXT("Failed to convert string to int64."));

	return result;
}

uint8 FString::ToUInt8() const
{
	uint8 result{};
	checkf(ToUInt8Save(result), TEXT("Failed to convert string to uint8."));

	return result;
}

uint16 FString::ToUInt16() const
{
	uint16 result{};
	checkf(ToUInt16Save(result), TEXT("Failed to convert string to uint16."));

	return result;
}

uint32 FString::ToUInt32() const
{
	uint32 result{};
	checkf(ToUInt32Save(result), TEXT("Failed to convert string to uint32."));

	return result;
}

uint64 FString::ToUInt64() const
{
	uint64 result{};
	checkf(ToUInt64Save(result), TEXT("Failed to convert string to uint64."));

	return result;
}

float FString::ToFloat() const
{
	float result{};
	checkf(ToFloatSave(result), TEXT("Failed to convert string to float."));

	return result;
}

double FString::ToDouble() const
{
	double result{};
	checkf(ToDoubleSave(result), TEXT("Failed to convert string to double."));

	return result;
}

FString FString::FromInt8(int8 InValue)
{
	TCHAR buffer[64];
	FPlatformString::FromIntegral<int8>(InValue, buffer, 64);

	return FString(buffer);
}

FString FString::FromInt16(int16 InValue)
{
	TCHAR buffer[64];
	FPlatformString::FromIntegral<int16>(InValue, buffer, 64);

	return FString(buffer);
}

FString FString::FromInt32(int32 InValue)
{
	TCHAR buffer[64];
	FPlatformString::FromIntegral<int32>(InValue, buffer, 64);

	return FString(buffer);
}

FString FString::FromInt64(int64 InValue)
{
	TCHAR buffer[64];
	FPlatformString::FromIntegral<int64>(InValue, buffer, 64);

	return FString(buffer);
}

FString FString::FromUInt8(uint8 InValue)
{
	TCHAR buffer[64];
	FPlatformString::FromIntegral<uint8>(InValue, buffer, 64);

	return FString(buffer);
}

FString FString::FromUInt16(uint16 InValue)
{
	TCHAR buffer[64];
	FPlatformString::FromIntegral<uint16>(InValue, buffer, 64);

	return FString(buffer);
}

FString FString::FromUInt32(uint32 InValue)
{
	TCHAR buffer[64];
	FPlatformString::FromIntegral<uint32>(InValue, buffer, 64);

	return FString(buffer);
}

FString FString::FromUInt64(uint64 InValue)
{
	TCHAR buffer[64];
	FPlatformString::FromIntegral<uint64>(InValue, buffer, 64);

	return FString(buffer);
}

FString FString::FromFloat(float InValue, int32 MinFractionalDigits)
{
	TCHAR buffer[64];
	FPlatformString::FromFloating<float>(InValue, buffer, 64, MinFractionalDigits);

	return FString(buffer);
}

FString FString::FromDouble(double InValue, int32 MinFractionalDigits)
{
	TCHAR buffer[64];
	FPlatformString::FromFloating<double>(InValue, buffer, 64, MinFractionalDigits);

	return FString(buffer);
}

FArchive& operator<<(FArchive& Ar, FString& Value)
{
#if ENCRYPT_ARCHIVE_STRING

	if (Ar.IsSaving())
	{
		// Before serializing the string, we need to encrypt it

		FString copy = Value;
		for (int32 i = 0; i < copy.Length(); ++i)
			copy[i] = ~copy[i];

		Ar << copy.m_Length;
		Ar.Serialize(copy.m_Data.GetData(), copy.m_Length * sizeof(TCHAR));
	}
	else
	{
		int32 length = 0;
		Ar << length;

		Value.Allocate(length);
		Ar.Serialize(Value.m_Data.GetData(), length * sizeof(TCHAR));

		Value.m_Length = length;
		Value.m_Data[length] = 0;

		// After serializing the string, we need to decrypt it

		for (int32 i = 0; i < Value.Length(); ++i)
			Value[i] = ~Value[i];
	}

#else

	if (Ar.IsSaving())
	{
		Ar << Value.m_Length;
		Ar.Serialize(Value.m_Data.GetData(), Value.m_Length * sizeof(TCHAR));
	}
	else
	{
		int32 length = 0;
		Ar << length;

		Value.Allocate(length);
		Ar.Serialize(Value.m_Data.GetData(), length * sizeof(TCHAR));

		Value.m_Length = length;
		Value.m_Data[length] = 0;
	}

#endif

	return Ar;
}
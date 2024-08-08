#include "Serialization/Archive.h"

void FMemoryArchive::Serialize(void* Data, uint64 Num)
{
	if (bIsLoading)
	{
		if (Pos + Num > TotalSize())
		{
			bIsError = true;
			return;
		}

		FMemory::Memcpy(Data, &DataRef[static_cast<int32>(Pos)], Num);
	}
	else
	{
		DataRef.SetNumUninitialized(static_cast<int32>(Pos + Num));
		FMemory::Memcpy(&DataRef[static_cast<int32>(Pos)], Data, Num);
	}

	Pos += Num;
}

void FMemoryArchive::Seek(uint64 InPos)
{
	Pos = InPos;
}

uint64 FMemoryArchive::Tell()
{
	return Pos;
}

uint64 FMemoryArchive::TotalSize()
{
	return static_cast<uint64>(DataRef.Num());
}

bool FMemoryArchive::IsLoading()
{
	return bIsLoading;
}

bool FMemoryArchive::IsSaving()
{
	return bIsSaving;
}

bool FMemoryArchive::IsError()
{
	return bIsError;
}

bool FMemoryArchive::AtEnd()
{
	return Pos >= TotalSize();
}

void FMemoryArchive::Flush()
{
}

void FMemoryArchive::Close()
{
}

FArchive& operator<<(FArchive& Ar, bool& Value)
{
	Ar.Serialize(&Value, sizeof(bool));
	return Ar;
}

FArchive& operator<<(FArchive& Ar, int8& Value)
{
	Ar.Serialize(&Value, sizeof(int8));
	return Ar;
}

FArchive& operator<<(FArchive& Ar, uint8& Value)
{
	Ar.Serialize(&Value, sizeof(uint8));
	return Ar;
}

FArchive& operator<<(FArchive& Ar, int16& Value)
{
	Ar.Serialize(&Value, sizeof(int16));
	return Ar;
}

FArchive& operator<<(FArchive& Ar, uint16& Value)
{
	Ar.Serialize(&Value, sizeof(uint16));
	return Ar;
}

FArchive& operator<<(FArchive& Ar, int32& Value)
{
	Ar.Serialize(&Value, sizeof(int32));
	return Ar;
}

FArchive& operator<<(FArchive& Ar, uint32& Value)
{
	Ar.Serialize(&Value, sizeof(uint32));
	return Ar;
}

FArchive& operator<<(FArchive& Ar, int64& Value)
{
	Ar.Serialize(&Value, sizeof(int64));
	return Ar;
}

FArchive& operator<<(FArchive& Ar, uint64& Value)
{
	Ar.Serialize(&Value, sizeof(uint64));
	return Ar;
}

FArchive& operator<<(FArchive& Ar, float& Value)
{
	Ar.Serialize(&Value, sizeof(float));
	return Ar;
}

FArchive& operator<<(FArchive& Ar, double& Value)
{
	Ar.Serialize(&Value, sizeof(double));
	return Ar;
}
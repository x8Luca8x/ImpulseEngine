#pragma once

#include "CoreModule.h"

#include "Containers/Array.h"

class CORE_API FArchive
{
public:
	FArchive() = default;
	virtual ~FArchive() = default;

	/**
	* Serializes a chunk of data.
	* @param Data - Pointer to the data to serialize.
	* @param Num - Number of bytes to read/write.
	*/
	virtual void Serialize(void* Data, uint64 Num) = 0;

	/**
	* Seeks to the specified position.
	* @param InPos - Position to seek to.
	*/
	virtual void Seek(uint64 InPos) = 0;

	/**
	* Returns the current position.
	* @return Current position.
	*/
	virtual uint64 Tell() = 0;

	/**
	* Returns the total size of the data being read/written.
	* @return Total size.
	*/
	virtual uint64 TotalSize() = 0;

	/**
	* Returns true if this archive is for loading data, false if it is for saving data.
	* @return true if this archive is for loading data, false if it is for saving data.
	*/
	virtual bool IsLoading() = 0;

	/**
	* Returns true if this archive is for saving data, false if it is for loading data.
	* @return true if this archive is for saving data, false if it is for loading data.
	*/
	virtual bool IsSaving() = 0;

	/**
	* Returns true if this archive encountered an error during serialization.
	* @return true if this archive encountered an error during serialization.
	*/
	virtual bool IsError() = 0;

	/**
	* Returns true if this archive has reached the end of the data to read/write.
	* @return true if this archive has reached the end of the data to read/write.
	*/
	virtual bool AtEnd() = 0;

	/**
	* Flushes any cached data to the underlying archive.
	*/
	virtual void Flush() = 0;

	/**
	* Closes the archive.
	*/
	virtual void Close() = 0;

public:

	friend CORE_API FArchive& operator<<(FArchive& Ar, bool& Value);
	friend CORE_API FArchive& operator<<(FArchive& Ar, int8& Value);
	friend CORE_API FArchive& operator<<(FArchive& Ar, uint8& Value);
	friend CORE_API FArchive& operator<<(FArchive& Ar, int16& Value);
	friend CORE_API FArchive& operator<<(FArchive& Ar, uint16& Value);
	friend CORE_API FArchive& operator<<(FArchive& Ar, int32& Value);
	friend CORE_API FArchive& operator<<(FArchive& Ar, uint32& Value);
	friend CORE_API FArchive& operator<<(FArchive& Ar, int64& Value);
	friend CORE_API FArchive& operator<<(FArchive& Ar, uint64& Value);
	friend CORE_API FArchive& operator<<(FArchive& Ar, float& Value);
	friend CORE_API FArchive& operator<<(FArchive& Ar, double& Value);

	template<typename T, typename Allocator = TDefaultAllocator<T>>
	friend FArchive& operator<<(FArchive& Ar, TArray<T, Allocator>& Array)
	{
		if (Ar.IsLoading())
		{
			int32 num;

			Ar << num;
			Array.Empty(num);

			for (int32 i = 0; i < num; ++i)
			{
				T value;
				Ar << value;

				Array.Add(MoveTemp(value));
			}
		}
		else
		{
			int32 num = Array.Num();

			Ar << num;
			for (int32 i = 0; i < num; ++i)
				Ar << Array[i];
		}

		return Ar;
	}
};

class CORE_API FMemoryArchive : public FArchive
{
public:

	virtual void Serialize(void* Data, uint64 Num) override;
	virtual void Seek(uint64 InPos) override;

	virtual uint64 Tell() override;
	virtual uint64 TotalSize() override;

	virtual bool IsLoading() override;
	virtual bool IsSaving() override;

	virtual bool IsError() override;
	virtual bool AtEnd() override;
	virtual void Flush() override;
	virtual void Close() override;

protected:

	FMemoryArchive(TArray<uint8>& InData, bool bInIsLoading)
		: DataRef(InData), bIsLoading(bInIsLoading), bIsSaving(!bInIsLoading) {}

	TArray<uint8>& DataRef;
	uint64 Pos = 0;

	bool bIsLoading = false;
	bool bIsSaving = false;

	bool bIsError = false;
};

/**
* Archive for reading from memory into a buffer.
*/
class CORE_API FMemoryReader : public FMemoryArchive
{
public:

	FMemoryReader(TArray<uint8>& InData)
		: FMemoryArchive(InData, true) {}

	virtual ~FMemoryReader() = default;
};

/**
* Archive for writing to memory from a buffer.
*/
class CORE_API FMemoryWriter : public FMemoryArchive
{
public:

	FMemoryWriter(TArray<uint8>& InData)
		: FMemoryArchive(InData, false) {}

	virtual ~FMemoryWriter() = default;
};
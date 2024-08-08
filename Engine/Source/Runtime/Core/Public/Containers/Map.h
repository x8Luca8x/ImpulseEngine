#pragma once

#include "Set.h"
#include "Array.h"

#include "Debug/ImpulseDebug.h"

template<typename KeyType, typename ValueType>
class TPair
{

	struct FKey
	{
		FKey(const KeyType& InKey)
			: Key(InKey), Hash(0)
		{
			Hash = GetTypeHash(Key);
		}

		FKey(KeyType&& InKey)
			: Key(MoveTemp(InKey)), Hash(0)
		{
			Hash = GetTypeHash(Key);
		}

		FORCEINLINE bool operator==(const uint32 InHash) const { return InHash == Hash; }
		FORCEINLINE bool operator==(const FKey& Other) const { return Hash == Other.Hash; }

		KeyType Key;
		uint32 Hash;
	};

public:

	TPair(const KeyType& InKey)
		: Key(InKey) {}
	TPair(const KeyType& InKey, const ValueType& InValue)
		: Key(InKey), Value(InValue) {}
	TPair(const KeyType& InKey, ValueType&& InValue)
		: Key(InKey), Value(MoveTemp(InValue)) {}
	TPair(KeyType&& InKey)
		: Key(MoveTemp(InKey)) {}
	TPair(KeyType&& InKey, const ValueType& InValue)
		: Key(MoveTemp(InKey)), Value(InValue) {}
	TPair(KeyType&& InKey, ValueType&& InValue)
		: Key(MoveTemp(InKey)), Value(MoveTemp(InValue)) {}

	FORCEINLINE bool operator==(const TPair& Other) const { return Key == Other.Key; }

	FORCEINLINE uint32 GetKeyHash() const { return Key.Hash; }

	FORCEINLINE const KeyType& GetKey() const { return Key.Key; }

	FORCEINLINE ValueType& GetValue() { return Value; }
	FORCEINLINE const ValueType& GetValue() const { return Value; }

public:

	FKey Key;
	ValueType Value;
};

template<typename KeyType, typename ValueType>
inline uint32 GetTypeHash(const TPair<KeyType, ValueType>& Pair)
{
	return Pair.GetKeyHash();
}

template<typename KeyType, typename ValueType>
using TMapIterator = TSetIterator<TPair<KeyType, ValueType>>;
template<typename KeyType, typename ValueType>
using TMapConstIterator = TSetConstIterator<TPair<KeyType, ValueType>>;

template<typename KeyType, typename ValueType>
class TMap
{
public:

	/**
	* Adds a new key/value pair to the map
	* @param Key - The key to use for association.
	* @param Value - The value to associate with the key.
	* @return True if the pair was added successfully, false if there was a collision with an existing key.
	*/
	bool Add(const KeyType& Key, const ValueType& Value);

	/**
	* Adds a new key/value pair to the map
	* @param Key - The key to use for association.
	* @param Value - The value to associate with the key.
	* @return True if the pair was added successfully, false if there was a collision with an existing key.
	*/
	bool Add(const KeyType& Key, ValueType&& Value);

	/**
	* Adds a new key/value pair to the map
	* @param Key - The key to use for association.
	* @param Value - The value to associate with the key.
	* @return True if the pair was added successfully, false if there was a collision with an existing key.
	*/
	bool Add(KeyType&& Key, const ValueType& Value);

	/**
	* Adds a new key/value pair to the map
	* @param Key - The key to use for association.
	* @param Value - The value to associate with the key.
	* @return True if the pair was added successfully, false if there was a collision with an existing key.
	*/
	bool Add(KeyType&& Key, ValueType&& Value);

	/**
	* Adds a new key and initializes the value with a default-constructed item.
	* @param Key - The key to use for association.
	* @return A reference to the value associated with the given key.
	*/
	ValueType& Add(const KeyType& Key)
	{
		const uint32 hash = GetTypeHash(Key);
		AddByHash(hash, Key, ValueType());

		return *FindByHash(hash, Key);
	}

	/**
	* Adds a new key and initializes the value with a default-constructed item.
	* @param Key - The key to use for association.
	* @return A reference to the value associated with the given key.
	*/
	ValueType& Add(KeyType&& Key)
	{
		const uint32 hash = GetTypeHash(Key);
		AddByHash(hash, MoveTemp(Key), ValueType());

		return *FindByHash(hash, Key);
	}

	/**
	* Finds or adds a value for the given key
	* @param Key - The key to search for
	* @param Value - The value to associate with the key if it doesn't already exist.
	* @return A reference to the value associated with the given key.
	*/
	ValueType& FindOrAdd(const KeyType& Key, const ValueType& Value)
	{
		const uint32 hash = GetTypeHash(Key);
		return FindOrAddByHash(hash, Key, Value);
	}

	/**
	* Finds or adds a value for the given key
	* @param Key - The key to search for
	* @param Value - The value to associate with the key if it doesn't already exist.
	* @return A reference to the value associated with the given key.
	*/
	ValueType& FindOrAdd(const KeyType& Key, ValueType&& Value)
	{
		const uint32 hash = GetTypeHash(Key);
		return FindOrAddByHash(hash, Key, MoveTemp(Value));
	}

	/**
	* Finds or adds a value for the given key
	* @param Key - The key to search for
	* @param Value - The value to associate with the key if it doesn't already exist.
	* @return A reference to the value associated with the given key.
	*/
	ValueType& FindOrAdd(KeyType&& Key, const ValueType& Value)
	{
		const uint32 hash = GetTypeHash(Key);
		return FindOrAddByHash(hash, MoveTemp(Key), Value);
	}

	/**
	* Finds or adds a value for the given key
	* @param Key - The key to search for
	* @param Value - The value to associate with the key if it doesn't already exist.
	* @return A reference to the value associated with the given key.
	*/
	ValueType& FindOrAdd(KeyType&& Key, ValueType&& Value)
	{
		const uint32 hash = GetTypeHash(Key);
		return FindOrAddByHash(hash, MoveTemp(Key), MoveTemp(Value));
	}

	/**
	* Searches for an element with the given key
	* @param Key - The key to search for
	* @return A pointer to the value associated with the given key, or nullptr if none exists.
	*/
	ValueType* Find(const KeyType& Key)
	{
		const uint32 hash = GetTypeHash(Key);
		return FindByHash(hash, Key);
	}

	/**
	* Searches for an element with the given key
	* @param Key - The key to search for
	* @return A pointer to the value associated with the given key, or nullptr if none exists.
	*/
	const ValueType* Find(const KeyType& Key) const
	{
		const uint32 hash = GetTypeHash(Key);
		return FindByHash(hash, Key);
	}

	/**
	* Searches for an element with the given key
	* @param Key - The key to search for
	* @return A reference to the value associated with the given key, or asserts if none exists.
	*/
	ValueType& FindChecked(const KeyType& Key)
	{
		ValueType* value = Find(Key);
		checkf(value, TEXT("Key not found"));

		return *value;
	}

	/**
	* Searches for an element with the given key
	* @param Key - The key to search for
	* @return A reference to the value associated with the given key, or asserts if none exists.
	*/
	const ValueType& FindChecked(const KeyType& Key) const
	{
		const ValueType* value = Find(Key);
		checkf(value, TEXT("Key not found"));

		return *value;
	}

	/**
	* Removes an element with the given key
	* @param Key - The key to search for
	* @return True if an element was removed, false if none existed with the given key.
	*/
	bool Remove(const KeyType& Key)
	{
		const uint32 hash = GetTypeHash(Key);
		return RemoveByHash(hash, Key);
	}

	/**
	* Checks if the map contains the given key
	* @param Key - The key to search for
	* @return True if the map contains the given key, false otherwise.
	*/
	bool Contains(const KeyType& Key) const
	{
		const uint32 hash = GetTypeHash(Key);
		return FindByHash(hash, Key) != nullptr;
	}

	/**
	* Generates an array from the keys in this map
	* @param OutKeys - Will hold the generated keys.
	*/
	void GenerateKeyArray(TArray<KeyType>& OutKeys) const
	{
		OutKeys.Reserve(m_Set.Num());

		for (const auto& pair : m_Set)
			OutKeys.Add(pair.GetKey());
	}

	/**
	* Generates an array from the values in this map
	* @param OutValues - Will hold the generated values.
	*/
	void GenerateValueArray(TArray<ValueType>& OutValues) const
	{
		OutValues.Reserve(m_Set.Num());

		for (const auto& pair : m_Set)
			OutValues.Add(pair.GetValue());
	}

	/**
	* Clears the map, removing all key/value pairs
	*/
	FORCEINLINE void Clear() { m_Set.Clear(); }

	/**
	* Gets the number of elements in the map
	* @return The number of elements in the map.
	*/
	FORCEINLINE int32 Num() const { return m_Set.Num(); }

public:

	ValueType& operator[](const KeyType& Key) { return FindOrAdd(Key, ValueType()); }
	const ValueType& operator[](const KeyType& Key) const { return FindOrAdd(Key, ValueType()); }

public:

	TMapIterator<KeyType, ValueType> begin() { return TMapIterator<KeyType, ValueType>(m_Set.begin()); }
	TMapIterator<KeyType, ValueType> end() { return TMapIterator<KeyType, ValueType>(m_Set.end()); }

	TMapConstIterator<KeyType, ValueType> begin() const { return TMapConstIterator<KeyType, ValueType>(m_Set.begin()); }
	TMapConstIterator<KeyType, ValueType> end() const { return TMapConstIterator<KeyType, ValueType>(m_Set.end()); }

public:

	bool AddByHash(uint32 KeyHash, const KeyType& Key, const ValueType& Value);
	bool AddByHash(uint32 KeyHash, KeyType&& Key, const ValueType& Value);
	bool AddByHash(uint32 KeyHash, const KeyType& Key, ValueType&& Value);
	bool AddByHash(uint32 KeyHash, KeyType&& Key, ValueType&& Value);

	ValueType& FindOrAddByHash(uint32 KeyHash, const KeyType& Key, const ValueType& Value);
	ValueType& FindOrAddByHash(uint32 KeyHash, const KeyType& Key, ValueType&& Value);
	ValueType& FindOrAddByHash(uint32 KeyHash, KeyType&& Key, const ValueType& Value);
	ValueType& FindOrAddByHash(uint32 KeyHash, KeyType&& Key, ValueType&& Value);

	bool RemoveByHash(uint32 KeyHash, const KeyType& Key) { return m_Set.RemoveByHash(KeyHash); }

	ValueType* FindByHash(uint32 KeyHash, const KeyType& Key);
	const ValueType* FindByHash(uint32 KeyHash, const KeyType& Key) const;

private:

	TSet<TPair<KeyType, ValueType>> m_Set;
};

template<typename KeyType, typename ValueType>
inline bool TMap<KeyType, ValueType>::Add(const KeyType& Key, const ValueType& Value)
{
	const uint32 hash = GetTypeHash(Key);
	return AddByHash(hash, Key, Value);
}

template<typename KeyType, typename ValueType>
inline bool TMap<KeyType, ValueType>::Add(const KeyType& Key, ValueType&& Value)
{
	const uint32 hash = GetTypeHash(Key);
	return AddByHash(hash, Key, MoveTemp(Value));
}

template<typename KeyType, typename ValueType>
inline bool TMap<KeyType, ValueType>::Add(KeyType&& Key, const ValueType& Value)
{
	const uint32 hash = GetTypeHash(Key);
	return AddByHash(hash, MoveTemp(Key), Value);
}

template<typename KeyType, typename ValueType>
inline bool TMap<KeyType, ValueType>::Add(KeyType&& Key, ValueType&& Value)
{
	const uint32 hash = GetTypeHash(Key);
	return AddByHash(hash, MoveTemp(Key), MoveTemp(Value));
}

template<typename KeyType, typename ValueType>
inline bool TMap<KeyType, ValueType>::AddByHash(uint32 KeyHash, const KeyType& Key, const ValueType& Value)
{
	return m_Set.AddByHash(TPair<KeyType, ValueType>(Key, Value), KeyHash);
}

template<typename KeyType, typename ValueType>
inline bool TMap<KeyType, ValueType>::AddByHash(uint32 KeyHash, KeyType&& Key, const ValueType& Value)
{
	return m_Set.AddByHash(TPair<KeyType, ValueType>(MoveTemp(Key), Value), KeyHash);
}

template<typename KeyType, typename ValueType>
inline bool TMap<KeyType, ValueType>::AddByHash(uint32 KeyHash, const KeyType& Key, ValueType&& Value)
{
	return m_Set.AddByHash(TPair<KeyType, ValueType>(Key, MoveTemp(Value)), KeyHash);
}

template<typename KeyType, typename ValueType>
inline bool TMap<KeyType, ValueType>::AddByHash(uint32 KeyHash, KeyType&& Key, ValueType&& Value)
{
	return m_Set.AddByHash(TPair<KeyType, ValueType>(MoveTemp(Key), MoveTemp(Value)), KeyHash);
}

template<typename KeyType, typename ValueType>
inline ValueType& TMap<KeyType, ValueType>::FindOrAddByHash(uint32 KeyHash, const KeyType& Key, const ValueType& Value)
{
	ValueType* value = FindByHash(KeyHash, Key);
	if (value)
		return *value;

	AddByHash(KeyHash, Key, Value);
	return *FindByHash(KeyHash, Key);
}

template<typename KeyType, typename ValueType>
inline ValueType& TMap<KeyType, ValueType>::FindOrAddByHash(uint32 KeyHash, const KeyType& Key, ValueType&& Value)
{
	ValueType* value = FindByHash(KeyHash, Key);
	if (value)
		return *value;

	AddByHash(KeyHash, Key, MoveTemp(Value));
	return *FindByHash(KeyHash, Key);
}

template<typename KeyType, typename ValueType>
inline ValueType& TMap<KeyType, ValueType>::FindOrAddByHash(uint32 KeyHash, KeyType&& Key, const ValueType& Value)
{
	ValueType* value = FindByHash(KeyHash, Key);
	if (value)
		return *value;

	AddByHash(KeyHash, MoveTemp(Key), Value);
	return *FindByHash(KeyHash, Key);
}

template<typename KeyType, typename ValueType>
inline ValueType& TMap<KeyType, ValueType>::FindOrAddByHash(uint32 KeyHash, KeyType&& Key, ValueType&& Value)
{
	ValueType* value = FindByHash(KeyHash, Key);
	if (value)
		return *value;

	AddByHash(KeyHash, MoveTemp(Key), MoveTemp(Value));
	return *FindByHash(KeyHash, Key);
}

template<typename KeyType, typename ValueType>
inline ValueType* TMap<KeyType, ValueType>::FindByHash(uint32 KeyHash, const KeyType& Key)
{
	auto iterator = m_Set.FindByPredicate([KeyHash](const TPair<KeyType, ValueType>& Pair) -> bool
		{
			return Pair.GetKeyHash() == KeyHash;
		});

	return iterator.IsValid() ? &iterator->GetValue() : nullptr;
}

template<typename KeyType, typename ValueType>
inline const ValueType* TMap<KeyType, ValueType>::FindByHash(uint32 KeyHash, const KeyType& Key) const
{
	auto iterator = m_Set.FindByPredicate([KeyHash](const TPair<KeyType, ValueType>& Pair) -> bool
		{
			return Pair.GetKeyHash() == KeyHash;
		});

	return iterator.IsValid() ? &iterator->GetValue() : nullptr;
}
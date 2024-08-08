#pragma once

#include "CoreModule.h"
#include "Definitions.h"

#include "Misc/Hash.h"

#ifndef IE_SET_DOUBLE_CHECK
// If IE_SET_DOUBLE_CHECK is defined, the set will check if the hash of the value is the same as the hash of the node.
#define IE_SET_DOUBLE_CHECK 0
#endif

namespace IE::Private
{
	template<typename ValueType>
	class TSetNode
	{
	public:

		TSetNode(uint32 InHash, ValueType&& InValue)
			: Hash(InHash)
			, Value(MoveTemp(InValue)) {}

		TSetNode(uint32 InHash, const ValueType& InValue)
			: Hash(InHash)
			, Value(InValue) {}

		TSetNode(const TSetNode&) = delete;
		TSetNode& operator=(const TSetNode&) = delete;

		TSetNode(TSetNode&&) = delete;
		TSetNode& operator=(TSetNode&&) = delete;

	public:

		FORCEINLINE bool Compare(uint32 InHash) const
		{
#if IE_SET_DOUBLE_CHECK
			return InHash == Hash && GetTypeHash(Value) == InHash;
#else
			return InHash == Hash;
#endif // IE_SET_DOUBLE_CHECK
		}

		FORCEINLINE void SetNextNode(TSetNode* InNextNode) { NextNode = InNextNode; }
		FORCEINLINE void SetPrevNode(TSetNode* InPrevNode) { PrevNode = InPrevNode; }

		FORCEINLINE TSetNode* GetNextNode() const { return NextNode; }
		FORCEINLINE TSetNode* GetPrevNode() const { return PrevNode; }

		FORCEINLINE uint32 GetHash() const { return Hash; }

		FORCEINLINE ValueType& GetValueMutable() { return Value; }
		FORCEINLINE const ValueType& GetValue() const { return Value; }

	protected:

		uint32 Hash;
		ValueType Value;

		TSetNode* NextNode = nullptr;
		TSetNode* PrevNode = nullptr;
	};
}

template<typename T>
class TSetIterator
{
public:

	using NodeType = IE::Private::TSetNode<T>;

public:

	TSetIterator(NodeType* InNode)
		: Node(InNode) {}

	TSetIterator(const TSetIterator&) = default;
	TSetIterator& operator=(const TSetIterator&) = default;

	TSetIterator(TSetIterator&&) = default;
	TSetIterator& operator=(TSetIterator&&) = default;

public:

	FORCEINLINE TSetIterator& operator++()
	{
		Node = Node->GetNextNode();
		return *this;
	}

	FORCEINLINE TSetIterator operator++(int)
	{
		TSetIterator Temp = *this;
		++(*this);
		return Temp;
	}

	FORCEINLINE TSetIterator& operator--()
	{
		Node = Node->GetPrevNode();
		return *this;
	}

	FORCEINLINE TSetIterator operator--(int)
	{
		TSetIterator Temp = *this;
		--(*this);
		return Temp;
	}

	FORCEINLINE bool IsValid() const { return Node != nullptr; }

	FORCEINLINE bool operator==(const TSetIterator& Other) const { return Node == Other.Node; }
	FORCEINLINE bool operator!=(const TSetIterator& Other) const { return Node != Other.Node; }

	FORCEINLINE T& operator*() const { return Node->GetValueMutable(); }
	FORCEINLINE T* operator->() const { return &Node->GetValueMutable(); }

private:

	NodeType* Node = nullptr;
};

template<typename T>
class TSetConstIterator
{
public:

	using NodeType = IE::Private::TSetNode<T>;

public:

	TSetConstIterator(NodeType* InNode)
		: Node(InNode) {}

	TSetConstIterator(const TSetConstIterator&) = default;
	TSetConstIterator& operator=(const TSetConstIterator&) = default;

	TSetConstIterator(TSetConstIterator&&) = default;
	TSetConstIterator& operator=(TSetConstIterator&&) = default;

public:

	FORCEINLINE TSetConstIterator& operator++()
	{
		Node = Node->GetNextNode();
		return *this;
	}

	FORCEINLINE TSetConstIterator operator++(int)
	{
		TSetConstIterator Temp = *this;
		++(*this);
		return Temp;
	}

	FORCEINLINE TSetConstIterator& operator--()
	{
		Node = Node->GetPrevNode();
		return *this;
	}

	FORCEINLINE TSetConstIterator operator--(int)
	{
		TSetConstIterator Temp = *this;
		--(*this);
		return Temp;
	}

	FORCEINLINE bool IsValid() const { return Node != nullptr; }

	FORCEINLINE bool operator==(const TSetConstIterator& Other) const { return Node == Other.Node; }
	FORCEINLINE bool operator!=(const TSetConstIterator& Other) const { return Node != Other.Node; }

	FORCEINLINE const T& operator*() const { return Node->GetValue(); }
	FORCEINLINE const T* operator->() const { return &Node->GetValue(); }

private:

	NodeType* Node = nullptr;
};

/**
* Sets are containers that store unique elements in no particular order, and which allow for fast retrieval of individual elements based on their value.
* Values are not supposed to be modified once they are in the set.
*/
template<typename T>
class TSet
{

	using NodeType = IE::Private::TSetNode<T>;
	using NumType = int32;

public:

	/**
		* Default constructor.
			*/
	TSet() = default;

	/**
		* Destructor.
			*/
	~TSet()
	{
		Clear();
	}

	/**
		* Copy constructor.
			* @param Other - The other set to copy.
				*/
	TSet(const TSet& Other)
	{
		*this = Other;
	}

	/**
	* Move constructor.
	* @param Other - The other set to move.
	*/
	TSet(TSet&& Other)
	{
		*this = MoveTemp(Other);
	}

	/**
	* Copy assignment operator.
	* @param Other - The other set to copy.
	* @return This set.
	*/
	TSet& operator=(const TSet& Other)
	{
		if (this != &Other)
		{
			Clear();

			for (const auto& Element : Other)
				Add(Element);
		}

		return *this;
	}

	/**
	* Move assignment operator.
	* @param Other - The other set to move.
	* @return This set.
	*/
	TSet& operator=(TSet&& Other)
	{
		if (this != &Other)
		{
			Clear();

			SetNum = Other.SetNum;
			FirstNode = Other.FirstNode;

			Other.SetNum = 0;
			Other.FirstNode = nullptr;
		}

		return *this;
	}

	/**
	* Adds a new element to the set.
	* @param Value - The value to add.
	* @return True if the element was added, false otherwise.
	*/
	bool Add(const T& Value)
	{
		uint32 Hash = GetTypeHash(Value);
		return AddByHash(Value, Hash);
	}

	/**
	* Adds a new element to the set.
	* @param Value - The value to add.
	* @return True if the element was added, false otherwise.
	*/
	bool Add(T&& Value)
	{
		uint32 Hash = GetTypeHash(Value);
		return AddByHash(MoveTemp(Value), Hash);
	}

	/**
	* Adds a new element to the set.
	* @param Value - The value to add.
	* @param Hash - The hash of the value.
	* @return True if the element was added, false otherwise.
	*/
	bool AddByHash(const T& Value, uint32 Hash)
	{
		NodeType* insertNode = nullptr;
		for (NodeType* Node = FirstNode; Node; Node = Node->GetNextNode())
		{
			if (Node->Compare(Hash))
				return false;

			insertNode = Node;
		}

		NodeType* NewNode = AllocateNode(Hash, Value);
		InsertNodeAfter(insertNode, NewNode);

		++SetNum;
		return true;
	}

	/**
	* Adds a new element to the set.
	* @param Value - The value to add.
	* @param Hash - The hash of the value.
	* @return True if the element was added, false otherwise.
	*/
	bool AddByHash(T&& Value, uint32 Hash)
	{
		NodeType* insertNode = nullptr;
		for (NodeType* Node = FirstNode; Node; Node = Node->GetNextNode())
		{
			if (Node->Compare(Hash))
				return false;

			insertNode = Node;
		}

		NodeType* NewNode = AllocateNode(Hash, MoveTemp(Value));
		InsertNodeAfter(insertNode, NewNode);

		++SetNum;
		return true;
	}

	/**
	* Removes an element from the set.
	* @param Value - The value to remove.
	* @return True if the element was removed, false otherwise.
	*/
	bool Remove(const T& Value)
	{
		uint32 Hash = GetTypeHash(Value);
		return RemoveByHash(Hash);
	}

	/**
	* Removes an element from the set.
	* @param Value - The value to remove.
	* @param Hash - The hash of the value.
	* @return True if the element was removed, false otherwise.
	*/
	bool RemoveByHash(uint32 Hash)
	{
		for (NodeType* Node = FirstNode; Node; Node = Node->GetNextNode())
		{
			if (Node->Compare(Hash))
			{
				if (Node->GetPrevNode())
					Node->GetPrevNode()->SetNextNode(Node->GetNextNode());
				else
					FirstNode = Node->GetNextNode();

				if (Node->GetNextNode())
					Node->GetNextNode()->SetPrevNode(Node->GetPrevNode());

				delete Node;

				--SetNum;
				return true;
			}
		}

		return false;
	}

	/**
	* Removes an element from the set.
	* @param Iterator - The iterator to the element to remove.
	* @return True if the element was removed, false otherwise.
	*/
	bool Remove(const TSetIterator<T>& Iterator)
	{
		NodeType* Node = Iterator.Node;
		if (!Node)
			return false;

		if (Node->GetPrevNode())
			Node->GetPrevNode()->SetNextNode(Node->GetNextNode());
		else
			FirstNode = Node->GetNextNode();

		if (Node->GetNextNode())
			Node->GetNextNode()->SetPrevNode(Node->GetPrevNode());

		delete Node;

		--SetNum;
		return true;
	}

	/**
	* Removes an element from the set.
	* @param Iterator - The iterator to the element to remove.
	* @return True if the element was removed, false otherwise.
	*/
	bool Remove(const TSetConstIterator<T>& Iterator)
	{
		NodeType* Node = Iterator.Node;
		if (!Node)
			return false;

		if (Node->GetPrevNode())
			Node->GetPrevNode()->SetNextNode(Node->GetNextNode());
		else
			FirstNode = Node->GetNextNode();

		if (Node->GetNextNode())
			Node->GetNextNode()->SetPrevNode(Node->GetPrevNode());

		delete Node;

		--SetNum;
		return true;
	}

	/**
	* Removes all elements from the set.
	*/
	void Clear()
	{
		NodeType* Node = FirstNode;
		while (Node)
		{
			NodeType* NextNode = Node->GetNextNode();
			delete Node;
			Node = NextNode;
		}

		SetNum = 0;
		FirstNode = nullptr;
	}

	/**
	* Checks if the set contains the specified value.
	* @param Value - The value to check.
	* @return True if the set contains the value, false otherwise.
	*/
	bool Contains(const T& Value) const
	{
		uint32 Hash = GetTypeHash(Value);
		return ContainsByHash(Value, Hash);
	}

	/**
	* Checks if the set contains the specified value.
	* @param Value - The value to check.
	* @param Hash - The hash of the value.
	* @return True if the set contains the value, false otherwise.
	*/
	bool ContainsByHash(const T& Value, uint32 Hash) const
	{
		for (NodeType* Node = FirstNode; Node; Node = Node->GetNextNode())
		{
			if (Node->Compare(Hash, Value))
				return true;
		}

		return false;
	}

	/**
	* Searches for the specified value in the set.
	* @param Value - The value to search for.
	* @return An iterator to the value if it was found, nullptr otherwise.
	*/
	template<typename FindType = T>
	TSetIterator<T> Find(const FindType& Value)
	{
		uint32 Hash = GetTypeHash(Value);
		return FindByHash(Value, Hash);
	}

	/**
	* Searches for the specified value in the set.
	* @param Value - The value to search for.
	* @return An iterator to the value if it was found, nullptr otherwise.
	*/
	template<typename FindType = T>
	TSetConstIterator<T> Find(const FindType& Value) const
	{
		uint32 Hash = GetTypeHash(Value);
		return FindByHash(Value, Hash);
	}

	/**
	* Searches for the specified value in the set.
	* @param Value - The value to search for.
	* @param Hash - The hash of the value.
	* @return An iterator to the value if it was found, nullptr otherwise.
	*/
	TSetIterator<T> FindByHash(const T& Value, uint32 Hash)
	{
		for (NodeType* Node = FirstNode; Node; Node = Node->GetNextNode())
		{
			if (Node->Compare(Hash))
				return TSetIterator<T>(Node);
		}

		return TSetIterator<T>(nullptr);
	}

	/**
	* Searches for the specified value in the set.
	* @param Value - The value to search for.
	* @param Hash - The hash of the value.
	* @return An iterator to the value if it was found, nullptr otherwise.
	*/
	TSetConstIterator<T> FindByHash(const T& Value, uint32 Hash) const
	{
		for (NodeType* Node = FirstNode; Node; Node = Node->GetNextNode())
		{
			if (Node->Compare(Hash))
				return TSetConstIterator<T>(Node);
		}

		return TSetConstIterator<T>(nullptr);
	}

	/**
	* Searches for the specified value in the set.
	* @param Predicate - The predicate to use to search for the value.
	* @return An iterator to the value if it was found, nullptr otherwise.
	*/
	template<typename PredicateType>
	TSetIterator<T> FindByPredicate(PredicateType Predicate)
	{
		for (NodeType* Node = FirstNode; Node; Node = Node->GetNextNode())
		{
			if (Predicate(Node->GetValue()))
				return TSetIterator<T>(Node);
		}

		return TSetIterator<T>(nullptr);
	}

	/**
	* Searches for the specified value in the set.
	* @param Predicate - The predicate to use to search for the value.
	* @return An iterator to the value if it was found, nullptr otherwise.
	*/
	template<typename PredicateType>
	TSetConstIterator<T> FindByPredicate(PredicateType Predicate) const
	{
		for (NodeType* Node = FirstNode; Node; Node = Node->GetNextNode())
		{
			if (Predicate(Node->GetValue()))
				return TSetConstIterator<T>(Node);
		}

		return TSetConstIterator<T>(nullptr);
	}

	/**
	* Gets the number of elements in the set.
	* @return The number of elements in the set.
	*/
	FORCEINLINE NumType Num() const { return SetNum; }

public:

	TSetIterator<T> begin() { return TSetIterator<T>(FirstNode); }
	TSetIterator<T> end() { return TSetIterator<T>(nullptr); }

	TSetConstIterator<T> begin() const { return TSetConstIterator<T>(FirstNode); }
	TSetConstIterator<T> end() const { return TSetConstIterator<T>(nullptr); }

private:

	/**
	* Allocates a new node.
	* @param Hash - The hash of the node.
	* @param Value - The value of the node.
	* @return The new node.
	*/
	NodeType* AllocateNode(uint32 Hash, const T& Value);

	/**
	* Inserts a node after the specified node.
	* @param Node - The node to insert after.
	* @param NewNode - The node to insert.
	*/
	void InsertNodeAfter(NodeType* Node, NodeType* NewNode);

private:

	/** Number of elements in the set. */
	NumType SetNum = 0;

	/** First node of the set . */
	NodeType* FirstNode = nullptr;
};

template<typename T>
inline TSet<T>::NodeType* TSet<T>::AllocateNode(uint32 Hash, const T& Value)
{
	return new NodeType(Hash, Value);
}

template<typename T>
inline void TSet<T>::InsertNodeAfter(NodeType* Node, NodeType* NewNode)
{
	if (!Node)
	{
		// If node is null, insert at the beginning of the list.

		FirstNode = NewNode;
		NewNode->SetPrevNode(nullptr);

		return;
	}

	NewNode->SetNextNode(Node->GetNextNode());
	NewNode->SetPrevNode(Node);

	if (Node->GetNextNode())
		Node->GetNextNode()->SetPrevNode(NewNode);

	Node->SetNextNode(NewNode);
}
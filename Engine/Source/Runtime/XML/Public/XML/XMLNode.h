#pragma once

#include "CoreMinimal.h"
#include "XMLModule.h"

enum EXMLQueryFlags : int32
{
	XQUERY_None = 0,

	XQUERY_CaseInsensitive = 1 << 1,
	XQUERY_AllowWildcards = 1 << 2,

	// Needs to be set if the query is looking for attributes. (If no other attribute flags are set, it will search for attributes with the specified name and value.)
	XQUERY_WithAttributes = 1 << 3,

	// Needs to be set if the query is looking for inner text.
	XQUERY_WithInnerText = 1 << 4,

	// Needs to be set if the query is looking for children (Names).
	XQUERY_WithChildren = 1 << 5,

	// When set, attributes will be searched for with the specified name.
	XQUERY_AttributeName = 1 << 6,

	// When set, attributes will be searched for with the specified value.
	XQUERY_AttributeValue = 1 << 7,
};

struct FXMLAttribute
{
	FString Name;
	FString Value;
};

struct FXMLNodeQuery
{
	int32 Flags;
	FString NodeName;
	FString InnerText;
	FString AttributeName;
	FString AttributeValue;
};

typedef TSharedPtr<FXMLNode, ESPMode::NotThreadSafe> FXMLNodePtr;
typedef TWeakPtr<FXMLNode, ESPMode::NotThreadSafe> FWeakXMLNodePtr;

class XML_API FXMLNode
{
	friend class FXMLDocument;
public:

	/**
	* Gets the name of the node.
	* @return The name of the node.
	*/
	inline FString GetName() const { return Name; }

	/**
	* Gets the inner text of the node.
	* @return The inner text of the node.
	*/
	inline FString GetInner() const { return Inner; }

	/**
	* Gets the parent node of the node.
	* @return The parent node of the node.
	*/
	inline FXMLNodePtr GetParent() const { return Parent.Pin(); }

	/**
	* Gets the number of attributes of the node.
	* @return The number of attributes of the node.
	*/
	inline int32 GetNumAttributes() const { return Attributes.Num(); }

	/**
	* Gets the attribute of the node at the specified index.
	* @param Index The index of the attribute to get.
	* @return The attribute of the node at the specified index.
	*/
	inline const FXMLAttribute& GetAttribute(int32 Index) const { return Attributes[Index]; }

	/**
	* Gets the number of children of the node.
	* @return The number of children of the node.
	*/
	inline int32 GetNumChildren() const { return Children.Num(); }

	/**
	* Gets the child of the node at the specified index.
	* @param Index The index of the child to get.
	* @return The child of the node at the specified index.
	*/
	inline FXMLNodePtr GetChild(int32 Index) const { return Children[Index]; }

	/**
	* Gets the child of the node with the specified name.
	* @param Query The query to use to find the child.
	* @return The child of the node with the specified name.
	*/
	FXMLNodePtr GetChild(const FXMLNodeQuery& Query) const;

	/**
	* Gets the child of the node with the specified name.
	* @param Query The query to use to find the child.
	* @return The child of the node with the specified name.
	*/
	TArray<FXMLNodePtr> GetChildren(const FXMLNodeQuery& Query) const;

private:

	FString Name;
	FString Inner;
	TArray<FXMLAttribute> Attributes;
	TArray<FXMLNodePtr> Children;
	FWeakXMLNodePtr Parent;
};
#pragma once

#include "Definitions.h"
#include "Templates/ImpulseTemplates.h"

template<typename Type>
class TTypeCompatibleBytes
{
public:
	alignas(Type) uint8 Bytes[sizeof(Type)];
};

template<typename Type>
class TTypeCompatibleBytesPtr : public TTypeCompatibleBytes<Type>
{
public:
	Type* GetPtr() { return reinterpret_cast<Type*>(this->Bytes); }
	const Type* GetPtr() const { return reinterpret_cast<const Type*>(this->Bytes); }
};
#pragma once

template<typename Left, typename Right>
struct TAreTypesEqual
{
	enum { Value = false };
};

template<typename Type>
struct TAreTypesEqual<Type, Type>
{
	enum { Value = true };
};

#define ARE_TYPES_EQUAL(Left, Right) TAreTypesEqual<Left, Right>::Value
#pragma once

template <typename T>				struct TIsPointer { enum { Value = false }; };
template <typename T>				struct TIsPointer<T*> { enum { Value = true }; };
template <typename T>				struct TIsPointer<T* const> { enum { Value = true }; };
template <typename T>				struct TIsPointer<T* volatile> { enum { Value = true }; };
template <typename T>				struct TIsPointer<T* volatile const> { enum { Value = true }; };

#define IS_POINTER(Type) TIsPointer<Type>::Value
#define IS_POINTER_VAR(Type) TIsPointer<decltype(Type)>::Value
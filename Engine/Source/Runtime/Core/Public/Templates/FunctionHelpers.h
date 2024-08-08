#pragma once

#include "ImpulseTuple.h"

template<typename FuncType>
struct TFunctionRetValType { typedef void Type; };

template<typename InRetValType, typename... ParamTypes>
struct TFunctionRetValType<InRetValType(*)(ParamTypes...)> { typedef InRetValType Type; };

template<typename InRetValType, typename Class, typename... ParamTypes>
struct TFunctionRetValType<InRetValType(Class::*)(ParamTypes...)> { typedef InRetValType Type; };

template<typename InRetValType, typename Class, typename... ParamTypes>
struct TFunctionRetValType<InRetValType(Class::*)(ParamTypes...) const> { typedef InRetValType Type; };

template<typename InRetValType, typename Class, typename... ParamTypes>
struct TFunctionRetValType<InRetValType(Class::*)(ParamTypes...) volatile> { typedef InRetValType Type; };

template<typename InRetValType, typename Class, typename... ParamTypes>
struct TFunctionRetValType<InRetValType(Class::*)(ParamTypes...) const volatile> { typedef InRetValType Type; };

template<typename FuncType>
struct TFunctionParamTypes { typedef void Type; };

template<typename InRetValType, typename... ParamTypes>
struct TFunctionParamTypes<InRetValType(*)(ParamTypes...)> { typedef TTuple<ParamTypes...> Type; };

template<typename InRetValType, typename Class, typename... ParamTypes>
struct TFunctionParamTypes<InRetValType(Class::*)(ParamTypes...)> { typedef TTuple<ParamTypes...> Type; };

template<typename InRetValType, typename Class, typename... ParamTypes>
struct TFunctionParamTypes<InRetValType(Class::*)(ParamTypes...) const> { typedef TTuple<ParamTypes...> Type; };

template<typename InRetValType, typename Class, typename... ParamTypes>
struct TFunctionParamTypes<InRetValType(Class::*)(ParamTypes...) volatile> { typedef TTuple<ParamTypes...> Type; };

template<typename InRetValType, typename Class, typename... ParamTypes>
struct TFunctionParamTypes<InRetValType(Class::*)(ParamTypes...) const volatile> { typedef TTuple<ParamTypes...> Type; };

template<typename FuncType>
struct TFunctionParamCount { enum { Value = 0 }; };

template<typename InRetValType, typename... ParamTypes>
struct TFunctionParamCount<InRetValType(*)(ParamTypes...)> { enum { Value = sizeof...(ParamTypes) }; };

template<typename InRetValType, typename Class, typename... ParamTypes>
struct TFunctionParamCount<InRetValType(Class::*)(ParamTypes...)> { enum { Value = sizeof...(ParamTypes) }; };

template<typename InRetValType, typename Class, typename... ParamTypes>
struct TFunctionParamCount<InRetValType(Class::*)(ParamTypes...) const> { enum { Value = sizeof...(ParamTypes) }; };

template<typename InRetValType, typename Class, typename... ParamTypes>
struct TFunctionParamCount<InRetValType(Class::*)(ParamTypes...) volatile> { enum { Value = sizeof...(ParamTypes) }; };

template<typename InRetValType, typename Class, typename... ParamTypes>
struct TFunctionParamCount<InRetValType(Class::*)(ParamTypes...) const volatile> { enum { Value = sizeof...(ParamTypes) }; };
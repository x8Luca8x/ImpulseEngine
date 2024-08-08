#pragma once

template<typename T>
struct TIsCharType
{
	enum { Value = false };
};

template<>
struct TIsCharType<char>
{
	enum { Value = true };
};

template<>
struct TIsCharType<wchar_t>
{
	enum { Value = true };
};

template<>
struct TIsCharType<char8_t>
{
	enum { Value = true };
};

template<>
struct TIsCharType<char16_t>
{
	enum { Value = true };
};

template<>
struct TIsCharType<char32_t>
{
	enum { Value = true };
};

template<typename T>
struct TIsCharType<const T>
{
	enum { Value = TIsCharType<T>::Value };
};

template<typename T>
struct TIsCharType<volatile T>
{
	enum { Value = TIsCharType<T>::Value };
};

template<typename T>
struct TIsCharType<const volatile T>
{
	enum { Value = TIsCharType<T>::Value };
};



template<typename T>
struct TIsIntegral
{
	enum { Value = false };
};

template<>
struct TIsIntegral<bool>
{
	enum { Value = true };
};

template<>
struct TIsIntegral<signed char>
{
	enum { Value = true };
};

template<>
struct TIsIntegral<unsigned char>
{
	enum { Value = true };
};

template<>
struct TIsIntegral<short>
{
	enum { Value = true };
};

template<>
struct TIsIntegral<unsigned short>
{
	enum { Value = true };
};

template<>
struct TIsIntegral<int>
{
	enum { Value = true };
};

template<>
struct TIsIntegral<unsigned int>
{
	enum { Value = true };
};

template<>
struct TIsIntegral<long>
{
	enum { Value = true };
};

template<>
struct TIsIntegral<unsigned long>
{
	enum { Value = true };
};

template<>
struct TIsIntegral<long long>
{
	enum { Value = true };
};

template<>
struct TIsIntegral<unsigned long long>
{
	enum { Value = true };
};



template<typename T>
struct TIsFloat
{
	enum { Value = false };
};

template<>
struct TIsFloat<float>
{
	enum { Value = true };
};

template<>
struct TIsFloat<double>
{
	enum { Value = true };
};
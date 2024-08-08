#pragma once

template <class T> struct TRemoveConst { typedef T Type; };
template <class T> struct TRemoveConst<const T> { typedef T Type; };

template <typename T> struct TRemoveCV { typedef T Type; };
template <typename T> struct TRemoveCV<const T> { typedef T Type; };
template <typename T> struct TRemoveCV<volatile T> { typedef T Type; };
template <typename T> struct TRemoveCV<const volatile T> { typedef T Type; };
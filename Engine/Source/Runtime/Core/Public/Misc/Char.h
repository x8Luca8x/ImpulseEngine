#pragma once

#include "Templates/TypeTraits.h"

template<typename T>
class TChar
{

	static_assert(TIsCharType<T>::Value, "T must be a char type.");

public:

	// Determines whether the specified character is a white-space character.
	static constexpr bool IsWhiteSpace(T Char);

	// Determines whether the specified character is a digit.
	static constexpr bool IsDigit(T Char);

	// Determines whether the specified character is a letter.
	static constexpr bool IsLetter(T Char);

	// Determines whether the specified character is a letter or digit.
	static constexpr bool IsLetterOrDigit(T Char);

	// Determines whether the specified character is a lower-case letter.
	static constexpr bool IsLower(T Char);

	// Determines whether the specified character is an upper-case letter.
	static constexpr bool IsUpper(T Char);

	// Converts the specified character to lower-case.
	static constexpr T ToLower(T Char);

	// Converts the specified character to upper-case.
	static constexpr T ToUpper(T Char);
};

template<typename T>
inline constexpr bool TChar<T>::IsWhiteSpace(T Char)
{
	return Char == ' ' || Char == '\t' || Char == '\n' || Char == '\r';
}

template<typename T>
inline constexpr bool TChar<T>::IsDigit(T Char)
{
	return Char >= '0' && Char <= '9';
}

template<typename T>
inline constexpr bool TChar<T>::IsLetter(T Char)
{
	return Char >= 'A' && Char <= 'Z' || Char >= 'a' && Char <= 'z';
}

template<typename T>
inline constexpr bool TChar<T>::IsLetterOrDigit(T Char)
{
	return IsLetter(Char) || IsDigit(Char);
}

template<typename T>
inline constexpr bool TChar<T>::IsLower(T Char)
{
	switch (Char)
	{
	case 'a':
	case 'b':
	case 'c':
	case 'd':
	case 'e':
	case 'f':
	case 'g':
	case 'h':
	case 'i':
	case 'j':
	case 'k':
	case 'l':
	case 'm':
	case 'n':
	case 'o':
	case 'p':
	case 'q':
	case 'r':
	case 's':
	case 't':
	case 'u':
	case 'v':
	case 'w':
	case 'x':
	case 'y':
	case 'z':
		return true;
	default:
		return false;
	}
}

template<typename T>
inline constexpr bool TChar<T>::IsUpper(T Char)
{
	switch (Char)
	{
	case 'A':
	case 'B':
	case 'C':
	case 'D':
	case 'E':
	case 'F':
	case 'G':
	case 'H':
	case 'I':
	case 'J':
	case 'K':
	case 'L':
	case 'M':
	case 'N':
	case 'O':
	case 'P':
	case 'Q':
	case 'R':
	case 'S':
	case 'T':
	case 'U':
	case 'V':
	case 'W':
	case 'X':
	case 'Y':
	case 'Z':
		return true;
	default:
		return false;
	}
}

template<typename T>
inline constexpr T TChar<T>::ToLower(T Char)
{
	switch (Char)
	{
	case 'A':
		return 'a';
	case 'B':
		return 'b';
	case 'C':
		return 'c';
	case 'D':
		return 'd';
	case 'E':
		return 'e';
	case 'F':
		return 'f';
	case 'G':
		return 'g';
	case 'H':
		return 'h';
	case 'I':
		return 'i';
	case 'J':
		return 'j';
	case 'K':
		return 'k';
	case 'L':
		return 'l';
	case 'M':
		return 'm';
	case 'N':
		return 'n';
	case 'O':
		return 'o';
	case 'P':
		return 'p';
	case 'Q':
		return 'q';
	case 'R':
		return 'r';
	case 'S':
		return 's';
	case 'T':
		return 't';
	case 'U':
		return 'u';
	case 'V':
		return 'v';
	case 'W':
		return 'w';
	case 'X':
		return 'x';
	case 'Y':
		return 'y';
	case 'Z':
		return 'z';
	default:
		return Char;
	}
}

template<typename T>
inline constexpr T TChar<T>::ToUpper(T Char)
{
	switch (Char)
	{
	case 'a':
		return 'A';
	case 'b':
		return 'B';
	case 'c':
		return 'C';
	case 'd':
		return 'D';
	case 'e':
		return 'E';
	case 'f':
		return 'F';
	case 'g':
		return 'G';
	case 'h':
		return 'H';
	case 'i':
		return 'I';
	case 'j':
		return 'J';
	case 'k':
		return 'K';
	case 'l':
		return 'L';
	case 'm':
		return 'M';
	case 'n':
		return 'N';
	case 'o':
		return 'O';
	case 'p':
		return 'P';
	case 'q':
		return 'Q';
	case 'r':
		return 'R';
	case 's':
		return 'S';
	case 't':
		return 'T';
	case 'u':
		return 'U';
	case 'v':
		return 'V';
	case 'w':
		return 'W';
	case 'x':
		return 'X';
	case 'y':
		return 'Y';
	case 'z':
		return 'Z';
	default:
		return Char;
	}
}
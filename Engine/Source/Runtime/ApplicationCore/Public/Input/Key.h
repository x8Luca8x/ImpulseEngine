#pragma once

#include "ApplicationCoreModule.h"
#include "Containers/Name.h"

/**
* Keys
*/
enum class EKeys
{
	AnyKey,
	AnyMouseButton,
	LeftMouseButton,
	RightMouseButton,
	MiddleMouseButton,
	ThumbMouseButton1,
	ThumbMouseButton2,
	BackSpace,
	Tab,
	Enter,
	Pause,
	CapsLock,
	Escape,
	SpaceBar,
	PageUp,
	PageDown,
	End,
	Home,
	Left,
	Up,
	Right,
	Down,
	Insert,
	Delete,
	Zero,
	One,
	Two,
	Three,
	Four,
	Five,
	Six,
	Seven,
	Eight,
	Nine,
	A,
	B,
	C,
	D,
	E,
	F,
	G,
	H,
	I,
	J,
	K,
	L,
	M,
	N,
	O,
	P,
	Q,
	R,
	S,
	T,
	U,
	V,
	W,
	X,
	Y,
	Z,
	NumPadZero,
	NumPadOne,
	NumPadTwo,
	NumPadThree,
	NumPadFour,
	NumPadFive,
	NumPadSix,
	NumPadSeven,
	NumPadEight,
	NumPadNine,
	Multiply,
	Add,
	Subtract,
	Decimal,
	Divide,
	F1,
	F2,
	F3,
	F4,
	F5,
	F6,
	F7,
	F8,
	F9,
	F10,
	F11,
	F12,
	NumLock,
	ScrollLock,
	LeftShift,
	RightShift,
	LeftControl,
	RightControl,
	LeftAlt,
	RightAlt,
	LeftCommand,
	RightCommand,
	Semicolon,
	Equals,
	Comma,
	Underscore,
	Period,
	Slash,
	Tilde,
	LeftBracket,
	Backslash,
	RightBracket,
	Quote,
	Unknown,
	Gamepad_LeftX,
	Gamepad_LeftY,
	Gamepad_RightX,
	Gamepad_RightY,
	Gamepad_LeftTriggerAxis,
	Gamepad_RightTriggerAxis,
	Gamepad_DPadUp,
	Gamepad_DPadDown,
	Gamepad_DPadLeft,
	Gamepad_DPadRight,
	Gamepad_LeftThumbstick,
	Gamepad_RightThumbstick,
	Gamepad_Special_Left,
	Gamepad_Special_Right,
	MouseX,
	MouseY,
	MouseScrollUp,
	MouseScrollDown,
	MouseWheelUp,
	MouseWheelDown,
	Touch1,
	Touch2,
	Touch3,
	Touch4,
	Touch5,
	Touch6,
	Touch7,
	Touch8,
	Touch9,
	Touch10,
	Count,
};

struct FKey
{
	FKey() = default;

	FKey(EKeys InKey)
		: KeyEnum(InKey)
	{
	}

	FKey(const TCHAR* InKeyName)
		: KeyName(InKeyName)
	{
	}

	FKey(const FName& InKeyName)
		: KeyName(InKeyName)
	{
	}

	FKey(const FKey& InKey)
		: KeyEnum(InKey.KeyEnum)
		, KeyName(InKey.KeyName)
	{
	}

	FKey& operator=(const FKey& InKey)
	{
		KeyEnum = InKey.KeyEnum;
		KeyName = InKey.KeyName;
		return *this;
	}

	bool operator==(const FKey& InKey) const
	{
		return KeyEnum == InKey.KeyEnum && KeyName == InKey.KeyName;
	}

	bool operator!=(const FKey& InKey) const
	{
		return KeyEnum != InKey.KeyEnum || KeyName != InKey.KeyName;
	}

private:

	FName KeyName;
	EKeys KeyEnum { EKeys::AnyKey };
};
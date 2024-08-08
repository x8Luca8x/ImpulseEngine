#include "Windows/WindowsConsole.h"

#if PLATFORM_WINDOWS

#include <Windows/WindowsAPI.h>

namespace Windows
{
	enum EWinConsolePredefinedColors : WORD
	{
		CONSOLE_BLACK = 0,
		CONSOLE_DARK_BLUE = FOREGROUND_BLUE,
		CONSOLE_DARK_GREEN = FOREGROUND_GREEN,
		CONSOLE_DARK_CYAN = FOREGROUND_GREEN | FOREGROUND_BLUE,
		CONSOLE_DARK_RED = FOREGROUND_RED,
		CONSOLE_DARK_MAGENTA = FOREGROUND_RED | FOREGROUND_BLUE,
		CONSOLE_DARK_YELLOW = FOREGROUND_RED | FOREGROUND_GREEN,
		CONSOLE_GREY = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE,
		CONSOLE_DARK_GREY = FOREGROUND_INTENSITY,
		CONSOLE_BLUE = FOREGROUND_INTENSITY | FOREGROUND_BLUE,
		CONSOLE_GREEN = FOREGROUND_INTENSITY | FOREGROUND_GREEN,
		CONSOLE_CYAN = FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_BLUE,
		CONSOLE_RED = FOREGROUND_INTENSITY | FOREGROUND_RED,
		CONSOLE_MAGENTA = FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_BLUE,
		CONSOLE_YELLOW = FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN,
		CONSOLE_WHITE = FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE
	};

	WORD ConsoleColorToPredifinedColor(EConsoleColor InColor)
	{
		switch (InColor)
		{
		case EConsoleColor::Black:
			return CONSOLE_BLACK;
		case EConsoleColor::DarkBlue:
			return CONSOLE_DARK_BLUE;
		case EConsoleColor::DarkGreen:
			return CONSOLE_DARK_GREEN;
		case EConsoleColor::DarkCyan:
			return CONSOLE_DARK_CYAN;
		case EConsoleColor::DarkRed:
			return CONSOLE_DARK_RED;
		case EConsoleColor::DarkMagenta:
			return CONSOLE_DARK_MAGENTA;
		case EConsoleColor::DarkYellow:
			return CONSOLE_DARK_YELLOW;
		case EConsoleColor::Grey:
			return CONSOLE_GREY;
		case EConsoleColor::DarkGrey:
			return CONSOLE_DARK_GREY;
		case EConsoleColor::Blue:
			return CONSOLE_BLUE;
		case EConsoleColor::Green:
			return CONSOLE_GREEN;
		case EConsoleColor::Cyan:
			return CONSOLE_CYAN;
		case EConsoleColor::Red:
			return CONSOLE_RED;
		case EConsoleColor::Magenta:
			return CONSOLE_MAGENTA;
		case EConsoleColor::Yellow:
			return CONSOLE_YELLOW;
		case EConsoleColor::White:
		default:
			return CONSOLE_WHITE;
		}
	}
}

FWindowsConsole::FWindowsConsole()
	: TextColor(EConsoleColor::White)
{
	ConsoleOutHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	ConsoleInHandle = GetStdHandle(STD_INPUT_HANDLE);
	ConsoleErrHandle = GetStdHandle(STD_ERROR_HANDLE);

	checkf(Windows::IsValidHandle(ConsoleOutHandle), TEXT("Failed to get console output handle"));
	checkf(Windows::IsValidHandle(ConsoleInHandle), TEXT("Failed to get console input handle"));
	checkf(Windows::IsValidHandle(ConsoleErrHandle), TEXT("Failed to get console error handle"));
}

FWindowsConsole::~FWindowsConsole()
{

}

void FWindowsConsole::Print(const FString& Message, const FPrintInfo& PrintInfo)
{
	HANDLE handle = PrintInfo.bError ? ConsoleErrHandle : ConsoleOutHandle;
	WriteConsole(handle, *Message, Message.Length(), nullptr, nullptr);
}

void FWindowsConsole::PrintLine(const FString& Message, const FPrintInfo& PrintInfo)
{
	const FString newMessage = Message.AppendChar('\n');
	Print(newMessage, PrintInfo);
}

void FWindowsConsole::SetTextColor(EConsoleColor Color)
{
	TextColor = Color;

	SetConsoleTextAttribute(ConsoleOutHandle, Windows::ConsoleColorToPredifinedColor(Color));
	SetConsoleTextAttribute(ConsoleErrHandle, Windows::ConsoleColorToPredifinedColor(Color));
}

EConsoleColor FWindowsConsole::GetTextColor() const
{
	return TextColor;
}

#endif
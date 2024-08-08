#pragma once

#include "Platform/PlatformConsole.h"

#if PLATFORM_WINDOWS

class CORE_API FWindowsConsole : public IGenericPlatformConsole
{
public:

	FWindowsConsole();
	~FWindowsConsole();

public:

	/**
	* Prints a message to the console.
	* @param Message - The message to print.
	*/
	virtual void Print(const FString& Message, const FPrintInfo& PrintInfo = FPrintInfo()) override;

	/**
	* Prints a message with a newline to the console.
	* @param Message - The message to print (Can be left blank to print a newline).
	*/
	virtual void PrintLine(const FString& Message, const FPrintInfo& PrintInfo = FPrintInfo()) override;

	/**
	* Sets the text color of the text that will be printed to the console.
	* Applied colors persist until a new color is set.
	* @param Color - The color to set.
	*/
	virtual void SetTextColor(EConsoleColor Color) override;

	/**
	* Gets the current text color of the console.
	* @return The current text color.
	*/
	virtual EConsoleColor GetTextColor() const override;

private:

	/** The current text color of the console. */
	EConsoleColor TextColor;

	/** Output handle of the console. */
	HANDLE ConsoleOutHandle;

	/** Input handle of the console. */
	HANDLE ConsoleInHandle;

	/** Error handle of the console. */
	HANDLE ConsoleErrHandle;
};

typedef FWindowsConsole FPlatformConsole;

#endif
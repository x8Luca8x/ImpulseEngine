#pragma once

#include "Definitions.h"

#include "Containers/Map.h"
#include "Containers/ImpulseString.h"

class IConsole;

#define DECLARE_CONSOLE_VARIABLE_TYPE(Name, DataType) \
class CORE_API FConsoleVariable_##Name : public IConsoleVariable \
{ \
public: \
	FConsoleVariable_##Name(IConsole* InConsole, const FString& InName, const FString& InDescription, DataType##* InValue) \
		: IConsoleVariable(InConsole, InName, InDescription) \
		, Value(InValue) {} \
\
	virtual FString GetValueAsString() const override; \
	virtual void SetValueFromString(const FString& InValue) override; \
\
	FORCEINLINE DataType##* GetValue() const { return Value; } \
\
private: \
	DataType##* Value; \
};

enum class EConsoleColor : uint8
{
	Black,
	DarkBlue,
	DarkGreen,
	DarkCyan,
	DarkRed,
	DarkMagenta,
	DarkYellow,
	Grey,
	DarkGrey,
	Blue,
	Green,
	Cyan,
	Red,
	Magenta,
	Yellow,
	White
};

/**
* Structure containing information about how to print a message to the console.
*/
struct FPrintInfo
{
	/** Whether the message should be written to the error output stream. */
	bool bError = false;
};

/**
* Base class for an executable console command.
*/
class CORE_API IConsoleCommand
{
public:

	IConsoleCommand(IConsole* InConsole, const FString& InName, const FString& InDescription);
	virtual ~IConsoleCommand();

	/**
	* Executes the command.
	* @param Arguments - The arguments to the command (Includes the command name).
	*/
	virtual void Execute(const TArray<FString>& Arguments) = 0;

public:

	FString GetName() const { return Name; }
	FString GetDescription() const { return Description; }
	IConsole* GetConsole() const { return Console; }

protected:

	/** Name which can be used to look up the variable. */
	const FString Name;

	/** Description of the variable. */
	const FString Description;

	/** Console which owns this variable. */
	IConsole* Console;
};

/**
* Console variables can be used to modify the behavior of the engine at runtime.
* Console variables are also like console function. If you type the name of a console variable into the console, it will print the current value.
*/
class CORE_API IConsoleVariable : public IConsoleCommand
{
public:

	IConsoleVariable(IConsole* InConsole, const FString& InName, const FString& InDescription);
	virtual ~IConsoleVariable();

public:

	virtual void Execute(const TArray<FString>& Arguments) override;

public:

	virtual FString GetValueAsString() const = 0;
	virtual void SetValueFromString(const FString& InValue) = 0;
};

DECLARE_CONSOLE_VARIABLE_TYPE(Boolean, bool)
DECLARE_CONSOLE_VARIABLE_TYPE(Integer, int32)
DECLARE_CONSOLE_VARIABLE_TYPE(Integer64, int64)
DECLARE_CONSOLE_VARIABLE_TYPE(Float, float)
DECLARE_CONSOLE_VARIABLE_TYPE(Double, double)
DECLARE_CONSOLE_VARIABLE_TYPE(String, FString)

class CORE_API FConsoleRegistry
{
public:

	/**
	* Registers a new console.
	* @param Console - The console to register.
	*/
	static void RegisterConsole(IConsole* Console);

	/**
	* Unregisters a console.
	* @param Console - The console to unregister.
	*/
	static void UnregisterConsole(IConsole* Console);

	static TArrayIterator<IConsole*> CreateBeginIterator();
	static TArrayIterator<IConsole*> CreateEndIterator();

	static TArrayConstIterator<IConsole*> CreateConstBeginIterator();
	static TArrayConstIterator<IConsole*> CreateConstEndIterator();

private:

	static TArray<IConsole*> s_Consoles;
};

class CORE_API IConsole
{
	friend class IConsoleCommand;
	friend class IConsoleVariable;
public:

	IConsole();
	virtual ~IConsole();

public:

	/**
	* Prints a message to the console.
	* @param Message - The message to print.
	*/
	virtual void Print(const FString& Message, const FPrintInfo& PrintInfo = FPrintInfo()) = 0;

	/**
	* Prints a message with a newline to the console.
	* @param Message - The message to print (Can be left blank to print a newline).
	*/
	virtual void PrintLine(const FString& Message, const FPrintInfo& PrintInfo = FPrintInfo()) = 0;

	/**
	* Sets the text color of the text that will be printed to the console.
	* Applied colors persist until a new color is set.
	* @param Color - The color to set.
	*/
	virtual void SetTextColor(EConsoleColor Color) = 0;

	/**
	* Gets the current text color of the console.
	* @return The current text color.
	*/
	virtual EConsoleColor GetTextColor() const = 0;

public:

	/**
	* Executes a command.
	* @param Command - The command to execute.
	* @return True if the command was executed successfully, otherwise false.
	*/
	virtual bool ExecuteCommand(const FString& Command);

public:

	/**
	* Searches for a command by name.
	* @param Name - The name of the command to search for.
	* @return The command if found, otherwise nullptr.
	*/
	virtual IConsoleCommand* FindCommand(const FString& Name) const;

	/**
	* Searches for commands by name.
	* @param Name - The name of the commands to search for.
	* @param OutCommands - Will contain all commands with the given name.
	* @return The number of commands found.
	*/
	virtual int32 FindCommands(const FString& Name, TArray<IConsoleCommand*>& OutCommands) const;

	/**
	* Searches for a variable by name.
	* @param Name - The name of the variable to search for.
	* @return The variable if found, otherwise nullptr.
	*/
	virtual IConsoleVariable* FindVariable(const FString& Name) const;

	/**
	* Searches for variables by name.
	* @param Name - The name of the variables to search for.
	* @param OutVariables - Will contain all variables with the given name.
	* @return The number of variables found.
	*/
	virtual int32 FindVariables(const FString& Name, TArray<IConsoleVariable*>& OutVariables) const;

protected:

	/**
	* Registers a new console command.
	* @param Variable - The command to register.
	*/
	virtual void RegisterCommand(IConsoleCommand* Command);

	/**
	* Unregisters a console command.
	* @param Variable - The command to unregister.
	*/
	virtual void UnregisterCommand(IConsoleCommand* Command);

	/**
	* Registers a new console variable.
	* @param Variable - The variable to register.
	*/
	virtual void RegisterVariable(IConsoleVariable* Variable);

	/**
	* Unregisters a console variable.
	* @param Variable - The variable to unregister.
	*/
	virtual void UnregisterVariable(IConsoleVariable* Variable);

public:

	/**
	* Gets whether a error should be printed when a command is not found.
	* @return True if a error should be printed, otherwise false.
	*/
	bool ShouldPrintCommandNotFound() const { return bPrintCommandNotFound; }

	/**
	* Sets whether a error should be printed when a command is not found.
	* @param bInPrintCommandNotFound - Whether a error should be printed.
	*/
	void SetPrintCommandNotFound(bool bInPrintCommandNotFound) { bPrintCommandNotFound = bInPrintCommandNotFound; }

private:

	/** Whether a error should be printed when a command is not found. */
	bool bPrintCommandNotFound = true;

	/** List of all registered commands. */
	TMap<FString, IConsoleCommand*> Commands;

	/** List of all registered variables. */
	TMap<FString, IConsoleVariable*> Variables;
};
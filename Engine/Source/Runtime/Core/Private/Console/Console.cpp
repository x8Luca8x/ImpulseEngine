#include "Console/Console.h"

#include "Misc/Parse.h"

TArray<IConsole*> FConsoleRegistry::s_Consoles;

FString FConsoleVariable_Boolean::GetValueAsString() const
{
	return (*Value) ? TEXT("True") : TEXT("False");
}

void FConsoleVariable_Boolean::SetValueFromString(const FString& InValue)
{
	FPlatformString::ToBool(*InValue, InValue.Length(), *Value);
}

FString FConsoleVariable_Integer::GetValueAsString() const
{
	return FString::FromInt32(*Value);
}

void FConsoleVariable_Integer::SetValueFromString(const FString& InValue)
{
	(*Value) = InValue.ToInt32();
}

FString FConsoleVariable_Integer64::GetValueAsString() const
{
	return FString::FromInt64(*Value);
}

void FConsoleVariable_Integer64::SetValueFromString(const FString& InValue)
{
	(*Value) = InValue.ToInt64();
}

FString FConsoleVariable_Float::GetValueAsString() const
{
	return FString::FromFloat(*Value);
}

void FConsoleVariable_Float::SetValueFromString(const FString& InValue)
{
	(*Value) = InValue.ToFloat();
}

FString FConsoleVariable_Double::GetValueAsString() const
{
	return FString::FromDouble(*Value);
}

void FConsoleVariable_Double::SetValueFromString(const FString& InValue)
{
	(*Value) = InValue.ToDouble();
}

FString FConsoleVariable_String::GetValueAsString() const
{
	return *Value;
}

void FConsoleVariable_String::SetValueFromString(const FString& InValue)
{
	(*Value) = InValue;
}

IConsoleCommand::IConsoleCommand(IConsole* InConsole, const FString& InName, const FString& InDescription)
	: Console(InConsole), Name(InName), Description(InDescription)
{
	check(InConsole);
	InConsole->RegisterCommand(this);
}

IConsoleCommand::~IConsoleCommand()
{
	Console->UnregisterCommand(this);
}

IConsoleVariable::IConsoleVariable(IConsole* InConsole, const FString& InName, const FString& InDescription)
	: IConsoleCommand(InConsole, InName, InDescription)
{
	InConsole->RegisterVariable(this);
}

IConsoleVariable::~IConsoleVariable()
{
	Console->UnregisterVariable(this);
}

void IConsoleVariable::Execute(const TArray<FString>& Arguments)
{
	if (Arguments.Num() >= 2)
		SetValueFromString(Arguments[1]);
	else
		Console->PrintLine(FString::Printf(TEXT("%s = %s"), *Name, *GetValueAsString()));
}

IConsole::IConsole()
{
	FConsoleRegistry::RegisterConsole(this);
}

IConsole::~IConsole()
{
	FConsoleRegistry::UnregisterConsole(this);
}

bool IConsole::ExecuteCommand(const FString& Command)
{
	// Every command that is executed is printed to the console

	PrintLine(Command);

	// Find a command with the name of the first token

	TArray<FString> tokens;
	if (!FParse::Tokenize(Command, tokens, false, false, ' '))
		return false;

	if (tokens.Num() == 0)
		return false;

	IConsoleCommand* command = FindCommand(tokens[0]);
	if (!command)
		return false;

	command->Execute(tokens);
	return true;
}

IConsoleCommand* IConsole::FindCommand(const FString& Name) const
{
	IConsoleCommand* const* commandPtr = Commands.Find(Name);
	return commandPtr ? *commandPtr : nullptr;
}

int32 IConsole::FindCommands(const FString& Name, TArray<IConsoleCommand*>& OutCommands) const
{
	OutCommands.Empty();
	for (const auto& Command : Commands)
	{
		if (Command.GetKey().Contains(Name, ESearchCase::IgnoreCase))
			OutCommands.Add(Command.GetValue());
	}

	return OutCommands.Num();
}

IConsoleVariable* IConsole::FindVariable(const FString& Name) const
{
	IConsoleVariable* const* variablePtr = Variables.Find(Name);
	return variablePtr ? *variablePtr : nullptr;
}

int32 IConsole::FindVariables(const FString& Name, TArray<IConsoleVariable*>& OutVariables) const
{
	OutVariables.Empty();
	for (const auto& Variable : Variables)
	{
		if (Variable.GetKey().Contains(Name, ESearchCase::IgnoreCase))
			OutVariables.Add(Variable.GetValue());
	}

	return OutVariables.Num();
}

void IConsole::RegisterCommand(IConsoleCommand* Command)
{
	checkf(Command, TEXT("Command is null"));
	checkf(!Commands.Contains(Command->GetName()), TEXT("Tried to register a command name '%s' thats already registerd!"), *Command->GetName());

	Commands.Add(Command->GetName(), Command);
}

void IConsole::UnregisterCommand(IConsoleCommand* Command)
{
	checkf(Command, TEXT("Command is null"));
	Commands.Remove(Command->GetName());
}

void IConsole::RegisterVariable(IConsoleVariable* Variable)
{
	checkf(Variable, TEXT("Variable is null"));
	checkf(!Variables.Contains(Variable->GetName()), TEXT("Tried to register a variable name '%s' thats already registerd!"), *Variable->GetName());

	Variables.Add(Variable->GetName(), Variable);
}

void IConsole::UnregisterVariable(IConsoleVariable* Variable)
{
	checkf(Variable, TEXT("Variable is null"));
	Variables.Remove(Variable->GetName());
}

void FConsoleRegistry::RegisterConsole(IConsole* Console)
{
	s_Consoles.Add(Console);
}

void FConsoleRegistry::UnregisterConsole(IConsole* Console)
{
	s_Consoles.Remove(Console);
}

TArrayIterator<IConsole*> FConsoleRegistry::CreateBeginIterator()
{
	return s_Consoles.begin();
}

TArrayIterator<IConsole*> FConsoleRegistry::CreateEndIterator()
{
	return s_Consoles.end();
}

TArrayConstIterator<IConsole*> FConsoleRegistry::CreateConstBeginIterator()
{
	return static_cast<const TArray<IConsole*>&>(s_Consoles).begin();
}

TArrayConstIterator<IConsole*> FConsoleRegistry::CreateConstEndIterator()
{
	return static_cast<const TArray<IConsole*>&>(s_Consoles).end();
}
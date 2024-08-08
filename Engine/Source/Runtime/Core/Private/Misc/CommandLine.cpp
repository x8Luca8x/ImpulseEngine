#include "Misc/CommandLine.h"
#include "Misc/StringConverter.h"

#include "Platform/PlatformMisc.h"

#include "Misc/Parse.h"

FString FCommandLine::s_CommandLine;

void FCommandLine::Init(const FString& CommandLine)
{
	s_CommandLine = CommandLine;
}

void FCommandLine::Init(int32 ArgC, char** ArgV)
{
	if (ArgC == 0)
		return;

	FString result;

	{
		auto conv = ANSI_TO_TCHAR(ArgV[0]);
		result = conv.Get();
	}

	for (int32 i = 1; i < ArgC; ++i)
	{
		auto conv = ANSI_TO_TCHAR(ArgV[i]);

		result.InlineAppendChar(TEXT(' '));
		result.InlineAppend(conv.Get());
	}

	Init(result);
}

void FCommandLine::Init()
{
	Init(FPlatformMisc::GetCommandLine());
}

const FString& FCommandLine::Get()
{
	return s_CommandLine;
}

TArray<FString> FCommandLine::GetAsArray()
{
	TArray<FString> tokens;
	FParse::Tokenize(s_CommandLine, tokens, false, false, ' ');

	return tokens;
}
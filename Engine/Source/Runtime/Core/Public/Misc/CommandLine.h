#pragma once

#include "Definitions.h"

#include "Containers/ImpulseString.h"

/**
* Wrapper for the command line.
* To use this class, call FCommandLine::Init() before using any of the other functions.
*/
class CORE_API FCommandLine
{
public:

	/**
	* Initializes the command line.
	*/
	static void Init(const FString& CommandLine);

	/**
	* Initializes the command line.
	*/
	static void Init(int32 ArgC, char** ArgV);

	/**
	* Initializes the command line.
	*/
	static void Init();

	/**
	* Gets the command line.
	*/
	static const FString& Get();

	/**
	* Gets the command line as an array of strings.
	* String elements are separated by spaces.
	*/
	static TArray<FString> GetAsArray();

private:

	static FString s_CommandLine;
};
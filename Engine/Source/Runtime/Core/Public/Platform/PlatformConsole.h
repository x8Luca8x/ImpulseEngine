#pragma once

#include "Console/Console.h"

class CORE_API IGenericPlatformConsole : public IConsole
{
public:
};

#if PLATFORM_WINDOWS
#include "Windows/WindowsConsole.h"
#endif
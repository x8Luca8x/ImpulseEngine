#pragma once

#include "App.h"

// Forward declarations to use in the actual application entry point

/**
* Gets a pointer to the current application.
* This pointer must be instantiated and must not be a stack variable!
*/
IApplication* EntryPointGetApplication();

#if PLATFORM_WINDOWS
#include "Windows/WindowsEntryPoint.h"
#endif
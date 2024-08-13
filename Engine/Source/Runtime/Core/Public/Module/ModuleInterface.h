#pragma once

#include "Definitions.h"

#include "Platform/PlatformMisc.h"

#define IMPLEMENT_MODULE(ModuleInterface) \
	extern "C" DLLEXPORT IModuleInterface* STDCALL GetModule() \
	{ \
		return new ModuleInterface(); \
	}

class CORE_API IModuleInterface
{
public:

	/**
	* Called after the module was created
	* @param ModuleHandle - The handle to the loaded module
	*/
	virtual void OnCreated(FModuleHandle ModuleHandle) {}

	/**
	* Called when the module gets initialized. This is the best place to perform any module specific initialization.
	* If startup fails, call unload module in the module manager.
	*/
	virtual void OnStartup() {}

	/**
	* Called when the module is about to be unloaded. This is the best place to perform any module specific cleanup.
	*/
	virtual void OnShutdown() {}
};
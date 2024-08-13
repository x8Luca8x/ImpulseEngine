#pragma once

#include "ModuleInterface.h"

#include "Containers/Map.h"
#include "Containers/ImpulseString.h"

#include "Platform/PlatformMisc.h"

#include "Misc/ScopeLock.h"

struct FModule
{
	bool bInitialized = false;

	FString Name;
	FModuleHandle Handle;
	IModuleInterface* Module = nullptr;
};

/**
* The module manager is responsible for loading and unloading modules.
*/
class CORE_API FModuleManager
{
public:

	FModuleManager() = default;
	FModuleManager(const FModuleManager&) = delete;
	FModuleManager(FModuleManager&&) = delete;

	FModuleManager& operator=(const FModuleManager&) = delete;
	FModuleManager& operator=(FModuleManager&&) = delete;

	~FModuleManager();

public:

	/**
	* Gets the global module manager.
	* @return The module manager.
	*/
	static FModuleManager& Get();

public:

	/**
	* Loads a module
	* @param ModuleName - Path to the module to load.
	* @return The handle to the loaded module.
	*/
	IModuleInterface* LoadModule(const FString& ModuleName);

	/**
	* Unloads a module.
	*/
	void UnloadModule(FModuleHandle ModuleHandle);

	/**
	* Unloads a module.
	* @param Module - Module to unload.
	*/
	void UnloadModule(IModuleInterface* Module);

	/**
	* Unloads all modules.
	*/
	void UnloadModules();

	/**
	* Frees all garbage collected modules.
	*/
	void FreeGarbage();

public:

	template<typename T>
	T& LoadModuleChecked(const FString& ModuleName)
	{
		IModuleInterface* Module = LoadModule(ModuleName);
		checkf(Module, TEXT("Failed to load module %s"), *ModuleName);

		return *static_cast<T*>(Module);
	}

private:

	/**
	* Gets an already loaded module.
	* @param ModuleName - Path to the module to get.
	* @return The handle to the loaded module.
	*/
	IModuleInterface* GetModule(const FString& ModuleName);

private:

	/** List of module handles that requested to be unloaded while startup was in progress. */
	TArray<FModuleHandle> PendingRemovals;

	/** List of modules that are currently loaded. */
	TMap<FModuleHandle, FModule> Modules;

	/** List of freed module interfaces that are waiting to be garbage collected. */
	TArray<void*> FreedModules;

	/** Lock to protect the module manager. */
	FCriticalSection Lock;
};
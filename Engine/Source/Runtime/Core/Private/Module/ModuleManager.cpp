#include "Module/ModuleManager.h"

FModuleManager::~FModuleManager()
{
	FreeGarbage();
}

FModuleManager& FModuleManager::Get()
{
	static FModuleManager ModuleManager;
	return ModuleManager;
}

FModuleHandle FModuleManager::LoadModule(const FString& ModuleName)
{
	FModuleHandle handle = FPlatformMisc::LoadDllHandle(*ModuleName);
	if (handle)
	{
		TFunction<IModuleInterface* (*)()> GetModule = (IModuleInterface * (*)())FPlatformMisc::GetDllExport(handle, TEXT("GetModule"));

		FModule& mod = Modules.Add(handle);

		mod.Handle = handle;
		mod.Module = GetModule();

		bool bSuccessful = false;

		if (mod.Module)
		{
			mod.Module->OnCreated(handle);
			mod.Module->OnStartup();

			// If the creation or startup process failed, we'll notify that by checking the PendingRemovals array.
			// Every module that requested to be unloaded during startup will be in that array.

			if(PendingRemovals.Remove(handle) == 0)
			{
				mod.bInitialized = true;
				bSuccessful = true;
			}
		}

		if (!bSuccessful)
		{
			FPlatformMisc::FreeDllHandle(handle);

			Modules.Remove(handle);
			handle = INVALID_MODULEHANDLE;

			FreedModules.Add(mod.Module);
		}
	}

	return handle;
}

void FModuleManager::UnloadModule(FModuleHandle ModuleHandle)
{
	FModule* mod = Modules.Find(ModuleHandle);
	if (!mod)
		return;

	if (!mod->bInitialized)
	{
		PendingRemovals.Add(ModuleHandle);
		return;
	}

	mod->Module->OnShutdown();
	mod->Module->~IModuleInterface();

	FreedModules.Add(mod->Module);

	mod->Module = nullptr;
	mod->bInitialized = false;

	FPlatformMisc::FreeDllHandle(ModuleHandle);
	Modules.Remove(ModuleHandle);
}

void FModuleManager::UnloadModule(IModuleInterface* Module)
{
	for (auto& pair : Modules)
	{
		if (pair.GetValue().Module == Module)
		{
			UnloadModule(pair.GetKey());
			break;
		}
	}
}

void FModuleManager::UnloadModules()
{
	TArray<FModuleHandle> handles;
	Modules.GenerateKeyArray(handles);

	for (FModuleHandle handle : handles)
		UnloadModule(handle);
}

void FModuleManager::FreeGarbage()
{
	for (void* module : FreedModules)
		FMemory::Free(module);

	FreedModules.Empty();
}

#pragma once

#include "Application.h"
#include "Memory/NonCopyable.h"

#include "Templates/IsDerivedFrom.h"

/**
* Default command line arguments for an application
*/
struct FAppCommandsArgs final
{

	void Init(const TArray<FString>& InCommandLine);

public:

	/** Whether the application can render (To disable use '-norender' in the command line) */
	bool bCanRender = true;
};

/**
* Class that manages the current program and application
*/
class APPCORE_API FApp final : public FStaticClass
{
public:

	/**
	* Gets the current application
	* @return The current application
	*/
	static FORCEINLINE IApplication* GetApplication() { return s_CurrentApplication; }

public:

	/**
	* Initializes the application system.
	* Must be called before any other application system function!
	*/
	static void Init();

	/**
	* Shuts down the application system.
	*/
	static void Shutdown();

	/**
	* Destroys an application.
	* @param Application - The application to destroy
	*/
	static void DestroyApplication(IApplication* Application);

	/**
	* Makes an application the current application. After initializing the application, it will be run. Most applications will block until the application is closed.
	* @param Application - The application to make the current application
	*/
	static void SetApplication(IApplication* Application);

	/**
	* Removes the current application.
	* @param bDestroy - Whether to destroy the application after removing it
	*/
	static void RemoveApplication(bool bDestroy = true);

public:

	/**
	* Creates a new application.
	* This will not set the application as the current application.
	* @param T - The type of the application to create
	* @return The created application
	*/
	template<typename T>
	static T* CreateApplication(const FAppInfo& Info)
	{
		static_assert(TIsDerivedFrom<T, IApplication>::IsDerivedFrom(), "T must inherit from IApplication");

		T* application = new T(Info);
		application->Create();

		return application;
	}

public:

	/**
	* Checks whether the application can render.
	* @return Whether the application can render
	*/
	static FORCEINLINE bool CanRender() { return s_CommandLineArgs.bCanRender; }

private:

	static void ExitRequested(uint32 ExitCode);

private:

	/** Whether the application system was initialized */
	static volatile bool s_bInitialized;

	/** Delegate handle for request exit delegate */
	static FDelegateHandle s_RequestExitDelegateHandle;

	/** Command line arguments */
	static FAppCommandsArgs s_CommandLineArgs;

	/** The current application */
	static IApplication* s_CurrentApplication;
};

template<typename T>
FORCEINLINE void RunApp(const FAppInfo& Info)
{
	T* application = FApp::CreateApplication<T>(Info);
	FApp::SetApplication(application);

	application->Run();
}
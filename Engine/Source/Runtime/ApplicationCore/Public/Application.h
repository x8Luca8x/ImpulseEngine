#pragma once

#include "ApplicationCoreModule.h"

#define MAKE_VERSION(Major, Minor, Patch) ((Major << 16) | (Minor << 8) | Patch)

#define GET_MAJOR_VERSION(Version) ((Version >> 16) & 0xFF)
#define GET_MINOR_VERSION(Version) ((Version >> 8) & 0xFF)
#define GET_PATCH_VERSION(Version) (Version & 0xFF)

#define DEFAULT_APP_BODY(ClassName) \
public: \
	ClassName(const FAppInfo& Info) \
		: IApplication(Info) {} \

/**
* Information about the current application
*/
struct APPCORE_API FAppInfo
{
	FString Name;
	FString Description;
	uint32 Version;
};

/**
* Base class for an application.
* Every program can only have one application instance.
* Application instances are created by the platform specific application main and can be accessed via FApp::GetApplication().
* Every application class needs DEFAULT_APP_BODY(ClassName) in the body of the class.
*/
class APPCORE_API IApplication
{
public:

	IApplication(const FAppInfo& Info)
		: Info(Info) {}

	/**
	* Called after the application was created
	*/
	virtual void Create() {};

	/**
	* Called before the application is destroyed
	*/
	virtual void Destroy() {};

	/**
	* Called before the application is set as the current application
	*/
	virtual void PreInit() {};

	/**
	* Called after the application was set as the current application
	*/
	virtual void PostInit() {};

	/**
	* Called after PostInit
	*/
	virtual void Run() {};

	/**
	* Called before the application is removed as the current application
	*/
	virtual void PreShutdown() {};

	/**
	* Called after the application was removed as the current application
	*/
	virtual void PostShutdown() {};

protected:

	/** Information about the application */
	FAppInfo Info;
};
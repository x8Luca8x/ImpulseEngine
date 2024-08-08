#pragma once

#include "Platform/PlatformWindow.h"

#if PLATFORM_WINDOWS

#include "Windows/WindowsAPI.h"
#include "Windows/WindowsApplication.h"

struct FWindowsWindowDefinition : public FGenericWindowDefinition
{
	// WINDOWS ONLY!
	int32 nCmdShow;
};

typedef TSharedPtr<class FWindowsWindow, ESPMode::ThreadSafe> FWindowsWindowPtr;
typedef TSharedPtr<FWindowsWindowDefinition, ESPMode::NotThreadSafe> FWindowsWindowDefinitionPtr;

class APPCORE_API FWindowsWindow : public IGenericWindow
{
public:

	/** Default class for impulse windows */
	static const TCHAR* WindowClassName;

	/**
	* Registers the default window class.
	*/
	static void RegisterWindowClass();

public:

	/**
	* Windows procedure for the window.
	*/
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

public:

	/**
	* Initializes the window.
	* @param InDesc - The description of the window.
	* @param InParent - The parent window.
	* @param bShowImmediately - Whether to show the window immediately after initialization. (If you're on windows, use nCmdShow in InDesc instead)
	* @return True if the window was successfully initialized, false otherwise.
	*/
	virtual bool Init(const FWindowDefinitionPtr& InDefinition, const FWindowPtr& InParent, const bool bShowImmediately) override;

	/**
	* Destroys the window.
	*/
	virtual void Destroy() override;

	virtual void ReshapeWindow(int32 X, int32 Y, int32 Width, int32 Height) override;

	virtual bool IsRegularWindow() const override;

	virtual void SetWindowMode(EWindowMode WindowMode) override;
	virtual EWindowMode GetWindowMode() const override;

	virtual bool IsOpen() const override;

private:

	HWND HWnd = NULL;
	EWindowMode WindowMode = EWindowMode::Windowed;

private:

	WINDOWPLACEMENT PreFullscreenWindowPlacement{};
};

inline FWindowPtr MakeWindow()
{
	return MakeShared<FWindowsWindow, ESPMode::ThreadSafe>();
}

inline FWindowDefinitionPtr MakeWindowDefinition()
{
	return MakeShared<FWindowsWindowDefinition, ESPMode::NotThreadSafe>();
}

typedef FWindowsWindow FWindow;
typedef FWindowsWindowDefinition FWindowDefinition;

#endif
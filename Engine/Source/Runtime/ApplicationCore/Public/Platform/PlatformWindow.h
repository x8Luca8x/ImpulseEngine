#pragma once

#include "ApplicationCoreModule.h"

/** Enumeration to specify different window types for windowa */
enum class EWindowType
{
	/** Value indicating that this is a standard, general-purpose window */
	Normal,
	/** Value indicating that this is a window used for a popup menu */
	Menu,
	/** Value indicating that this is a window used for a tooltip */
	ToolTip,
	/** Value indicating that this is a window used for a notification toast */
	Notification,
	/** Value indicating that this is a window used for a cursor decorator */
	CursorDecorator,
	/** Value indicating that this is a game window */
	GameWindow
};

enum class EWindowMode
{
	/** The window is in true fullscreen mode */
	Fullscreen,
	/** The window has no border and takes up the entire area of the screen */
	WindowedFullscreen,
	/** The window has a border and may not take up the entire screen area */
	Windowed,
};

struct FGenericWindowDefinition
{
	EWindowType WindowType;

	/** The initial position of the window */
	float XDesiredPosition;

	/** The initial position of the window */
	float YDesiredPosition;

	/** The initial size of the window */
	float WidthDesiredSize;

	/** The initial size of the window */
	float HeightDesiredSize;

	/** The minimum size of the window */
	float MinWidth;

	/** The minimum size of the window */
	float MinHeight;

	/** The maximum size of the window */
	float MaxWidth;

	/** The maximum size of the window */
	float MaxHeight;

	/** true if the window is using the os window border instead of a slate created one */
	bool bHasOSWindowBorder;
	/** should this window show up in the taskbar */
	bool bAppearsInTaskbar;
	/** true if the window should be on top of all other windows; false otherwise */
	bool bIsTopmostWindow;
	/** true if the window accepts input; false if the window is non-interactive */
	bool bAcceptsInput;

	/** true if this window will be focused when it is first shown */
	bool bFocusWhenFirstShown;
	/** true if this window displays an enabled close button on the toolbar area */
	bool bHasCloseButton;
	/** true if this window displays an enabled minimize button on the toolbar area */
	bool bSupportsMinimize;
	/** true if this window displays an enabled maximize button on the toolbar area */
	bool bSupportsMaximize;
	/** true if this is a user-sized window with a thick edge */
	bool bHasSizingFrame;
	/** true if this is a regular windows */
	bool bIsRegularWindow;

	/** the title of the window */
	FString Title;
	/** opacity of the window (0-1) */
	float Opacity;
	/** the radius of the corner rounding of the window */
	int32 CornerRadius;
};

class IGenericWindow;

typedef IGenericWindow FWindowBase;
typedef TSharedPtr<class IGenericWindow, ESPMode::ThreadSafe> FWindowPtr;
typedef TSharedPtr<FGenericWindowDefinition, ESPMode::NotThreadSafe> FWindowDefinitionPtr;

typedef TMulticastDelegate<void(FWindowBase*)> FWindowEventDelegate;

/**
* Base class for platform specific windows.
*/
class APPCORE_API IGenericWindow : public TSharedFromThis<IGenericWindow, ESPMode::ThreadSafe>
{
public:

	/**
	* Initializes the window.
	* @param InDesc - The description of the window.
	* @param InParent - The parent window.
	* @param bShowImmediately - Whether to show the window immediately after initialization. (If you're on windows, use nCmdShow in InDesc instead)
	* @return True if the window was successfully initialized, false otherwise.
	*/
	virtual bool Init(const FWindowDefinitionPtr& InDefinition, const FWindowPtr& InParent, const bool bShowImmediately) = 0;

	/**
	* Destroys the window.
	*/
	virtual void Destroy() = 0;

	virtual void ReshapeWindow(int32 X, int32 Y, int32 Width, int32 Height) = 0;

	virtual bool IsRegularWindow() const = 0;

	virtual void SetWindowMode(EWindowMode WindowMode) = 0;
	virtual EWindowMode GetWindowMode() const = 0;

	/**
	* Checks if the window is open.
	* @return True if the window is open, false otherwise.
	*/
	virtual bool IsOpen() const = 0;

protected:

	/** Definition of the window */
	FWindowDefinitionPtr WindowDefinition;

	/** Parent window. TODO: Switch to Weak Ptr! */
	FWindowPtr ParentWindow;

public:

	/** Event delegate that is called when the window is destroyed */
	FWindowEventDelegate OnDestroyed;

	/** Event delegate that is called when the window is closed */
	FWindowEventDelegate OnClosed;

protected:

	/**
	* Called by the wndproc when the window is being destroyed.
	*/
	virtual void OnWindowDestroyed() { /*OnDestroyed.Broadcast(this);*/ }

	/**
	* Called by the wndproc when the window is closed.
	*/
	virtual void OnWindowClosed() { /*OnClosed.Broadcast(this);*/ }
};
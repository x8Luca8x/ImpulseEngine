#include "Windows/WindowsWindow.h"

#if PLATFORM_WINDOWS

#include <Windows/WindowsAPI.h>

const TCHAR* FWindowsWindow::WindowClassName = TEXT("MinimalWindowsWindow");

void FWindowsWindow::RegisterWindowClass()
{
	WNDCLASSEXW wndclass{};

	wndclass.cbSize = sizeof(WNDCLASSEXW);
	wndclass.style = CS_DBLCLKS;
	wndclass.lpfnWndProc = FWindowsWindow::WndProc;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = GetModuleHandleW(nullptr);
	wndclass.hIcon = nullptr;
	wndclass.hCursor = LoadCursorW(nullptr, IDC_ARROW);
	wndclass.hbrBackground = static_cast<HBRUSH>(GetStockObject(BLACK_BRUSH));
	wndclass.lpszMenuName = nullptr;
	wndclass.lpszClassName = FWindowsWindow::WindowClassName;

	checkf(RegisterClassExW(&wndclass), TEXT("Failed to register window class"));
}

LRESULT FWindowsWindow::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	FWindowsWindow* window = (FWindowsWindow*)GetWindowLongPtr(hWnd, GWLP_USERDATA); // NOTE: Only valid after CreateWindowEx

	switch (message)
	{
	case WM_CLOSE:
	{
		if (window)
			window->OnWindowClosed();

		break;
	}

	case WM_DESTROY:
	{
		if (window)
			window->OnWindowDestroyed();

		break;
	}
	}

	return DefWindowProcW(hWnd, message, wParam, lParam);
}

bool FWindowsWindow::Init(const FWindowDefinitionPtr& InDefinition, const FWindowPtr& InParent, const bool bShowImmediately)
{
	ParentWindow = InParent;
	WindowDefinition = InDefinition;

	uint32 windowExStyle = 0;
	uint32 windowStyle = 0;

	const float xInitialRect = InDefinition->XDesiredPosition;
	const float yInitialRect = InDefinition->YDesiredPosition;

	const float widthInitial = InDefinition->WidthDesiredSize;
	const float heightInitial = InDefinition->HeightDesiredSize;

	int32 clientX = FMath::TruncToInt(xInitialRect);
	int32 clientY = FMath::TruncToInt(yInitialRect);
	int32 clientWidth = FMath::TruncToInt(widthInitial);
	int32 clientHeight = FMath::TruncToInt(heightInitial);
	int32 windowX = clientX;
	int32 windowY = clientY;
	int32 windowWidth = clientWidth;
	int32 windowHeight = clientHeight;

	if (!InDefinition->bHasOSWindowBorder)
	{
		windowExStyle = WS_EX_WINDOWEDGE;
		windowStyle = WS_POPUP | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;

		if (InDefinition->bAppearsInTaskbar)
			windowExStyle |= WS_EX_APPWINDOW;
		else
			windowExStyle |= WS_EX_TOOLWINDOW;

		if (InDefinition->bIsTopmostWindow)
			// Tool tips are always top most windows
			windowExStyle |= WS_EX_TOPMOST;

		if (!InDefinition->bAcceptsInput)
			// Window should never get input
			windowExStyle |= WS_EX_TRANSPARENT;
	}
	else
	{
		// OS Window border setup
		windowExStyle = WS_EX_APPWINDOW;
		windowStyle = WS_OVERLAPPED | WS_SYSMENU | WS_CAPTION;

		if (IsRegularWindow())
		{
			if (InDefinition->bSupportsMaximize)
				windowStyle |= WS_MAXIMIZEBOX;

			if (InDefinition->bSupportsMinimize)
				windowStyle |= WS_MINIMIZEBOX;

			if (InDefinition->bHasSizingFrame)
				windowStyle |= WS_THICKFRAME;
			else
				windowStyle |= WS_BORDER;
		}
		else
			windowStyle |= WS_POPUP | WS_BORDER;

		// X,Y, Width, Height defines the top-left pixel of the client area on the screen
		// This adjusts a zero rect to give us the size of the border
		RECT BorderRect = { 0, 0, 0, 0 };
		AdjustWindowRectEx(&BorderRect, windowStyle, false, windowExStyle);

		// Border rect size is negative - see MoveWindowTo
		windowX += BorderRect.left;
		windowY += BorderRect.top;

		// Inflate the window size by the OS border
		windowWidth += BorderRect.right - BorderRect.left;
		windowHeight += BorderRect.bottom - BorderRect.top;
	}

	HWnd = CreateWindowExW(
		windowExStyle,
		WindowClassName,
		*WindowDefinition->Title,
		windowStyle,
		windowX,
		windowY,
		windowWidth,
		windowHeight,
		nullptr,
		nullptr,
		GetModuleHandleW(nullptr),
		nullptr);

	if (HWnd == nullptr)
		return false;

	// Set the window data to this object

	SetWindowLongPtr(HWnd, GWLP_USERDATA, (LONG_PTR)this);

	// Set the icon

	HICON HIcon = (HICON)GetClassLongW(HWnd, GCLP_HICON);

	if (HIcon != nullptr)
		SendMessageW(HWnd, WM_SETICON, ICON_SMALL, (LPARAM)HIcon);

	// Show the window

	if (bShowImmediately)
		ShowWindow(HWnd, SW_SHOW);

	return true;
}

void FWindowsWindow::Destroy()
{
	if (HWnd)
	{
		DestroyWindow(HWnd);
		HWnd = NULL;
	}
}

void FWindowsWindow::ReshapeWindow(int32 X, int32 Y, int32 Width, int32 Height)
{
	if (HWnd)
	{
		SetWindowPos(HWnd, nullptr, X, Y, Width, Height, SWP_NOZORDER | SWP_NOACTIVATE);
	}

}

bool FWindowsWindow::IsRegularWindow() const
{
	return WindowDefinition->bIsRegularWindow;
}

void FWindowsWindow::SetWindowMode(EWindowMode NewWindowMode)
{
	if (NewWindowMode != WindowMode)
	{
		EWindowMode PreviousWindowMode = WindowMode;
		WindowMode = NewWindowMode;

		const bool bTrueFullscreen = NewWindowMode == EWindowMode::Fullscreen;

		// Setup Win32 Flags to be used for Fullscreen mode
		LONG WindowStyle = GetWindowLongW(HWnd, GWL_STYLE);
		const LONG FullscreenModeStyle = WS_POPUP;

		LONG WindowedModeStyle = WS_OVERLAPPED | WS_SYSMENU | WS_CAPTION;
		if (IsRegularWindow())
		{
			if (WindowDefinition->bSupportsMaximize)
				WindowedModeStyle |= WS_MAXIMIZEBOX;

			if (WindowDefinition->bSupportsMinimize)
				WindowedModeStyle |= WS_MINIMIZEBOX;

			if (WindowDefinition->bHasSizingFrame)
				WindowedModeStyle |= WS_THICKFRAME;
			else
				WindowedModeStyle |= WS_BORDER;
		}
		else
			WindowedModeStyle |= WS_POPUP | WS_BORDER;

		// If we're not in fullscreen, make it so
		if (NewWindowMode == EWindowMode::WindowedFullscreen || NewWindowMode == EWindowMode::Fullscreen)
		{
			if (PreviousWindowMode == EWindowMode::Windowed)
			{
				PreFullscreenWindowPlacement.length = sizeof(WINDOWPLACEMENT);
				GetWindowPlacement(HWnd, &PreFullscreenWindowPlacement);
			}

			// Setup Win32 flags for fullscreen window
			WindowStyle &= ~WindowedModeStyle;
			WindowStyle |= FullscreenModeStyle;

			SetWindowLongW(HWnd, GWL_STYLE, WindowStyle);
			SetWindowPos(HWnd, nullptr, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);

			if (!bTrueFullscreen)
			{
				// Ensure the window is restored if we are going for WindowedFullscreen
				ShowWindow(HWnd, SW_RESTORE);
			}

			// Get the current window position.
			RECT ClientRect;
			GetClientRect(HWnd, &ClientRect);

			// Grab current monitor data for sizing
			HMONITOR Monitor = MonitorFromWindow(HWnd, bTrueFullscreen ? MONITOR_DEFAULTTOPRIMARY : MONITOR_DEFAULTTONEAREST);
			MONITORINFO MonitorInfo;
			MonitorInfo.cbSize = sizeof(MONITORINFO);
			GetMonitorInfoW(Monitor, &MonitorInfo);

			// Get the target client width to send to ReshapeWindow.
			// Preserve the current res if going to true fullscreen and the monitor supports it and allow the calling code
			// to resize if required.
			// Else, use the monitor's res for windowed fullscreen.
			LONG MonitorWidth = MonitorInfo.rcMonitor.right - MonitorInfo.rcMonitor.left;
			LONG TargetClientWidth = bTrueFullscreen ?
				FMath::Min(MonitorWidth, ClientRect.right - ClientRect.left) :
				MonitorWidth;

			LONG MonitorHeight = MonitorInfo.rcMonitor.bottom - MonitorInfo.rcMonitor.top;
			LONG TargetClientHeight = bTrueFullscreen ?
				FMath::Min(MonitorHeight, ClientRect.bottom - ClientRect.top) :
				MonitorHeight;


			// Resize and position fullscreen window
			ReshapeWindow(
				MonitorInfo.rcMonitor.left,
				MonitorInfo.rcMonitor.top,
				TargetClientWidth,
				TargetClientHeight);
		}
		else
		{
			// Windowed:

			// Setup Win32 flags for restored window
			WindowStyle &= ~FullscreenModeStyle;
			WindowStyle |= WindowedModeStyle;
			SetWindowLongW(HWnd, GWL_STYLE, WindowStyle);
			SetWindowPos(HWnd, nullptr, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);

			if (PreFullscreenWindowPlacement.length) // Was PreFullscreenWindowPlacement initialized?
			{
				SetWindowPlacement(HWnd, &PreFullscreenWindowPlacement);
			}

			// Set the icon back again as it seems to get ignored if the application has ever started in full screen mode
			HICON HIcon = (HICON)GetClassLongW(HWnd, GCLP_HICON);
			if (HIcon != nullptr)
			{
				SendMessageW(HWnd, WM_SETICON, ICON_SMALL, (LPARAM)HIcon);
			}
		}
	}
}

EWindowMode FWindowsWindow::GetWindowMode() const
{
	return WindowMode;
}

bool FWindowsWindow::IsOpen() const
{
	return IsWindow(HWnd) != 0;
}

#endif
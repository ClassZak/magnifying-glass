#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
#include "pch.hpp"
#include "CheckIsRunning.hpp"
#include "Global.hpp"
#include "CheckPolicy.hpp"
#include "ProgramWindow.hpp"
#include "ZoomContext.hpp"

HHOOK hMouseHook;
HINSTANCE hInst;
HWND globalHwnd = NULL;

#define WINDOW_CLASSNAME "magnifying-glass"
#define WINDOW_NAME WINDOW_CLASSNAME

void InitZoomContext()
{
	ZoomContext& zoomContext = Global::getInstance().getZoomContext();
	zoomContext.screenW = GetSystemMetrics(SM_CXSCREEN);
	zoomContext.screenH = GetSystemMetrics(SM_CYSCREEN);
	
	zoomContext.zoomRect.left = zoomContext.zoomRect.top = 0;
	zoomContext.zoomRect.right = zoomContext.screenW;
	zoomContext.zoomRect.bottom = zoomContext.screenH;
}

ATOM RegisterWindowClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex = { 0 };

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = ProgramWindowWnd::ProgramWindowWndProc;
	wcex.hInstance = hInstance;
	wcex.hCursor = LoadCursor(0, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(1 + COLOR_BTNFACE);
	wcex.lpszClassName = WINDOW_CLASSNAME;

	return RegisterClassEx(&wcex);
}

int APIENTRY wWinMain(	_In_ HINSTANCE hInstance,
						_In_opt_ HINSTANCE hPrevInstance,
						_In_ LPWSTR lpCmdLine,
						_In_ int nCmdShow)
{
	ZoomContext& zoomContext = Global::getInstance().getZoomContext();
	hInst = hInstance;
	if (CheckIsRunning())
		return 1;
	
	SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);
	InitZoomContext();
	// Magnifer API initialization
	if(!MagInitialize())
		return 1;

	MSG  msg;
	ATOM aRegistrationResult = RegisterWindowClass(hInstance);
	globalHwnd = CreateWindowEx
	(
		WS_EX_LAYERED | // Required style to render the magnification correctly
		WS_EX_TOPMOST | // Always-on-top
		WS_EX_TRANSPARENT, // Click-through
		WINDOW_CLASSNAME,
		WINDOW_NAME,
		WS_CLIPCHILDREN | // For no drawing inside main window
		WS_POPUP | // Removes titlebar and borders - simply a bare window
		WS_BORDER, // Adds a 1-pixel border for tracking the edges - aesthetic
		0,0,
		zoomContext.screenW, zoomContext.screenH,
		NULL, NULL, hInstance, NULL
	);

	if (globalHwnd == NULL)
	{
		std::string message = std::format("Error code: {}", GetLastError());
		MessageBox(NULL, message.c_str(), "Error", MB_ICONERROR | MB_OK);
		MagUninitialize();
		return 1;
	}

	// Make the window fully opaque.
	SetLayeredWindowAttributes(globalHwnd, 0, 255, LWA_ALPHA);
	ShowWindow(globalHwnd, nCmdShow);
	UpdateWindow(globalHwnd);
	

	while (GetMessage(&msg, NULL, 0, 0)) 
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

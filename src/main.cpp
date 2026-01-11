#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
#include "pch.hpp"
#include "CheckIsRunning.hpp"
#include "Global.hpp"
#include "CheckPolicy.hpp"
#include "MenuWindow.hpp"
#include "RenderWindow.hpp"
#include "ZoomContext.hpp"

HWND hGlobalRenderWindow = NULL;
HWND hGlobalMenuWindow = NULL;


#define RENDER_WINDOW_CLASSNAME "render-window"
#define RENDER_WINDOW_NAME RENDER_WINDOW_CLASSNAME
#define MENU_WINDOW_CLASSNAME "menu-window"
#define MENU_WINDOW_NAME MENU_WINDOW_CLASSNAME


void InitZoomContext()
{
	ZoomContext& zoomContext = Global::getInstance().getZoomContext();

	zoomContext.screenDC = GetDC(NULL);
	zoomContext.memDC = CreateCompatibleDC(zoomContext.screenDC);

	zoomContext.screenW = GetSystemMetrics(SM_CXSCREEN);
	zoomContext.screenH = GetSystemMetrics(SM_CYSCREEN);

	zoomContext.srcBitmap = 
	CreateCompatibleBitmap
	(
		zoomContext.screenDC,
		zoomContext.screenW,
		zoomContext.screenH
	);

	zoomContext.oldBitmap = (HBITMAP)SelectObject(zoomContext.memDC, zoomContext.srcBitmap);

	zoomContext.zoom = 1;
	zoomContext.isRunning = TRUE;
}


static inline void ShowError(const std::string& errorMessage)
{
	MessageBox(NULL, errorMessage.c_str(), "Error", MB_OK | MB_ICONERROR);
}
static inline ATOM RegisterWindowClass(HINSTANCE hInstance, WNDPROC wndProc, LPCSTR lpClassName)
{
	WNDCLASSEX wcex = { 0 };

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = wndProc;
	wcex.hInstance = hInstance;
	wcex.hCursor = LoadCursor(0, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(1 + COLOR_BTNFACE);
	wcex.lpszClassName = lpClassName;

	return RegisterClassEx(&wcex);
}

DWORD RenderThreadId;
DWORD RenderThread(LPVOID lpThreadParameter)
{
	ZoomContext* zoomContext = static_cast<ZoomContext*>(lpThreadParameter);
	if (zoomContext == nullptr)
	{
		{
			std::string errorMessage = std::format(R"(zoom context is not defined. Send issue for repozitory
File: {}
Function: {}
Line: {})", __FILE__, __FUNCTION__, __LINE__);
			MessageBox(NULL, errorMessage.c_str(), "Error", MB_OK | MB_ICONERROR);
		}
		exit(EXIT_FAILURE);
	}

	HDC windowDC = GetDC(hGlobalRenderWindow);

	timeBeginPeriod(1);
	while (zoomContext->isRunning)
	{
		DWORD startTime = timeGetTime();

		SetLayeredWindowAttributes(hGlobalRenderWindow, 0, 0, LWA_ALPHA);
		BitBlt
		(
			zoomContext->memDC,
			0, 0, zoomContext->screenW, zoomContext->screenH,
			zoomContext->screenDC,
			0, 0,
			SRCCOPY
		);
		SetLayeredWindowAttributes(hGlobalRenderWindow, 0, 255, LWA_ALPHA);

		RECT rc;
		GetClientRect(hGlobalRenderWindow, &rc);
		long& targetW = rc.right, targetH = rc.bottom;
		LONG zoom = zoomContext->zoom;


		int srcW = targetW / zoom;
		int srcH = targetH / zoom;

		int srcX = (zoomContext->screenW / 2) - (srcW / 2);
		int srcY = (zoomContext->screenH / 2) - (srcH / 2);

		SetStretchBltMode(windowDC, COLORONCOLOR); // Without blurring

		StretchBlt
		(
			windowDC,
			0, 0,
			targetW, targetH,
			zoomContext->memDC,
			srcX, srcY,
			srcW, srcH,
			SRCCOPY
		);
		DWORD elapsedTime = timeGetTime() - startTime;
		Sleep(1000);
	}
	timeEndPeriod(1);


	ReleaseDC(hGlobalRenderWindow, windowDC);

	return EXIT_SUCCESS;
}

int APIENTRY wWinMain(	_In_ HINSTANCE hInstance,
						_In_opt_ HINSTANCE hPrevInstance,
						_In_ LPWSTR lpCmdLine,
						_In_ int nCmdShow)
{
	ZoomContext& zoomContext = Global::getInstance().getZoomContext();
	if (CheckIsRunning())
		return EXIT_FAILURE;
	
	SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);
	InitZoomContext();

	if (!RegisterWindowClass(hInstance, RenderWindow::WindowProc, RENDER_WINDOW_CLASSNAME))
	{
		ShowError(std::format(R"(Window class registration error {}
File: {}
Function: {}
Line: {})", GetLastError(), __FILE__, __FUNCTION__, __LINE__));

		return EXIT_FAILURE;
	}
	if (!RegisterWindowClass(hInstance, MenuWindow::WindowProc, MENU_WINDOW_CLASSNAME))
	{
		ShowError(std::format(R"(Window class registration error {}
File: {}
Function: {}
Line: {})", GetLastError(), __FILE__, __FUNCTION__, __LINE__));

		return EXIT_FAILURE;
	}




	hGlobalRenderWindow = CreateWindowEx
	(
		WS_EX_LAYERED | // Required style to render the magnification correctly
		WS_EX_TOPMOST | // Always-on-top
		WS_EX_TRANSPARENT, // Click-through
		RENDER_WINDOW_CLASSNAME,
		RENDER_WINDOW_NAME,
		WS_CLIPCHILDREN | // For no drawing inside main window
		WS_POPUP, // Removes titlebar and borders - simply a bare window
		0,0,
		zoomContext.screenW, zoomContext.screenH,
		NULL, NULL, hInstance, NULL
	);
	if (!hGlobalRenderWindow)
	{
		ShowError(std::format(R"(Window creating error {}
File: {}
Function: {}
Line: {})", GetLastError(), __FILE__, __FUNCTION__, __LINE__));
		
		return EXIT_FAILURE;
	}
	// Make the window fully opaque.
	SetLayeredWindowAttributes(hGlobalRenderWindow, 0, 0, LWA_ALPHA);
	ShowWindow(hGlobalRenderWindow, nCmdShow);
	UpdateWindow(hGlobalRenderWindow);


	{
		// For clear scopes of visibility
		int wX = int(zoomContext.screenW * 0.2), wY = 0, wWidth = int(zoomContext.screenW * 0.6)  /*80% of srceen width*/, wHeight = 96;
		hGlobalMenuWindow = CreateWindowEx
		(
			WS_EX_TOPMOST, // Always-on-top
			MENU_WINDOW_CLASSNAME,
			MENU_WINDOW_NAME,
			WS_OVERLAPPED | // Base style
			WS_CAPTION | // Window caption
			WS_SYSMENU, // System memu. Close button only
			wX, wY,
			wWidth, wHeight,
			NULL, NULL, hInstance, NULL
		);
	}
	if (!hGlobalMenuWindow)
	{
		ShowError(std::format(R"(Window creating error {}
File: {}
Function: {}
Line: {})", GetLastError(), __FILE__, __FUNCTION__, __LINE__));

		return EXIT_FAILURE;
	}
	ShowWindow(hGlobalMenuWindow, nCmdShow);







	// Create render thread
	RenderThreadId = 0;
	HANDLE hThread = CreateThread(NULL, 0x1000, RenderThread, &zoomContext, 0, &RenderThreadId);
	

	MSG  msg;
	while (GetMessage(&msg, NULL, 0, 0)) 
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return EXIT_SUCCESS;
}

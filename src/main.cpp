#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
#include "pch.hpp"
#include "CheckIsRunning.hpp"
#include "Global.hpp"
#include "CheckPolicy.hpp"
#include "ProgramWindow.hpp"
#include "ZoomContext.hpp"

HWND globalHwnd = NULL;

#define WINDOW_CLASSNAME "magnifying-glass"
#define WINDOW_NAME WINDOW_CLASSNAME

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

	HDC windowDC = GetDC(globalHwnd);

	timeBeginPeriod(1);
	while (zoomContext->isRunning)
	{
		DWORD startTime = timeGetTime();

		BitBlt
		(
			zoomContext->memDC,
			0, 0, zoomContext->screenW, zoomContext->screenH,
			zoomContext->screenDC,
			0, 0,
			SRCCOPY
		);

		RECT rc;
		GetClientRect(globalHwnd, &rc);
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
		if (elapsedTime < 16)
			Sleep(16 - elapsedTime);
	}
	timeEndPeriod(1);


	ReleaseDC(globalHwnd, windowDC);

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
		WS_POPUP, // Removes titlebar and borders - simply a bare window
		0,0,
		zoomContext.screenW, zoomContext.screenH,
		NULL, NULL, hInstance, NULL
	);

	if (globalHwnd == NULL)
	{
		{
			std::string errorMessage = std::format(R"(Window creating error {}
File: {}
Function: {}
Line: {})", GetLastError(), __FILE__, __FUNCTION__, __LINE__);
			MessageBox(NULL, errorMessage.c_str(), "Error", MB_OK | MB_ICONERROR);
		}
		return EXIT_FAILURE;
	}

	// Make the window fully opaque.
	SetLayeredWindowAttributes(globalHwnd, 0, 120, LWA_ALPHA);
	ShowWindow(globalHwnd, nCmdShow);
	UpdateWindow(globalHwnd);


	// Create render thread
	RenderThreadId = 0;
	HANDLE hThread = CreateThread(NULL, 0x1000, RenderThread, &zoomContext, 0, &RenderThreadId);
	

	while (GetMessage(&msg, NULL, 0, 0)) 
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return EXIT_SUCCESS;
}

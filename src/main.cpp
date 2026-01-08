#include "pch.hpp"
#include "CheckIsRunning.hpp"
#include "Global.hpp"
#include "CheckPolicy.hpp"
#include "ProgramWindow.hpp"
#include "ZoomContext.hpp"

HHOOK hMouseHook;
HINSTANCE hInst;
HWND globalHwnd = NULL;



#define DEFAULT_WINDOW_WIDTH	600
#define DEFAULT_WINDOW_HEIGHT	400




BOOL InitInstance(HINSTANCE hInstance, int nCmdShow, LPCSTR szWindowClass, LPCSTR szTitle)
{
	HWND hWnd = CreateWindowEx(0, szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT, nullptr, nullptr, hInstance, nullptr);
	globalHwnd = hWnd;

	if (!hWnd)
		return FALSE;

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

void InitZoomContext()
{
	ZoomContext& zoomContext = Global::getInstance().getZoomContext();
	zoomContext.screenW = GetSystemMetrics(SM_CXSCREEN);
	zoomContext.screenH = GetSystemMetrics(SM_CYSCREEN);
	
	zoomContext.zoomRect.left = zoomContext.zoomRect.top = 0;
	zoomContext.zoomRect.right = zoomContext.screenW;
	zoomContext.zoomRect.bottom = zoomContext.screenH;
}



int APIENTRY wWinMain(	_In_ HINSTANCE hInstance,
						_In_opt_ HINSTANCE hPrevInstance,
						_In_ LPWSTR lpCmdLine,
						_In_ int nCmdShow)
{
	hInst = hInstance;
	if (CheckIsRunning())
		exit(1);

	SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);
	InitZoomContext();
	// Magnifer API initialization
	if(!MagInitialize())
		exit(1);

	MSG  msg;
	WNDCLASSEX wcex = { 0 };

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = ProgramWindowWnd::ProgramWindowWndProc;
	wcex.hInstance = hInstance;
	wcex.hCursor = LoadCursor(0, IDC_ARROW);
	wcex.hbrBackground = GetSysColorBrush(COLOR_BTNFACE);
	wcex.lpszClassName = "magnifying-glass";

	try
	{
		if(!RegisterClassEx(&wcex))
			throw std::runtime_error(std::format("Error of register window class {}", GetLastError()));
	}
	catch (std::exception& e)
	{
		MessageBox(NULL, e.what(), "Error", MB_ICONERROR | MB_OK);
		exit(-1);
	}
	
	const char* windowTitle = "magnifying-glass";
	if (!InitInstance(hInstance, nCmdShow, wcex.lpszClassName, windowTitle))
		exit(-1);


	//hMouseHook

	while (GetMessage(&msg, NULL, 0, 0)) 
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

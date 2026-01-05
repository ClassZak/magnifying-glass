#include "pch.hpp"
#include "CheckIsRunning.hpp"
#include "Global.hpp"
#include "CheckPolicy.hpp"
#include "ProgramWindow.hpp"


LONG zoom = 0;

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



int APIENTRY wWinMain(	_In_ HINSTANCE hInstance,
						_In_opt_ HINSTANCE hPrevInstance,
						_In_ LPWSTR lpCmdLine,
						_In_ int nCmdShow)
{
	hInst = hInstance;
	if (CheckIsRunning())
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
	
	const char* windowTitle = "234";
	if (!InitInstance(hInstance, nCmdShow, wcex.lpszClassName, windowTitle))
		exit(-1);
	MessageBoxExW(globalHwnd, L"dfgdfgdfg", L"dfgdfgdfg", MB_OK, NULL);

	while (GetMessage(&msg, NULL, 0, 0)) 
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

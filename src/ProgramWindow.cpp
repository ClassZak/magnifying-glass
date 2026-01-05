#include "ProgramWindow.hpp"

ProgramWindowWnd::ProgramWindowWnd()
{
}

LRESULT ProgramWindowWnd::ProgramWindowWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
		case WM_CREATE:
		{
			
			break;
		}
		case WM_DESTROY:
		{
			PostQuitMessage(0);
			break;
		}
		default:
			return DefWindowProc(hwnd, msg, wParam, lParam);
	}
	
	return DefWindowProcW(hwnd, msg, wParam, lParam);
}

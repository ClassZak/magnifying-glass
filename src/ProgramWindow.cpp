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
		// Draw border for magnify window
		case WM_PAINT:
		{
			PAINTSTRUCT ps;
			RECT rWindowRect;
			GetClientRect(hwnd, &rWindowRect);
			HDC hdc = BeginPaint(hwnd, &ps);
			
			
			HBRUSH hBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
			HPEN hPen = CreatePen(PS_SOLID, 2, RGB(0,255,0));
			
			HGDIOBJ hgdiOldBrush = SelectObject(hdc, hBrush);
			HGDIOBJ hgdiOldPen = SelectObject(hdc, hPen);
			
			Rectangle(hdc, rWindowRect.left, rWindowRect.top, rWindowRect.right, rWindowRect.bottom);
			
			SelectObject(hdc, hgdiOldBrush);
			SelectObject(hdc, hgdiOldPen);
			DeleteObject(hPen);
			
			
			EndPaint(hwnd, &ps);
			return 0;
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
	
	return DefWindowProc(hwnd, msg, wParam, lParam);
}

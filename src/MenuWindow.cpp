#include "MenuWindow.hpp"
#include "Global.hpp"
#include "ZoomContext.hpp"


#include "resource.h"



#include <format>





LRESULT MenuWindow::WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
		case WM_CREATE:
		{
			HWND creatingWindow = NULL;
			creatingWindow = CreateWindowExA
			(
				0L,
				"button",
				"+",
				WS_VISIBLE | WS_CHILD | ES_CENTER | BS_DEFPUSHBUTTON | BS_PUSHBUTTON | WS_TABSTOP,
				0, 0,
				64, 64, 
				hwnd, (HMENU)IDC_BUTTON_MENU_WINDOW_ADD_ZOOM, NULL, NULL
			);
			if (!creatingWindow)
			{
				std::string errorMessage = std::format(R"(Error creating button
File: {}
Function: {}
Line: {})", __FILE__, __FUNCTION__, __LINE__);
				MessageBox(NULL, errorMessage.c_str(), "Error", MB_OK | MB_ICONERROR);
				exit(EXIT_FAILURE);
			}
			creatingWindow = CreateWindowExA
			(
				0L,
				"button",
				"-",
				WS_VISIBLE | WS_CHILD | ES_CENTER | BS_DEFPUSHBUTTON | BS_PUSHBUTTON | WS_TABSTOP,
				64, 0,
				64, 64,
				hwnd, (HMENU)IDC_BUTTON_MENU_WINDOW_SUB_ZOOM, NULL, NULL
			);
			if (!creatingWindow)
			{
				std::string errorMessage = std::format(R"(Error creating button
File: {}
Function: {}
Line: {})", __FILE__, __FUNCTION__, __LINE__);
				MessageBox(NULL, errorMessage.c_str(), "Error", MB_OK | MB_ICONERROR);
				exit(EXIT_FAILURE);
			}
			creatingWindow = CreateWindowExA
			(
				0L,
				"button",
				"=",
				WS_VISIBLE | WS_CHILD | ES_CENTER | BS_DEFPUSHBUTTON | BS_PUSHBUTTON | WS_TABSTOP,
				128, 0,
				64, 64,
				hwnd, (HMENU)IDC_BUTTON_MENU_WINDOW_RES_ZOOM, NULL, NULL
			);
			if (!creatingWindow)
			{
				std::string errorMessage = std::format(R"(Error creating button
File: {}
Function: {}
Line: {})", __FILE__, __FUNCTION__, __LINE__);
				MessageBox(NULL, errorMessage.c_str(), "Error", MB_OK | MB_ICONERROR);
				exit(EXIT_FAILURE);
			}
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
		case WM_COMMAND:
		{
			static ZoomContext& zoomContext = Global::getInstance().getZoomContext();
			int iCommandId = LOWORD(wParam);
			switch (iCommandId)
			{
				case IDC_BUTTON_MENU_WINDOW_ADD_ZOOM:
				{
					if (zoomContext.zoom >= ZoomContext::MAX_ZOOM)
						return DefWindowProc(hwnd, msg, wParam, lParam);
					else
						zoomContext.zoom <<= 1;
					break;
				}
				case IDC_BUTTON_MENU_WINDOW_SUB_ZOOM:
				{
					if (zoomContext.zoom <= 1L)
						return DefWindowProc(hwnd, msg, wParam, lParam);
					else
						zoomContext.zoom >>= 1;
					break;
				}
				case IDC_BUTTON_MENU_WINDOW_RES_ZOOM:
				{
					zoomContext.zoom = 1L;
					break;
				}
				default:
					return DefWindowProc(hwnd, msg, wParam, lParam);
			}
			break;
		}
		case WM_DESTROY:
		{
			ZoomContext& zoomContex = Global::getInstance().getZoomContext();
			zoomContex.isRunning = FALSE;
			PostQuitMessage(0);
			break;
		}
		default:
			return DefWindowProc(hwnd, msg, wParam, lParam);
	}
	
	return DefWindowProc(hwnd, msg, wParam, lParam);
}

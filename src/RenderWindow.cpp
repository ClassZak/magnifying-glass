#include "RenderWindow.hpp"
#include "ZoomContext.hpp"
#include "Global.hpp"




LRESULT RenderWindow::WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
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
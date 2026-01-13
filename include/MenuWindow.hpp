#include <wtypes.h>




class MenuWindow
{
	static VOID UpdateZoomLabel(HWND hLabel);
public:
	static LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
};
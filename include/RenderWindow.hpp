#include <wtypes.h>




class RenderWindow
{
public:
	RenderWindow() = default;
	static LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
};
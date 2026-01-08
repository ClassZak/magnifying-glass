#include <wtypes.h>

struct ZoomContext
{
	// Handles
	HDC		screenDC = NULL;
	HDC		memDC = NULL;
	HBITMAP	srcBitmap = NULL;
	HBITMAP	oldBitmap = NULL;

	int screenW;
	int screenH;

	// Logic
	volatile LONG zoom = 1U;
	volatile RECT zoomRect {NULL};
	POINT mousePos {NULL};
};
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
	volatile BOOL isRunning = TRUE;
	volatile LONG zoom = 1U;
	RECT zoomRect {NULL};
	POINT mousePos {NULL};


	static const LONG MAX_ZOOM;
};
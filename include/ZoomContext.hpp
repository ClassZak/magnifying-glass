#include <wtypes.h>

struct ZoomContext
{
	// Захват
	HDC		screenDC;
	HDC		memDC;
	HBITMAP	srcBitmap;
	HBITMAP	oldBitmap;

	int screenW;
	int screenH;

	// Логика лупы
	volatile LONG zoom;
	volatile LONG srcX;
	volatile LONG srcY;

};
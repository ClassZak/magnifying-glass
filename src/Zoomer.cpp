#include "Zoomer.hpp"



#include <Windows.h>
#include <stdexcept>
#include <thread>




inline void Zoomer::zoomScreenWinAPI(struct ZoomContext* ctx)
{
	if (ctx == nullptr)
		throw std::runtime_error("No context for zoom");
	
	InterlockedExchange(&ctx->zoom, _zoom);
	
}

void Zoomer::zoomScreen(LONG zoom, struct ZoomContext* ctx)
{
	if (zoom < 1)
		zoom = 1;
	if (zoom > 16)
		zoom = 16;

	if (ctx == nullptr)
		throw std::runtime_error("No context for zoom");

	this->setZoom(zoom);
	this->zoomScreenWinAPI(ctx);
}

void Zoomer::ZoomContextInit(ZoomContext* ctx)
{
	if (ctx == nullptr)
		throw std::runtime_error("No context for initilize");
	
	ctx->screenDC = GetDC(NULL);
	ctx->memDC = CreateCompatibleDC(ctx->screenDC);
	ctx->screenW = GetSystemMetrics(SM_CXSCREEN);
	ctx->screenH = GetSystemMetrics(SM_CYSCREEN);

	ctx->srcBitmap = CreateCompatibleBitmap
	(ctx->screenDC, ctx->screenW, ctx->screenH);

	ctx->oldBitmap = static_cast<HBITMAP>(SelectObject(ctx->memDC, ctx->srcBitmap));

	ctx->zoom = 1;
}

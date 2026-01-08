#include <wtypes.h>


struct ZoomContext;

class Global
{
	Global();

	LONG _zoom = 0.f;
	ZoomContext* _zoomContext;
public:
	Global(const Global&) = delete;
	Global& operator = (const Global&) = delete;

	static Global& getInstance()
	{
		static Global instance;
		return instance;
	}

	LONG& getZoom()
	{
		return _zoom;
	}
	ZoomContext& getZoomContext() noexcept
	{
		return *_zoomContext;
	}
	
};
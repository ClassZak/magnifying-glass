#include <wtypes.h>


struct ZoomContext;

class Global
{
	Global();

	ZoomContext* _zoomContext; // Inits in constructor
public:
	Global(const Global&) = delete;
	Global& operator = (const Global&) = delete;

	static Global& getInstance()
	{
		static Global instance;
		return instance;
	}

	ZoomContext& getZoomContext() noexcept
	{
		return *_zoomContext;
	}
	
};
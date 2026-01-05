#include "ZoomContext.hpp"




#include <wtypes.h>




class Zoomer
{
	volatile LONG _zoom = 1;

private:

	inline void setZoom(LONG zoom)
	{
		_zoom = zoom;
	}
	inline void zoomScreenWinAPI(struct ZoomContext* ctx);

#pragma region Private Constructor
	Zoomer() = default;
#pragma endregion
public:
#pragma region Sigleton code for C++ 11 and higher
	Zoomer(const Zoomer&) = delete;
	Zoomer& operator = (const Zoomer&) = delete;
	Zoomer& getInstance()
	{
		static Zoomer instance;
		return instance;
	}
#pragma endregion

	inline LONG getZoom() const
	{
		return _zoom;
	}

	void zoomScreen(LONG zoom, struct ZoomContext* ctx);


	void ZoomContextInit(struct ZoomContext* ctx);
};

class Global
{
	Global() = default;

	LONG _zoom = 0.f;
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
	
};
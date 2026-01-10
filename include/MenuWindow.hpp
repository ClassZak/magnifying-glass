#include <wtypes.h>
#include <unordered_map>
#include <string>
#include <functional>
#include <thread>
#include <mutex>
#include <exception>




class MenuWindow
{
	std::unordered_map<std::string, std::function<void()>> _functions;
private:
	inline const std::function<void()>* getFunction(const std::string& name) const
	{
		auto it = _functions.find(name);
		return it != _functions.end() ? &it->second : nullptr;
	}
	inline void addFunction(const std::string& name, const std::function<void()> func)
	{
		if (_functions.find(name) != _functions.end())
			throw std::runtime_error(std::string("Function ") + name + " is already exists");

		_functions.insert(std::pair<std::string, std::function<void()>>(name, func));
	}
	inline void invokeFunction(const std::string& name)
	{
		auto* func = getFunction(name);
		if (func != nullptr)
			(*func)();
		else
			throw std::runtime_error(std::string("Function ") + name + " is not exists");
	}

public:
	MenuWindow() = default;
	static LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
};
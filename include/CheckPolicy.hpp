#include <string>
#include <windows.h>



enum CheckPolicyResult
{
	WRONG_VALUE = -3, // The policy is set to an unrecognized value
	UNAVIABLE = -2, // Couldn't access registry
	NOT_CONFIGURED = -1, // Policy is not configured
	DISABLED = 0, // Policy is disabled
	ENABLED = 1, // Policy is enabled
};


// Registry scope where gpo policy values are stored.
static const HKEY POLICIES_SCOPE_MACHINE = HKEY_LOCAL_MACHINE;
static const HKEY POLICIES_SCOPE_USER = HKEY_CURRENT_USER;
static const std::wstring POLICIES_PATH = L"SOFTWARE\\Policies\\PowerToys";

CheckPolicyResult CheckPolicy(const std::wstring& registry_value_name)
{
	HKEY key{};
	DWORD value = 0xFFFFFFFE;
	DWORD valueSize = sizeof(value);

	bool machine_key_found = true;
	auto res = RegOpenKeyExW(POLICIES_SCOPE_MACHINE, POLICIES_PATH.c_str(), 0, KEY_READ, &key);
	if (res != ERROR_SUCCESS)
		machine_key_found = false;

	if (machine_key_found)
	{
		// If the path was found in the machine, we need to check if the value for the policy exists.
		auto res = RegQueryValueExW(key, registry_value_name.c_str(), nullptr, nullptr, reinterpret_cast<LPBYTE>(&value), &valueSize);

		RegCloseKey(key);

		if (res != ERROR_SUCCESS)
		{
			// Value not found on the path.
			machine_key_found = false;
		}
	}

	if (!machine_key_found)
	{
		// If there's no value found on the machine scope, try to get it from the user scope.
		auto res = RegOpenKeyExW(POLICIES_SCOPE_USER, POLICIES_PATH.c_str(), 0, KEY_READ, &key);
		if (res != ERROR_SUCCESS)
		{
			if (res == ERROR_FILE_NOT_FOUND)
				return CheckPolicyResult::NOT_CONFIGURED;

			return CheckPolicyResult::UNAVIABLE;
		}
		res = RegQueryValueExW(key, registry_value_name.c_str(), nullptr, nullptr, reinterpret_cast<LPBYTE>(&value), &valueSize);
		RegCloseKey(key);

		if (res != ERROR_SUCCESS)
		{
			return CheckPolicyResult::NOT_CONFIGURED;
		}
	}

	switch (value)
	{
	case 0:
		return CheckPolicyResult::DISABLED;
	case 1:
		return CheckPolicyResult::ENABLED;
	default:
		return CheckPolicyResult::WRONG_VALUE;
	}
}

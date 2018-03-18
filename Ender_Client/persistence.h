#pragma once
#include <Windows.h>
#include <strsafe.h>

class Registry
{
	HKEY key;
	DWORD TotalKeys;
	DWORD lenValueKey = 1000;
	LPWSTR ValueofKey;
	LPWSTR RegStr;
	LPWSTR FileName;
public:
	Registry();
	~Registry();
	bool CheckRegKey();
	bool SetRegKey();
};
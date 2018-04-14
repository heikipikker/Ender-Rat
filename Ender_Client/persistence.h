#pragma once
#define WIN32_LEAN_AND_MEAN
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
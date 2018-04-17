#include "server.h"
#include "com_vars.h"
#include "extras.h"
#include <iostream>
#include <strsafe.h>

string SERVER::file_port_str = " ";

SERVER::SERVER(SOCKET tmp, int id)
{
	client = tmp;
	client_id = id;
	client_status = 1;
}

SERVER::~SERVER()
{
	closesocket(client);
}

int SERVER::get_client_id()
{
	return client_id;
}

int SERVER::get_client_status()
{
	return client_status;
}

bool SERVER::send_command(string& command)
{
	const char* command_to_send = command.c_str();
	char commandLen[10];
	_itoa_s(command.length()+1, commandLen, 10 ,10);

	if(send(client, commandLen, 10, 0) == SOCKET_ERROR) // send size as char buffer
	{
		client_status = 0;
		return false;
	}

	if(send(client, command_to_send, command.length() + 1, 0) == SOCKET_ERROR)
	{
		client_status = 0;
		return false;
	}
	return true;
}

bool SERVER::recieve_response(string& response)
{
	char lenOfResponse[10];    // recieve size as char buffer
	if (recv(client, lenOfResponse, 10, 0) < 0) {  // TODO: Entirely new function for large data, Probably a new thread
		client_status = 0;
		return false;
	}

	int size = atoi(lenOfResponse);
	char *resp = new char[size];

	if(recv(client, resp, size, 0) < 0)
	{
		client_status = 0;
		return false;
	}

	response.assign(resp);
	delete[] resp;
	return true;
}

string SERVER::get_username()
{
	string username = "username";
	send_command(username);
	recieve_response(username);
	return username;
}

void SERVER::recieve_file_async(LPVOID param)
{
	SOCKET file_listen_socket = socket(AF_UNSPEC, SOCK_STREAM, IPPROTO_TCP);
	struct addrinfo *sock;
	getaddrinfo("0.0.0.0", SERVER::file_port_str.c_str(), NULL, &sock);
	
	if (bind(file_listen_socket, sock->ai_addr, sock->ai_addrlen))
	{
		show_error("Unable To Bind Socket!");
		return;
	}

	if(listen(file_listen_socket, SOMAXCONN))
	{
		show_error("Listen Failed!");
		return;
	}
	SOCKET file_socket = accept(file_listen_socket, NULL, NULL);
	if(file_socket == INVALID_SOCKET)
	{
		show_error("Unable To accept Connection!");
		return;
	}
	char file_ext[10];
	if (recv(file_socket, file_ext, 10, 0) < 0)
	{
		closesocket(file_listen_socket);
		closesocket(file_socket);
		show_error(" File Transfer Failed!");
		return;
	}

	char file_size[15];
	if (recv(file_socket, file_size, 15, 0) < 0)
	{
		closesocket(file_listen_socket);
		closesocket(file_socket);
		show_error(" File Transfer Failed!");
		return;
	}

	int size = atoi(file_size);
	char* file_buffer = new char[size];
	int current_offset = 0;

	while (true)
	{
		int bytes_recvd = recv(file_socket, file_buffer + current_offset, size - current_offset, 0);
		current_offset += bytes_recvd;
		if (bytes_recvd < 0)
		{
			closesocket(file_listen_socket);
			closesocket(file_socket);
			return;
		}
		if (bytes_recvd == 0)
		{
			break;
		}
	}

	string filename = gen_random_string();
	string filepath = "C:\\ender\\";
	filepath.append(filename);
	filepath.append(file_ext);
	HANDLE file = CreateFileA(
		filepath.c_str(),
		GENERIC_READ | GENERIC_WRITE,
		0,
		NULL,
		CREATE_NEW,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	if(file == NULL)
	{
		show_error("CreateFile Failed!");
	}

	if(!WriteFile(file, file_buffer, size, 0, NULL))
	{
		show_error("Write File Failed!");
	}
	show_update("File Saved at " + filepath);
	show_shell();
	closesocket(file_socket);
	closesocket(file_listen_socket);
	CloseHandle(file);
	delete[] file_buffer;
}

void SERVER::recieve_file()
{
	srand(GetTickCount());
	int port = 1337 + (rand() % 2000);
	char port_chr[20];
	_itoa_s(port, port_chr, 20, 10);   // No need to open new port TODO: Change this and counterpart function in client side
	file_port_str.assign(port_chr);
	send_command(file_port_str);
	CreateThread(
		NULL,
		NULL,
		(LPTHREAD_START_ROUTINE)recieve_file_async,
		NULL, NULL, NULL);
}

string SERVER::gen_random_string()
{
	string root_str = "1234567890abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
	string str;
	srand(GetTickCount());
	for(int i=0;i<10;i++)
	{
		int index = rand() % 63;
		str.push_back(root_str[index]);
	}
	return str;
}
#include "server.h"
#include "com_vars.h"
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
	getaddrinfo("127.0.0.1", SERVER::file_port_str.c_str(), NULL, &sock);
	
	if (!bind(file_listen_socket, sock->ai_addr, sock->ai_addrlen))
	{
		listen(file_listen_socket, SOMAXCONN);
		SOCKET file_socket = accept(file_listen_socket, NULL, NULL );
		closesocket(file_listen_socket);
		char file_ext[10];
		if (recv(file_socket, file_ext, 10, 0) > 0)
		{
			char file_size[15];
			if(recv(file_socket, file_size, 15, 0) > 0)
			{
				int size = atoi(file_size);
				char* file_buffer = new char[size];
				int current_offset = 0;

				while (true) 
				{
					int bytes_recvd = recv(file_socket, file_buffer + current_offset, size - current_offset, 0);
					current_offset += bytes_recvd;
					if (bytes_recvd < 0)
					{
						closesocket(file_socket);
						return;
					}
					if (bytes_recvd == 0)
					{
						break;
					}
				}

				char* filename = gen_random_string();
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

				WriteFile(file, file_buffer, size, 0, NULL);
				cout << endl <<"File Saved at " << filepath << endl;
				closesocket(file_socket);
				return;
			}
		}
		closesocket(file_socket);
	}
	cout << endl << " File Transfer FAILED" << endl;
}

void SERVER::recieve_file()
{
	int port = 1337 + (GetTickCount() % 2000);
	char port_chr[20];
	_itoa_s(port, port_chr, 20, 10);
	file_port_str.assign(port_chr);
	send_command(file_port_str);
	CreateThread(
		NULL,
		NULL,
		(LPTHREAD_START_ROUTINE)recieve_file_async,
		NULL, NULL, NULL);
}

char* SERVER::gen_random_string()
{
	char root_str[63] = "1234567890abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
	char str[16];

	for(int i=0;i<15;i++)
	{
		int index = GetTickCount() % 63;
		str[i] = root_str[index];
	}
	str[15] = '\0';
	return str;
}
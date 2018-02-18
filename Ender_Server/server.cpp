#include "server.h"
#include <iostream>

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

	if(send(client, commandLen, 10, 0) == SOCKET_ERROR)
	{
		return false;
	}

	if(send(client, command_to_send, command.length() + 1, 0) == SOCKET_ERROR)
	{
		return false;
	}
	return true;
}

bool SERVER::recieve_response(string& response)
{
	char lenOfResponse[10];   
	if (recv(client, lenOfResponse, 10, 0) < 0) {  // TODO: Entirely new function for large data, Probably a new thread
		return false;
	}

	int size = atoi(lenOfResponse) + 1;
	char *resp = new char[size];

	if(recv(client, resp, size, 0) < 0)
	{
		return false;
	}

	response.assign(resp);
	delete[] resp;
	return true;
}

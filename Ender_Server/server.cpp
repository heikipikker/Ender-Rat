#include "server.h"

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
	int len = command.length();
	int status = send(client, (char*)&len, 4, 0);
	if (status == SOCKET_ERROR)
	{
		return false;
	}
	status = send(client, command_to_send, len, 0);
	return status != SOCKET_ERROR ? true : false;
}

void SERVER::set_client_socket(SOCKET& temp)
{
	client = temp;
}

bool SERVER::recieve_response(char* response)
{

}

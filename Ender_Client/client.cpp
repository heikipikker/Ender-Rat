#include "client.h"

CLIENT::CLIENT()
{
	connect_to_server();
}

CLIENT::~CLIENT()
{
	closesocket(client_socket);
}

void CLIENT::connect_to_server()
{
	struct addrinfo hints;
	struct addrinfo *result;
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	getaddrinfo("127.0.0.1", "1457", &hints, &result);
	client_socket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	int stat = connect(client_socket, result->ai_addr, (int)result->ai_addrlen);
	while (stat == SOCKET_ERROR) {
		client_socket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
		stat = connect(client_socket, result->ai_addr, (int)result->ai_addrlen);
		Sleep(1000);
	}
}

bool CLIENT::send_response(string& response)
{
	const char* response_to_send = response.c_str();
	char responseLen[10];
	_itoa_s(response.length()+1, responseLen, 10, 10);

	if (send(client_socket, responseLen, 10, 0) == SOCKET_ERROR)
	{
		return false;
	}

	if (send(client_socket, response_to_send, response.length()+1, 0) == SOCKET_ERROR)
	{
		return false;
	}
	return true;
}

bool CLIENT::recieve_command(string& command)
{
	char lenOfCommand[10];
	if (recv(client_socket, lenOfCommand, 10, 0) < 0) {  // TODO: Entirely new function to send/recieve large data
		return false;
	}

	int size = atoi(lenOfCommand);
	char *comm = new char[size];
	memset(comm, 0, size);
	if (recv(client_socket, comm, size, 0) < 0)
	{
		return false;
	}

	command.assign(comm);
	delete[] comm;
	return true;
}


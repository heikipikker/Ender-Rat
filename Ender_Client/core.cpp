#include "core.h"

Core::~Core()
{
	closesocket(client_socket);
}

void Core::connect_to_server()
{
	struct addrinfo hints;
	struct addrinfo *result;
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	getaddrinfo(IP, PORT, &hints, &result);
	client_socket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	int stat = connect(client_socket, result->ai_addr, (int)result->ai_addrlen);
	while (stat == SOCKET_ERROR) {
		client_socket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
		stat = connect(client_socket, result->ai_addr, (int)result->ai_addrlen);
		Sleep(1000);
	}
}

bool Core::send_response(string& response)
{
	const char* response_to_send = response.c_str();
	char responseLen[10];
	_itoa_s(response.length() + 1, responseLen, 10, 10);

	if (send(client_socket, responseLen, 10, 0) == SOCKET_ERROR)
	{
		return false;
	}

	if (send(client_socket, response_to_send, response.length() + 1, 0) == SOCKET_ERROR)
	{
		return false;
	}
	return true;
}

bool Core::recieve_command(string& command)
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

bool Core::send_file(string filepath)
{
	LARGE_INTEGER filesize;
	string port;
	recieve_command(port);  // recieve port to send file on

	HANDLE file = CreateFileA(
		filepath.c_str(),
		GENERIC_READ,
		FILE_SHARE_READ,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	GetFileSizeEx(file, &filesize);
	char* file_buffer = (char*)malloc(filesize.LowPart);

	if(!ReadFile(file, file_buffer, filesize.LowPart, 0, NULL))
	{
		CloseHandle(file);
		return false;
	}

	SOCKET file_socket = socket(AF_UNSPEC, SOCK_STREAM, IPPROTO_TCP);
	struct addrinfo *sock;
	getaddrinfo(IP, port.c_str(), NULL, &sock);

	int status = connect(file_socket, sock->ai_addr, sock->ai_addrlen);
	if(status == SOCKET_ERROR)
	{
		CloseHandle(file);
		closesocket(file_socket);
		return false; // Transfer Failed
	}

	string filename = filepath.substr(filepath.find_last_of('.'));
	if(send(file_socket, filename.c_str(), 10, 0) == SOCKET_ERROR) // send file extension
	{
		closesocket(file_socket);
		return false;
	}

	char size[15];
	_itoa_s(filesize.LowPart, size, 15, 10);

	if(send(file_socket, size, 15, 0) == SOCKET_ERROR)  // send file size
	{
		closesocket(file_socket);
		return false;
	}

	int current_offset = 0;
	int fsize = filesize.LowPart;
	while(true)
	{
		int bytes_sent = send(file_socket, file_buffer + current_offset,  fsize - current_offset, 0);
		current_offset += bytes_sent;
		if (bytes_sent < 0)
		{
			closesocket(file_socket);
			free(file_buffer);
			return false;
		}
		if (bytes_sent == 0)
		{
			break;
		}
	}
	CloseHandle(file);
	closesocket(file_socket);
	free(file_buffer);
	return true;
}
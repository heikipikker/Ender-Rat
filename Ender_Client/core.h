#pragma once
#include <WS2tcpip.h>
#include <string>
using namespace std;
class Core
{
	SOCKET client_socket = INVALID_SOCKET;
public:
	~Core();
	void connect_to_server();
	bool send_response(string&);
	bool recieve_command(string&);
};
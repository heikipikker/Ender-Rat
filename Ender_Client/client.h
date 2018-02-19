#pragma once
#include <WS2tcpip.h>
#include <string>

using namespace std;

class CLIENT
{
	SOCKET client_socket = INVALID_SOCKET;
public:
	CLIENT();
	~CLIENT();
	void connect_to_server();
	bool send_response(string&);
	bool recieve_command(string&);
};
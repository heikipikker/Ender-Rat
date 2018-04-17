#pragma once
#include <WS2tcpip.h>

void handle_connections(LPVOID);
void handle_client(LPVOID);
void admin_mode();
void handle_client_panel(SERVER&, std::string&);
void handle_admin_command(std::string&);

typedef struct LSOCKET_PARAM{
	SOCKET ListenSocket;
}ListenSocketStruct;

typedef struct CSOCKET_PARAM{
	SOCKET ClientSocket;
	int id;
}ClientSocketStruct;
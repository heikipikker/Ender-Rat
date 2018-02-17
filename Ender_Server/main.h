#pragma once

void handle_connections(LPVOID);
void handle_client(LPVOID);
void admin_mode();
void display_admin_help();
void display_client_help();
void handle_client_panel(SERVER&, string&);
void handle_admin_command(string&);
bool check_current_client_id(int);

typedef struct LSOCKET_PARAM{
	SOCKET ListenSocket;
}ListenSocketStruct;

typedef struct CSOCKET_PARAM{
	SOCKET ClientSocket;
	int id;
}ClientSocketStruct;
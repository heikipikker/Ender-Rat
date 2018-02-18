#pragma once

void handle_connections(LPVOID);
void handle_client(LPVOID);
void admin_mode();
void display_admin_help();
void display_client_help();
void handle_client_panel(SERVER&, string&);
void handle_admin_command(string&);
bool check_current_client_id(int);

vector<SERVER*> client_array;
vector<int> client_ids;
int id_var = 0;
int current_client = -1;

typedef struct LSOCKET_PARAM{
	SOCKET ListenSocket;
}ListenSocketStruct;

typedef struct CSOCKET_PARAM{
	SOCKET ClientSocket;
	int id;
}ClientSocketStruct;
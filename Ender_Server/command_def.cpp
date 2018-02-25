#include <Windows.h>
#include <iostream>
#include "command_def.h"
using namespace std;

void display_client_help()
{
	cout << "----> show <msg>   - Display a Message Box with <msg> on client's system" << endl;
	cout << "----> exit         - Go back to admin mode" << endl;
	cout << "----> disconnect   - Disconnect the current client" << endl;
}

void display_admin_help()
{
	cout << " Commands:" << endl;
	cout << "--> show clients - Display all the connected clients" << endl;
	cout << "--> client <id>  - To Open Client Interacting Panel for that Client" << endl;
	cout << "--> quit         - End this program" << endl;
	cout << "--> admin help   - Display this help message" << endl << "# ";
}


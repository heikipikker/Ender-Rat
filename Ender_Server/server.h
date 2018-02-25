#include <WS2tcpip.h>
#include <string>

using namespace std;
class SERVER
{
	SOCKET client;
	int client_id;
	int client_status;
	//char username[40];
public:
	SERVER() {}
	SERVER(SOCKET, int);
	~SERVER();
	bool send_command(string&);
	bool recieve_response(string&);
	//string show_client_info();
	int get_client_id();
	int get_client_status();
};

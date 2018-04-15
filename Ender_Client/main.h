#pragma once
int init_winsock();
struct addrinfo *result;
void handle_command(CLIENT&, string&);
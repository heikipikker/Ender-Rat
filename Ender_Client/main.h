#pragma once
int init_winsock();
struct addrinfo *result;
void handle_command(CLIENT&, string&);
void send_username(CLIENT& client);
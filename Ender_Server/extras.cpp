#define WIN32_LEAN_AND_MEAN
#include "extras.h"
#include <Windows.h>
#include "com_vars.h"

bool check_current_client_id(int id)
{
	for (int i = 0; i < client_ids.size(); i++)
	{
		if (id == client_ids[i])
		{
			return true;
		}
	}
	return false;
}

void show_error(string error)
{
	HANDLE std = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(std, FOREGROUND_RED | FOREGROUND_INTENSITY);
	cout << endl <<"[-] " << error << endl;
	SetConsoleTextAttribute(std, 7);
}

void show_update(string info)
{
	HANDLE std = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(std, 11);
	cout << endl << "[+] " << info << endl;
	SetConsoleTextAttribute(std, 7);
}

void show_update(string info, int num)
{
	HANDLE std = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(std, 11);
	cout << endl << "[+] " << info << num << endl;
	SetConsoleTextAttribute(std, 7);
}

void show_update(string info, int num, string info2)
{
	HANDLE std = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(std, 11);
	cout << endl << "[+] " << info << num << info2 << endl;
	SetConsoleTextAttribute(std, 7);
}

void show_shell()
{
	current_client == -1 ? cout << "# " : cout << "$ client-" << current_client << ": ";
}


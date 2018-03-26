#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <fstream>
#include <time.h>

#include "Main.h"
#include "Packet.h"
#include "Login.h"
#include "Game.h"
#include "Misc.h"
#include "Music.h"

//#pragma comment(linker, "/SUBSYSTEM:WINDOWS")

using namespace std;

int version = 41;

SOCKET sock;
unsigned char *pac;

bool done = false;
bool fullscreen = false;

extern bool colorkey;

extern int FPS;

int resX = 800;
int resY = 600;

extern bool dc;
extern bool transWindow;

vector<ConfigOption> cf;

bool noSound = false;
bool noMusic = false;

extern bool login;
extern bool spawned;
extern bool remLogin;

extern vector<LoginMessage*> lmsgs;

extern char loginName[40];

int main(int argc, char **argv)
{
	if(argc > 1)
	{
		if(strcmp(argv[1], "puhadennapuhn") == 0)
		{
			bool didGame = false;

			srand((unsigned)time(NULL));

			int res = Connect();

			lmsgs.at(0)->Clean();
			delete lmsgs.at(0);
			lmsgs.clear();

			UpdateLogin();

			if(res != 0)
			{
				switch(res)
				{
					case 1: KMessageBox("Error", MB_OK, "Network start-up problems.");
						break;

					case 2: KMessageBox("Error", MB_OK, "Cannot connect to the server.");
						break;
				}
			}

			else
				GameLoop();

			SaveConfig();
			CleanUp();

			if(!dc)
				closesocket(sock);
		}
	}

	return 0;
}

void SaveConfig()
{
	if(cf.size() > 0)
	{
		ofstream o("Config.txt");

		for(unsigned int i = 0; i < cf.size(); i++)
		{
			o << cf.at(i).command;

			switch(cf.at(i).param)
			{
				case TWOINT: o << " " << cf.at(i).int1 << " " << cf.at(i).int2 << endl;
					break;

				case TEXTCONFIG: o << " " << cf.at(i).arb << endl;
					break;

				case ONEINT: o << " " << cf.at(i).int1 << endl;
					break;
			}
		}

		o.close();
	}
}

void RemoveConfigOption(char *name)
{
	for(unsigned int i = 0; i < cf.size(); i++)
	{
		if(strcmp(name, cf.at(i).command) == 0)
		{
			cf.erase(cf.begin() + i);

			break;
		}
	}
}

void AddConfigOption(int TYPE, char *arb, int int1, int int2, int param, char *name)
{
	for(unsigned int i = 0; i < cf.size(); i++)
	{
		if(strcmp(name, cf.at(i).command) == 0)
		{
			cf.erase(cf.begin() + i);

			break;
		}
	}

	ConfigOption co;

	strcpy_s(co.command, name);

	if(arb != NULL)
		strcpy_s(co.arb, arb);

	co.int1 = int1;
	co.int2 = int2;
	co.param = param;

	cf.push_back(co);
	SaveConfig();
}

int Connect()
{
	char connects[256] = "kobla.servegame.com";
	char portn[40] = "2400";

	WSADATA wsad;
	char temp[256];

	ifstream o("Config.txt");

	if(o)
	{
		while(o >> temp)
		{
			ConfigOption co;

			strcpy_s(co.command, temp);

			if(strcmp("IP:", temp) == 0)
			{
				o >> connects;
				strcpy_s(co.arb, connects);

				co.param = TEXTCONFIG;
			}

			else if(strcmp("Port:", temp) == 0)
			{
				o >> portn;
				strcpy_s(co.arb, portn);

				co.param = TEXTCONFIG;
			}

			else if(strcmp("ColorKey:", temp) == 0)
			{
				char temp2[20] = "";
				o >> temp2;

				if(strcmp("ON", temp2) == 0)
					colorkey = true;
				else
					colorkey = false;

				strcpy_s(co.arb, temp2);

				co.param = TEXTCONFIG;
			}
			
			else if(strcmp("FPS-cap:", temp) == 0)
			{
				o >> FPS;

				co.int1 = FPS;

				co.param = ONEINT;
			}

			else if(strcmp("Fullscreen:", temp) == 0)
			{
				o >> co.int1;

				if(co.int1 == 1)
					fullscreen = true;
				else
					fullscreen = false;

				co.param = ONEINT;
			}

			else if(strcmp("Resolution:", temp) == 0)
			{
				o >> resX;

				if(resX == 1024 || resX == 1280 || resX == 800 || resX == 640)
				{
				}

				else
					resX = 1024;

				o >> resY;

				if(resY == 768 || resY == 1024 || resY == 600 || resY == 480)
				{
				}

				else
					resY = 768;

				co.int1 = resX;
				co.int2 = resY;

				co.param = TWOINT;
			}

			else if(strcmp("Transparency:", temp) == 0)
			{
				o >> co.int1;

				if(co.int1 == 1)
					transWindow = true;
				else
					transWindow = false;

				co.param = ONEINT;
			}

			else if(strcmp("Sound:", temp) == 0)
			{
				o >> co.int1;

				if(co.int1 == 0)
					noSound = true;

				co.param = ONEINT;
			}

			else if(strcmp("Music:", temp) == 0)
			{
				o >> co.int1;

				if(co.int1 == 0)
					noMusic = true;

				co.param = ONEINT;
			}

			else if(strcmp("RemLogin:", temp) == 0)
			{
				o >> co.int1;

				if(co.int1 == 1)
					remLogin = true;

				co.param = ONEINT;
			}

			else if(strcmp("Login:", temp) == 0)
			{
				o >> co.arb;

				strcpy_s(loginName, co.arb);

				co.param = TEXTCONFIG;
			}

			cf.push_back(co);
		}
	}

	o.close();

	StartUp();
	LMessage(0, "Connecting, please wait..");
	UpdateLogin();

	struct addrinfo hints, *res;

	int result = 0;

	result = WSAStartup(MAKEWORD(2, 2), &wsad);
	if(result != 0)
		return 1;

	memset(&hints, 0, sizeof(hints));
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_family = AF_INET;

	result = getaddrinfo(connects, portn, &hints, &res);
	if(result != 0)
		return 1;

	sock = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
	if(sock == INVALID_SOCKET)
		return 1;

	result = connect(sock, res->ai_addr, res->ai_addrlen);
	if(result != 0)
		return 2;

	result = 1;
	setsockopt(sock, IPPROTO_TCP, TCP_NODELAY, (char*)&result, sizeof(int));

	return 0;
}

int CheckUpdate()
{
	Packet pak = Packet();

	pak.addHeader(0x1);
	
	pak.ready();

	return 0;
}

void getInput(int &intt)
{
	int ret = 0;
	string input = "";

	getline(cin, input);

	if(input.length() > 0)
	{
		for(unsigned int i = 0; i < input.length(); i++)
		{
			bool okay = true;

			for(int y = 0; y < 10; y++)
			{
				if(atoi(&input.c_str()[i]) == y)
				{
					okay = true;

					break;
				}
			}

			if(!okay)
				break;
			else
				ret++;
		}

		if(ret == input.length())
			intt = atoi(input.c_str());
		else
			intt = 0;
	}

	else
		intt = 0;
}

string getCharInput(int &result)
{
	int len = 0;
	string input = "";

	getline(cin, input);

	if(input.length() > 0)
	{
		for(unsigned int i = 0; i < input.length(); i++)
		{
			if(input.c_str()[i] == ' ')
			{
				result = 1;
				break;
			}

			len++;
		}

		if(input.length() == len)
			return input;
		else
			return "";
	}

	else
		return "";
}

void LogoutFunction()
{
	login = true;
	spawned = false;

	SmallClean();
}

void Log(char *msg, ...)
{
	va_list args;
	char *buf = new char[(strlen(msg) + 1024)];

	va_start(args, msg);
	vsnprintf_s(buf, (strlen(msg) + 1024), (strlen(msg) + 1024), msg, args);
	va_end(args);

	time_t t = time(0);
	tm *lt = new tm();
	int res = localtime_s(lt, &t);

	char *dates = new char[4096];
	sprintf_s(dates, 4096, "[%02d/%02d/%04d - %02d:%02d:%02d] ", lt->tm_mday, (lt->tm_mon + 1), (lt->tm_year + 1900), lt->tm_hour, lt->tm_min, lt->tm_sec);	

	ofstream w;
	w.open("KoblaLog.txt", fstream::app);
	if(w)
	{
		w << dates << buf << endl;
	}

	w.close();

	delete[] buf;
	delete[] dates;
}
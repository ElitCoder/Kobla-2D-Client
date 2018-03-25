#ifndef PACKET_H
#define PACKET_H

#ifdef _WIN32
#include <winsock2.h>
#include <windows.h>
#endif

#include <iostream>

std::string getString(char *packet, int pos, int strleng);
bool getBool(int stat);

std::string IntToString(int intt);
std::string FloatToString(float add);

class Packet
{
public:
	Packet()
	{
		this->pos = 3;

		this->packet = new unsigned char[61440];

		memset(this->packet, 0, 61440);
	}

	Packet(const Packet &c)
	{
		this->packet = new unsigned char[c.pos];

		this->pos = c.pos;

		memcpy(this->packet, c.packet, c.pos);
	}

	~Packet()
	{
		delete[] this->packet;
	}

	void addHeader(int header);
	void addInt(int what);
	void addLongInt(int add);
	void addString(char add[]);
	void addLongString(char add[]);

	void ready();

	int pos;
	unsigned char *packet;
};

#endif
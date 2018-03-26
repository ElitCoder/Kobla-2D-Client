#include "Packet.h"
#include "Main.h"
#include "Misc.h"

using namespace std;

extern SOCKET sock;

extern std::vector<SendPacket> toserv;

void Packet::addHeader(int header)
{
	this->packet[2] = (unsigned char)header;
}

void Packet::addInt(int what)
{
	this->packet[this->pos] = (unsigned char)what;
	this->pos++;
}

void Packet::addString(char add[])
{
	int length = strlen(add);

	this->packet[this->pos] = (unsigned char)length;
	this->pos++;

	for(unsigned int i = 0; i < strlen(add); i++)
	{
		this->packet[this->pos] = add[i];
		this->pos++;
	}
}

void Packet::addLongString(char add[])
{
	int strleng = strlen(add);
	char buf[20] = "";

	_itoa_s(strleng, buf, 10);
	this->addString(buf);

	for(int i = 0; i < strleng; i++)
	{
		this->packet[this->pos] = add[i];
		this->pos++;
	}
}

void Packet::addLongInt(int add)
{
	char buf[20] = "";

	_itoa_s(add, buf, 10);
	this->addString(buf);
}

std::string getString(char *packet, int pos, int strleng)
{
	char temp[1000] = "";
	std::string out;

	for(int i = 0; i < strleng; i++)
		temp[i] = packet[(pos + i)];

	out = temp;
	return out;
}

string IntToString(int intt)
{
	char itoado[20] = "";
	_itoa_s(intt, itoado, 20, 10);

	std::string retVal = itoado;
	return retVal;
}

string FloatToString(float add)
{
	char itoado[20] = "";
	sprintf_s(itoado, 20, "%2.1f", add);

	std::string retVal = itoado;
	return retVal;
}

void Packet::ready()
{
	this->packet[0] = 0x3;
	this->packet[1] = 0;

	unsigned char *datasend = new unsigned char[this->pos + 30];
	memset(datasend, 0, this->pos + 30);

	string lengd = IntToString(this->pos);

	int setpos = 0;
	datasend[0] = lengd.length();
	setpos++;

	for(unsigned int i = 0; i < lengd.length(); i++)
	{
		datasend[(i + 1)] = lengd.c_str()[i];
		setpos++;
	}

	memcpy(datasend + setpos, this->packet, this->pos);

	int fulllen = (this->pos + setpos);

	SendPacket csp = SendPacket();

	csp.bu = datasend;
	csp.len = fulllen;
	csp.cp = 0;

	while(true)
	{
		SendPacket *sp = &csp;

		if(sp->len == 0)
		{
			delete[] sp->bu;
			break;
		}

		else if(sp->cp == sp->len)
		{
			delete[] sp->bu;
			break;
		}

		else
		{
			int s = send(sock, (const char*)sp->bu + sp->cp, sp->len - sp->cp, 0);

			if(s == 0)
			{
				break;
			}

			else if(s < 0)
			{
				break;
			}

			else if(s > 0)
			{
				sp->cp += s;

				if(sp->cp >= sp->len)
				{
					delete[] sp->bu;
					break;
				}
			}
		}
	}
}

bool getBool(int stat)
{
	if(stat == 1)
		return true;

	else
		return false;
}
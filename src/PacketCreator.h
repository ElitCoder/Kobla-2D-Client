#ifndef PACKET_CREATOR_H
#define PACKET_CREATOR_H

#include "Packet.h"

enum {
	HEADER_LOGIN,
	HEADER_GET_CHARACTERS,
	HEADER_PING_ALIVE,
	HEADER_UNKNOWN
};

class PacketCreator {
public:
	static Packet login(const std::string& username, const std::string& password);
	static Packet ping();
};

#endif
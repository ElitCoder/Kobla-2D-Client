#ifndef PACKET_CREATOR_H
#define PACKET_CREATOR_H

#include "Packet.h"

enum {
	HEADER_LOGIN,
	HEADER_GET_CHARACTERS,
	HEADER_PING_ALIVE,
	HEADER_UNKNOWN,
	HEADER_SPAWN,
	HEADER_MOVE,
	HEADER_ADD_PLAYER,
	HEADER_REMOVE_CHARACTER
};

class PacketCreator {
public:
	static Packet login(const std::string& username, const std::string& password);
	static Packet ping();
	static Packet spawn();
	static Packet move(double x, double y, int direction, bool moving);
};

#endif
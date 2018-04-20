#pragma once
#ifndef PACKET_CREATOR_H
#define PACKET_CREATOR_H

//#include "Packet.h"

#include <string>

enum {
	HEADER_LOGIN,
	HEADER_GET_CHARACTERS,
	HEADER_PING_ALIVE,
	HEADER_UNKNOWN,
	HEADER_SPAWN,
	HEADER_MOVE,
	HEADER_ADD_PLAYER,
	HEADER_REMOVE_CHARACTER,
	HEADER_UPDATE_HEALTH,
	HEADER_SHOOT
};

class Packet;

class PacketCreator {
public:
	static Packet login(const std::string& username, const std::string& password);
	static Packet ping();
	static Packet spawn();
	static Packet move(double x, double y, int direction, bool moving);
	static Packet shoot();
};

#endif
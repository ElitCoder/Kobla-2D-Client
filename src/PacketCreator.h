#pragma once
#ifndef PACKET_CREATOR_H
#define PACKET_CREATOR_H

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
	HEADER_SHOOT,
	HEADER_HIT,
	HEADER_ACTIVATE,
	HEADER_TEXT,
	HEADER_CHAT
};

class Packet;
class Object;

class PacketCreator {
public:
	static Packet login(const std::string& username, const std::string& password);
	static Packet ping();
	static Packet spawn();
	static Packet move(double x, double y, int direction, bool moving);
	static Packet shoot();
	static Packet hit(int object_id, int hit_id);
	static Packet activate(Object* object);
	static Packet chat(const std::string& message);
};

#endif
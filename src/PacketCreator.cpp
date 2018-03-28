#include "PacketCreator.h"

using namespace std;

Packet PacketCreator::login(const string& username, const string& password) {
	Packet packet;
	packet.addHeader(HEADER_LOGIN);
	packet.addString(username);
	packet.addString(password);
	packet.finalize();
	
	return packet;
}

Packet PacketCreator::ping() {
	Packet packet;
	packet.addHeader(HEADER_PING_ALIVE);
	packet.finalize();
	
	return packet;
}

Packet PacketCreator::spawn() {
	Packet packet;
	packet.addHeader(HEADER_SPAWN);
	packet.finalize();
	
	return packet;
}

Packet PacketCreator::move(double x, double y, int direction, bool moving) {
	Packet packet;
	packet.addHeader(HEADER_MOVE);
	packet.addBool(moving);
	packet.addFloat(x);
	packet.addFloat(y);
	packet.addInt(direction);
	packet.finalize();
	
	return packet;
}
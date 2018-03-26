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
#include "Game.h"
#include "Log.h"
#include "PacketCreator.h"

using namespace std;

void Game::process(Packet& packet) {
	auto header = packet.getByte();
	
	current_packet_ = &packet;
	
	switch (header) {
		case HEADER_UNKNOWN: Log(NETWORK) << "Server did not recognize our packet " << header << endl;
			break;
			
		default: Log(NETWORK) << "Unknown packet header " << header << endl;
	}
}

void Game::logic() {
}
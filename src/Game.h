#ifndef GAME_H
#define GAME_H

#include "Packet.h"

class Game {
public:
	void process(Packet& packet);
	void logic();
	
private:
	Packet* current_packet_;
};

#endif
#ifndef GAME_H
#define GAME_H

#include "Packet.h"
#include "Map.h"

#include <deque>

class Game {
public:
	Game();
	
	void process(Packet& packet);
	void logic();
	
	void render(sf::RenderWindow& window);
	void processRenderQueue();
	
private:
	
	void handleSpawn();
	
	Packet* current_packet_;
	Map current_map_;
	
	std::deque<std::pair<unsigned char, Packet>> render_queue_;
};

#endif
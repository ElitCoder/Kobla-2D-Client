#ifndef GAME_H
#define GAME_H

#include "Packet.h"
#include "Map.h"
#include "Player.h"

#include <deque>

class Game {
public:
	Game();
	
	void process(Packet& packet);
	void logic();
	void input(sf::Event& event, bool text, bool release);
	
	void render(sf::RenderWindow& window);
	void processRender(unsigned char header);
	void processRenderQueue();
	
private:
	void handleSpawn();
	
	Map map_;
	Player player_;
	
	Packet* current_packet_;
	std::deque<std::pair<unsigned char, Packet>> render_queue_;
	
	// Put these in a config somewhere, or server-sided
	double moving_speed_;
};

#endif
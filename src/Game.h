#ifndef GAME_H
#define GAME_H

#include "Packet.h"
#include "Map.h"
#include "Player.h"

#include <deque>

// How to handle key presses
enum {
	GAME_STATUS_INGAME,
	GAME_STATUS_LOGINSCREEN,
	GAME_STATUS_NONE
};

class Game {
public:
	Game();
	
	void process(Packet& packet);
	void logic();
	bool input(sf::Event& event);
	
	void render(sf::RenderWindow& window);
	void processRender(unsigned char header);
	void processRenderQueue();
	
	void pause();
	void resume();
	
	sf::Vector2u getMapSize();
	
private:
	void removeCharacter(int id);
	void setGameStatus(int status);
	int getGameStatus();
	
	void handleSpawn();
	void handleMove();
	void handleAddPlayer();
	void handleRemove();
	
	Map map_;
	Player player_;
	
	std::vector<Player> players_;
	
	Packet* current_packet_;
	std::deque<std::pair<unsigned char, Packet>> render_queue_;
	
	// Current game status, see enum above
	int game_status_;
	bool paused_;
};

#endif
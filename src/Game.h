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
	void logic(sf::Clock& frame_clock);
	bool input(sf::Event& event);
	
	void render(sf::RenderWindow& window);
	void processRender(unsigned char header);
	void processRenderQueue();
	
	void pause();
	void resume();
	
	bool isCollision(const sf::FloatRect& bound, Character& moving_player);
	Map& getMap();
	
	int getGameStatus();
	
private:
	Character* getCharacter(int id);
	
	void removeCharacter(int id);
	void setGameStatus(int status);
	
	void handleSpawn();
	void handleMove();
	void handleAddPlayer();
	void handleRemove();
	void handleUpdateHealth();
	
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
#pragma once
#ifndef GAME_H
#define GAME_H

//#include "Packet.h"
#include "Map.h"
#include "Character.h"
#include "TemporaryObject.h"

#include <vector>

enum {
	OBJECT_CLOSE_DISTANCE = 100
};

// How to handle key presses
enum {
	GAME_STATUS_INGAME,
	GAME_STATUS_LOGINSCREEN,
	GAME_STATUS_NONE
};

class Packet;
class Object;
class TemporaryObject;

class Game {
public:
	Game();
	
	void process(Packet& packet);
	void logic(sf::Clock& frame_clock);
	bool input(sf::Event& event);
	
	void render(sf::RenderWindow& window);
	
	void pause();
	void resume();
	
	bool isCollision(const sf::FloatRect& bound, Object& object);
	Map& getMap();
	
	int getGameStatus();
	
private:
	Character* getCharacter(int id);
	Object* getActivateObject(Object* character);
	
	void removeCharacter(int id);
	void setGameStatus(int status);
	
	void removeObjects(const std::vector<int>& ids);
	
	void handleSpawn();
	void handleMove();
	void handleAddPlayer();
	void handleRemove();
	void handleUpdateHealth();
	void handleShoot();
	void handleText();
	
	Map map_;
	Character player_;
	
	std::vector<Character> players_;
	std::vector<TemporaryObject> objects_;
	
	Packet* current_packet_;
	
	// Current game status, see enum above
	int game_status_;
	bool paused_;
};

#endif
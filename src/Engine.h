#ifndef ENGINE_H
#define ENGINE_H

#include "Entity.h"

#define NORMAL_FONT	("CAPUT_TRIAL.ttf")

// How to handle key presses
enum {
	GAME_STATUS_INGAME,
	GAME_STATUS_LOGINSCREEN,
	GAME_STATUS_NONE
};

class Engine {
public:
	Engine();
	~Engine();
	
	Engine(const Engine& engine) = delete;
	Engine(const Engine&& engine) = delete;
	
	bool operator=(const Engine& engine) = delete;
	bool operator=(const Engine&& engine) = delete;
	
	void start();
	void load();
	
	bool running();
	void render();
	
	sf::Texture& getTexture(const std::string& filename);
	sf::Font& getFont(const std::string& filename);
	
	std::string getMapName(int id);
	std::string getTextureName(int id);
	
	std::string getTexturePath();
	std::string getFontPath();
	
	int getGameStatus();
	void setGameStatus(int status);
	
private:
	void checkEventNotHandled(sf::Event& type);
	
	bool checkEventStatusNone(sf::Event& event);
	bool checkEventStatusLoginScreen(sf::Event& event);
	bool checkEventStatusIngame(sf::Event& event);
	
	sf::RenderWindow window_;
	
	std::vector<std::pair<std::string, sf::Texture>> textures_;
	std::vector<std::pair<std::string, sf::Font>> fonts_;
	
	std::vector<std::pair<int, std::string>> map_names_;
	std::vector<std::pair<int, std::string>> texture_names_;
	
	int game_status_;
};

#endif
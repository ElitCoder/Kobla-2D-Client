#ifndef ENGINE_H
#define ENGINE_H

#include "Entity.h"

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
	
private:
	sf::RenderWindow window_;
	
	std::vector<std::pair<std::string, sf::Texture>> textures_;
	std::vector<std::pair<std::string, sf::Font>> fonts_;
	std::vector<std::pair<int, std::string>> maps_;
};

#endif
#pragma once
#ifndef ENGINE_H
#define ENGINE_H

#include <SFML/Graphics/RenderWindow.hpp>

#include <string>
#include <vector>

class ObjectInformation;

namespace sf {
	class Texture;
	class Font;
	class Event;
	class RenderWindow;
}

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
	
	sf::Texture* getTexture(int id);
	sf::Font* getFont(const std::string& filename);
	ObjectInformation& getObjectInformation(int id);
	
	std::string getMapName(int id);
	
private:
	std::string getTextureName(int id);
	std::string getObjectInformationName(int id);
	
	std::string getTexturePath();
	std::string getFontPath();
	std::string getCharacterPath();
	
	void checkEventNotHandled(sf::Event& type);
	
	sf::RenderWindow window_;
	
	std::vector<std::pair<std::string, sf::Texture*>> textures_;
	std::vector<std::pair<std::string, sf::Font*>> fonts_;
	std::vector<std::pair<std::string, ObjectInformation>> objects_;
	
	std::vector<std::pair<int, std::string>> map_names_;
	std::vector<std::pair<int, std::string>> texture_names_;
	std::vector<std::pair<int, std::string>> object_names_;
};

#endif
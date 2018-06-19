#pragma once
#ifndef ENGINE_H
#define ENGINE_H

#include <SFML/Graphics/RenderWindow.hpp>

#include <string>
#include <vector>
#include <memory>

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
	
	std::shared_ptr<sf::Texture>& getTexture(int id);
	std::shared_ptr<sf::Font>& getFont(const std::string& filename);
	ObjectInformation& getObjectInformation(int id);
	
	std::string getMapName(int id);
	
	void runWarm();
	
private:
	std::string getTextureName(int id);
	std::string getObjectInformationName(int id);
	
	std::string getTexturePath();
	std::string getFontPath();
	std::string getCharacterPath();
	
	void checkEventNotHandled(sf::Event& type);
	
	sf::RenderWindow window_;
	
	std::vector<std::pair<std::string, std::shared_ptr<sf::Texture>>> textures_;
	std::vector<std::pair<std::string, std::shared_ptr<sf::Font>>> fonts_;
	std::vector<std::pair<std::string, ObjectInformation>> objects_;
	
	std::vector<std::pair<int, std::string>> map_names_;
	std::vector<std::pair<int, std::string>> texture_names_;
	std::vector<std::pair<int, std::string>> object_names_;
};

#endif
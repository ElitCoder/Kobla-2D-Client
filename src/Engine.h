#ifndef ENGINE_H
#define ENGINE_H

#include "Entity.h"

#define NORMAL_FONT		("CAPUT_TRIAL.ttf")
#define NORMAL_FONT_ID	(0)

class CharacterInformation;

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
	CharacterInformation& getCharacterInformation(int id);
	
	std::string getMapName(int id);
	
private:
	std::string getTextureName(int id);
	std::string getCharacterInformationName(int id);
	
	std::string getTexturePath();
	std::string getFontPath();
	std::string getCharacterPath();
	
	void checkEventNotHandled(sf::Event& type);
	
	sf::RenderWindow window_;
	
	std::vector<std::pair<std::string, sf::Texture*>> textures_;
	std::vector<std::pair<std::string, sf::Font*>> fonts_;
	std::vector<std::pair<std::string, CharacterInformation>> characters_;
	
	std::vector<std::pair<int, std::string>> map_names_;
	std::vector<std::pair<int, std::string>> texture_names_;
	std::vector<std::pair<int, std::string>> character_names_;
};

#endif
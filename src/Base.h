#pragma once
#ifndef BASE_H
#define BASE_H

class Config;
class NetworkCommunication;
class Game;
class Engine;
class GUI;

namespace sf {
	class RenderWindow;
}

class Base {
public:
	static Config& settings();
	static NetworkCommunication& network();
	static Game& game();
	static Engine& engine();
	static GUI& gui();
	
	static void createGUI(sf::RenderWindow& window);
	static void destroyGUI();
	
private:
	static Config settings_;
	static NetworkCommunication network_;
	static Game game_;
	static Engine engine_;
	
	// RenderWindow has to be passed into the GUI at construction time
	static GUI* gui_;
};

#endif
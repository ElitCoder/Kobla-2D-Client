#ifndef BASE_H
#define BASE_H

#include "Config.h"
#include "NetworkCommunication.h"
#include "Game.h"
#include "Engine.h"
#include "GUI.h"

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
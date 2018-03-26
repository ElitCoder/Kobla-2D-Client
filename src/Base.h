#ifndef BASE_H
#define BASE_H

#include "Config.h"
#include "NetworkCommunication.h"
#include "Game.h"
#include "Engine.h"

class Base {
public:
	static Config& settings();
	static NetworkCommunication& network();
	static Game& game();
	static Engine& engine();
	
private:
	static Config settings_;
	static NetworkCommunication network_;
	static Game game_;
	static Engine engine_;
};

#endif
#include "Base.h"
#include "Config.h"
#include "NetworkCommunication.h"
#include "Game.h"
#include "Engine.h"
#include "GUI.h"

using namespace std;

Config Base::settings_;
NetworkCommunication Base::network_;
Game Base::game_;
Engine Base::engine_;
shared_ptr<GUI> Base::gui_;

Config& Base::settings() {
	return settings_;
}

NetworkCommunication& Base::network() {
	return network_;
}

Game& Base::game() {
	return game_;
}

Engine& Base::engine() {
	return engine_;
}

GUI& Base::gui() {
	return *gui_;
}

void Base::createGUI(sf::RenderWindow& window) {
	gui_ = make_shared<GUI>(window);
	
	gui_->load(0);
}
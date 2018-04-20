#include "Base.h"
#include "Config.h"
#include "NetworkCommunication.h"
#include "Game.h"
#include "Engine.h"
#include "GUI.h"

Config Base::settings_;
NetworkCommunication Base::network_;
Game Base::game_;
Engine Base::engine_;
GUI* Base::gui_ = nullptr;

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
	gui_ = new GUI(window);
	
	gui_->load(0);
}

void Base::destroyGUI() {
	if (gui_ == nullptr)
		return;
		
	delete gui_;	
}
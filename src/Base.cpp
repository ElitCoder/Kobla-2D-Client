#include "Base.h"

Config Base::settings_;
NetworkCommunication Base::network_;
Game Base::game_;
Engine Base::engine_;

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
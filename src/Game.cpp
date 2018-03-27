#include "Game.h"
#include "Log.h"
#include "PacketCreator.h"
#include "Base.h"

using namespace std;

Game::Game() :
	map_() {
}
	
void Game::process(Packet& packet) {
	auto header = packet.getByte();
	
	current_packet_ = &packet;
	
	switch (header) {
		case HEADER_UNKNOWN: Log(NETWORK) << "Server did not recognize our packet " << header << endl;
			break;
			
		default: render_queue_.push_back({ header, packet });
	}
}

void Game::processRender(unsigned char header) {
	switch (header) {
		case HEADER_SPAWN: handleSpawn();
			break;
			
		default: Log(NETWORK) << "Unknown packet header " << header << endl;
	}	
}

void Game::processRenderQueue() {
	while (!render_queue_.empty()) {
		auto header = render_queue_.front().first;
		auto& packet = render_queue_.front().second;
		
		current_packet_ = &packet;
		processRender(header);
		
		render_queue_.pop_front();
	}
}

void Game::logic() {
	// Go through rendering packets before doing logic
	processRenderQueue();
}

void Game::render(sf::RenderWindow& window) {
	// Render map first
	map_.draw(window);
	player_.draw(window);
}

void Game::handleSpawn() {
	int map_id = current_packet_->getInt();
	int player_image_id = current_packet_->getInt();
	
	int x = current_packet_->getInt();
	int y = current_packet_->getInt();
	string name = current_packet_->getString();
	
	// Load map
	map_.load(Base::engine().getMapName(map_id));
	
	// Load player
	player_.load(Base::engine().getTextureName(player_image_id));
	player_.setName(name);
	player_.setPosition(x, y);
}
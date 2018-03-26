#include "Game.h"
#include "Log.h"
#include "PacketCreator.h"
#include "Base.h"

using namespace std;

Game::Game() :
	current_map_() {
	Log(DEBUG) << "Running constructor\n";
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

void Game::processRenderQueue() {
	while (!render_queue_.empty()) {
		auto header = render_queue_.front().first;
		auto& packet = render_queue_.front().second;
		
		current_packet_ = &packet;
		
		switch (header) {
			case HEADER_SPAWN: handleSpawn();
				break;
				
			default: Log(NETWORK) << "Unknown packet header " << header << endl;
		}
		
		render_queue_.pop_front();
	}
}

void Game::logic() {
}

void Game::render(sf::RenderWindow& window) {
	// Render map first
	current_map_.draw(window);
}

void Game::handleSpawn() {
	int id = current_packet_->getInt();
	
	current_map_.load(Base::engine().getMapName(id));
}
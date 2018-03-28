#include "Game.h"
#include "Log.h"
#include "PacketCreator.h"
#include "Base.h"

using namespace std;

Game::Game() :
	map_() {
	game_status_ = GAME_STATUS_NONE;
	paused_ = false;
}
	
void Game::process(Packet& packet) {
	auto header = packet.getByte();
	
	current_packet_ = &packet;
	
	switch (header) {
		case HEADER_UNKNOWN: Log(NETWORK) << "Server did not recognize our packet " << header << endl;
			break;
			
		case HEADER_PING_ALIVE: // Ignore this, just for testing
			break;
			
		case HEADER_MOVE: handleMove();
			break;
			
		case HEADER_REMOVE_CHARACTER: handleRemove();
			break;
			
		default: render_queue_.push_back({ header, packet });
	}
}

// Moving packets are more expensive, avoid at all costs
void Game::processRender(unsigned char header) {
	switch (header) {
		case HEADER_SPAWN: handleSpawn();
			break;
			
		case HEADER_ADD_PLAYER: handleAddPlayer();
			break;
			
		default: Log(NETWORK) << "Unknown packet header " << header << endl;
	}	
}

void Game::setGameStatus(int status) {
	game_status_ = status;
}

int Game::getGameStatus() {
	return game_status_;
}

void Game::logic() {
	// Go through rendering packets before doing logic
	processRenderQueue();
	
	// See if we're moving
	player_.move();
	
	for (auto& player : players_)
		player.move();
}

void Game::render(sf::RenderWindow& window) {
	// Change view to put player in middle
	sf::View view;
	auto x = player_.getMiddleX();
	auto y = player_.getMiddleY();
	
	// Check if the player is in the corners
	if (x < window.getSize().x / 2)
		x = window.getSize().x / 2;
		
	if (y < window.getSize().y / 2)
		y = window.getSize().y / 2;
	
	if (x > getMapSize().x - window.getSize().x / 2)
		x = getMapSize().x - window.getSize().x / 2;
		
	if (y > getMapSize().y - window.getSize().y / 2)
		y = getMapSize().y - window.getSize().y / 2;
	
	view.setCenter(x, y);
	view.setSize(window.getSize().x, window.getSize().y);
	window.setView(view);
	
	// Render map first
	map_.draw(window);
	
	// Render other player before own player
	for (auto& player : players_)
		player.draw(window);
		
	player_.draw(window);
}

// Return false if the event was not handled
bool Game::input(sf::Event& event) {
	// Ignore input if the game is paused/lost focus
	if (paused_)
		return false;
		
	bool handled = true;
	
	if (getGameStatus() == GAME_STATUS_INGAME) {
		// Enable key presses for movement etc
		int direction = -1;
		
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
			direction = PLAYER_MOVE_LEFT;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
			direction = PLAYER_MOVE_RIGHT;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
			direction = PLAYER_MOVE_UP;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
			direction = PLAYER_MOVE_DOWN;
			
		if (direction < 0)
			player_.stopMoving(true);
		else
			player_.startMoving(direction, true);
			
		switch (event.type) {
			default: handled = false;
		}
	} else if (getGameStatus() == GAME_STATUS_NONE) {
		// Loading screen, disable everything
		switch (event.type) {
			default: handled = false;
		}
	} else if (getGameStatus() == GAME_STATUS_LOGINSCREEN) {
		// Enable text input
		switch (event.type) {
			default: handled = false;
		}
	}
	
	return handled;
}

void Game::pause() {
	paused_ = true;
}

void Game::resume() {
	paused_ = false;
}

void Game::removeCharacter(int id) {
	auto is_player = remove_if(players_.begin(), players_.end(), [&id] (auto& player) { return player.getID() == (unsigned)id; });
	
	if (is_player != players_.end())
		players_.erase(is_player);
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

sf::Vector2u Game::getMapSize() {
	return map_.getSize();
}

void Game::handleSpawn() {
	auto map_id = current_packet_->getInt();
	auto player_image_id = current_packet_->getInt();
	
	auto x = current_packet_->getFloat();
	auto y = current_packet_->getFloat();
	auto name = current_packet_->getString();
	auto moving_speed = current_packet_->getFloat();
	
	// Load map
	map_.load(Base::engine().getMapName(map_id));
	
	// Load player
	player_.load(Base::engine().getTextureName(player_image_id));
	player_.setName(name);
	player_.setPosition(x, y);
	player_.setMovingSpeed(moving_speed);
	
	// Tell Game we're ingame to enable event handler properly
	setGameStatus(GAME_STATUS_INGAME);
}

void Game::handleMove() {
	auto moving = current_packet_->getBool();
	auto x = current_packet_->getFloat();
	auto y = current_packet_->getFloat();
	auto direction = current_packet_->getInt();
	auto id = current_packet_->getInt();
	
	// It's a player?
	for (auto& player : players_) {
		if (player.getID() != (unsigned int)id)
			continue;
			
		player.setPosition(x, y);	
			
		if (moving)
			player.startMoving(direction, false);
		else
			player.stopMoving(false);
			
		break;
	}
}

void Game::handleAddPlayer() {
	auto id = current_packet_->getInt();
	auto moving = current_packet_->getBool();
	auto direction = current_packet_->getInt();
	auto texture_id = current_packet_->getInt();
	auto x = current_packet_->getFloat();
	auto y = current_packet_->getFloat();
	auto name = current_packet_->getString();
	auto moving_speed = current_packet_->getFloat();
	
	Player player;
	player.load(Base::engine().getTextureName(texture_id));
	
	player.setName(name);
	player.setID(id);
	player.setPosition(x, y);
	player.setMovingSpeed(moving_speed);
	
	if (moving)
		player.startMoving(direction, false);
	else
	 	player.stopMoving(false);
	
	players_.push_back(player);
}

void Game::handleRemove() {
	auto id = current_packet_->getInt();
	
	removeCharacter(id);
}
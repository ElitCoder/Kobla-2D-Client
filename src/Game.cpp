#include "Game.h"
#include "Log.h"
#include "PacketCreator.h"
#include "Base.h"
#include "Packet.h"
#include "TemporaryObject.h"
#include "NetworkCommunication.h"
#include "GUI.h"

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
			
		case HEADER_SPAWN: handleSpawn();
			break;
			
		case HEADER_ADD_PLAYER: handleAddPlayer();
			break;
			
		case HEADER_UPDATE_HEALTH: handleUpdateHealth();
			break;
			
		case HEADER_SHOOT: handleShoot();
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

void Game::logic(sf::Clock& frame_clock) {
	// Set frame time for animations
	auto frame_time = frame_clock.restart();
	
	// See if we're moving
	if (player_.move(frame_time))
		player_.updatePosition();
	
	for (auto& player : players_)
		if (player.move(frame_time))
			player_.updatePosition();
	
	// Move Objects
	vector<int> remove_objects_ids;
	
	for (auto& object : objects_)
		if (!object.move(frame_time))
			remove_objects_ids.push_back(object.getID());
			
	removeObjects(remove_objects_ids);		
}

void Game::render(sf::RenderWindow& window) {
	// Change view to put player in middle
	sf::View view;
	auto x = player_.getMiddleX();
	auto y = player_.getMiddleY();
	
	// Check if the player is in the corners
	auto window_size = window.getSize();
	auto map_size = getMap().getSize();
	
	if (x < window_size.x / 2)
		x = window_size.x / 2;
		
	if (y < window_size.y / 2)
		y = window_size.y / 2;
	
	if (x > map_size.x - window_size.x / 2)
		x = map_size.x - window_size.x / 2;
		
	if (y > map_size.y - window_size.y / 2)
		y = map_size.y - window_size.y / 2;
	
	view.setCenter(x, y);
	view.setSize(window_size.x, window_size.y);
	window.setView(view);
	
	// Render map first
	map_.draw(window);
	
	// Render objects
	for (auto& object : objects_)
		object.draw(window);
		
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
		
		// Movement
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
			case sf::Event::KeyPressed: {
				if (event.key.code == sf::Keyboard::S) {
					// It's only possible to shoot if we're not moving
					if (!player_.isMoving())
						// Send packet that we're shooting
						Base::network().send(PacketCreator::shoot());
				}
				
				break;
			}
			
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

void Game::removeObjects(const vector<int>& ids) {
	if (ids.empty())
		return;

	objects_.erase(remove_if(objects_.begin(), objects_.end(), [&ids] (auto& object) { 
		return find(ids.begin(), ids.end(), object.getID()) != ids.end();
	}), objects_.end());
}

bool Game::isCollision(const sf::FloatRect& bound, Object& moving_player) {
	if (moving_player.getCollision(COLLISION_MAP))
		if (map_.isCollision(bound))
			return true;
	
	// Check if our own player is colliding
	if (moving_player.getCollision(COLLISION_PLAYERS))
		if (moving_player.getID() != player_.getID())
			if (player_.isCollision(bound))
				return true;
		
	// Check other collision players
	for (auto& player : players_) {
		if (player.getID() == moving_player.getID())
			continue;
			
		if (!moving_player.getCollision(Object::translateObjectTypeToCollision(player.getObjectType())) && !player.isColliding())
			continue;

		if (player.isCollision(bound))
			return true;
	}		
	
	return false;
}

Map& Game::getMap() {
	return map_;
}

static vector<bool> readCollisionInformation(Packet& packet) {
	vector<bool> collisions;
	auto collisions_size = packet.getInt();
	
	for (int i = 0; i < collisions_size; i++)
		collisions.push_back(packet.getBool());
		
	return collisions;
}

static void readSpawnPlayer(Character& player, Packet& packet) {
	auto id = packet.getInt();
	auto player_image_id = packet.getInt();
	auto x = packet.getFloat();
	auto y = packet.getFloat();
	auto name = packet.getString();
	auto moving_speed = packet.getFloat();
	auto collisions = readCollisionInformation(packet);
	auto colliding = packet.getBool();
	auto full_health = packet.getFloat();
	auto current_health = packet.getFloat();
	auto object_type = packet.getInt();
	
	// Load player
	player.load(player_image_id);
	player.setName(name);
	player.setPosition(x, y);
	player.setMovingSpeed(moving_speed);
	player.setCollisions(collisions);
	player.setColliding(colliding);
	player.setID(id);
	player.setHealth(full_health, current_health);
	player.setObjectType(object_type);
}

void Game::handleSpawn() {
	auto map_id = current_packet_->getInt();
	
	// Load map
	map_.load(map_id);
	
	// Load player
	readSpawnPlayer(player_, *current_packet_);
	
	// Tell Game we're ingame to enable event handler properly
	setGameStatus(GAME_STATUS_INGAME);
	
	// Set health bar to represent the player's health
	Base::gui().updateHealthBar(player_.getFullHealth(), player_.getCurrentHealth());
}

void Game::handleMove() {
	auto moving = current_packet_->getBool();
	auto x = current_packet_->getFloat();
	auto y = current_packet_->getFloat();
	auto direction = current_packet_->getInt();
	auto id = current_packet_->getInt();
	auto predetermined_distance = current_packet_->getFloat();
	
	// It's a player?
	for (auto& player : players_) {
		if (player.getID() != (unsigned int)id)
			continue;
			
		player.setPosition(x, y);	
			
		if (moving) {
			player.startMoving(direction, false);
			player.setPredeterminedDistance(predetermined_distance);
		} else {
			player.stopMoving(false);
		}
			
		break;
	}
}

void Game::handleAddPlayer() {
	auto moving = current_packet_->getBool();
	auto direction = current_packet_->getInt();
	
	players_.emplace_back();
	auto& player = players_.back();
	
	readSpawnPlayer(player, *current_packet_);
	
	player.setDirection(direction);
	
	if (moving)
		player.startMoving(direction, false);
	else
	 	player.stopMoving(false);
}

void Game::handleRemove() {
	auto id = current_packet_->getInt();
	
	removeCharacter(id);
	removeObjects({ id });
}

Character* Game::getCharacter(int id) {
	if (player_.getID() == (unsigned int)id)
		return &player_;
		
	auto iterator = find_if(players_.begin(), players_.end(), [&id] (auto& player) {
		return player.getID() == (unsigned int)id;
	});
	
	if (iterator == players_.end())
		Log(WARNING) << "Character not found: " << id << endl;
		
	return &*iterator;
}

void Game::handleUpdateHealth() {
	auto id = current_packet_->getInt();
	auto full = current_packet_->getFloat();
	auto current = current_packet_->getFloat();
	
	Character* character = getCharacter(id);
	character->setHealth(full, current);
	
	if (character->getID() == player_.getID())
		Base::gui().updateHealthBar(full, current);
}

void Game::handleShoot() {
	auto direction = current_packet_->getInt();
	auto speed = current_packet_->getFloat();
	auto object_type = current_packet_->getInt();
	auto id = current_packet_->getInt();
	auto x = current_packet_->getFloat();
	auto y = current_packet_->getFloat();
	auto collisions = readCollisionInformation(*current_packet_);
	
	objects_.emplace_back();
	auto& bullet = objects_.back();
	
	bullet.load(object_type);
	bullet.setID(id);
	bullet.setPosition(x, y);
	bullet.setType(TEMP_OBJECT_BULLET);
	bullet.setCollisions(collisions);
	bullet.setMovingSpeed(speed);
	bullet.startMoving(direction, false);
}
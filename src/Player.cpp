#include "Player.h"
#include "Engine.h"
#include "Log.h"

using namespace std;

void Player::draw(sf::RenderWindow& window) {
	image_.draw(window);
	text_.draw(window);
}

void Player::load(const string& filename) {
	image_.load(filename);
	text_.load(NORMAL_FONT);
}

void Player::setName(const string& name) {
	name_ = name;
	
	// Update display text
	text_.set(name);
	text_.size(24);
	text_.color(sf::Color::White);
}

void Player::setPosition(double x, double y) {
	// Get size of player image and text
	auto image_size = image_.getSize();
	auto text_size = text_.getSize();
	
	// Set text to centered of player image
	int text_x = image_size.width / 2 - text_size.width / 2;
	int text_y = -text_size.height;
	
	text_.position(x + text_x, y + text_y);
	image_.position(x, y);
		
	x_ = x;
	y_ = y;
	
	//Log(DEBUG) << "Position " << x << " " << y << endl;
}

// Start moving player in one direction
void Player::startMoving(int direction, bool tell_server) {
	// TODO: Tell the server we're moving IF IT'S THE REAL PLAYER
	if (tell_server) {
		// TODO: Not implemented
	}
	
	moving_ = true;
	direction_ = direction;
	
	started_moving_.start();
}

void Player::stopMoving(bool tell_server) {
	// TODO: Tell the server we're NOT moving IF IT'S THE REAL PLAYER
	if (tell_server) {
		// TODO: Not implemented
	}
	
	moving_ = false;
}

void Player::move(double speed) {
	if (!moving_)
		return;
		
	auto time_elapsed = started_moving_.elapsed();
	started_moving_.start();
	
	// In ms
	double pixels = speed * time_elapsed;
	pixels /= 1000;
	
	//Log(DEBUG) << "Moved " << pixels << endl;
	
	double x = x_;
	double y = y_;
	
	switch (direction_) {
		case PLAYER_MOVE_UP: y -= pixels;
			break;
			
		case PLAYER_MOVE_DOWN: y += pixels;
			break;
			
		case PLAYER_MOVE_LEFT: x -= pixels;
			break;
			
		case PLAYER_MOVE_RIGHT: x += pixels;
			break;
	}
	
	setPosition(x, y);
}
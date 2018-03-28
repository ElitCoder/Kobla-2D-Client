#include "Character.h"
#include "PacketCreator.h"
#include "Engine.h"
#include "Base.h"

using namespace std;

Character::Character() {
	moving_ = false;
}

Character::~Character() {}

void Character::draw(sf::RenderWindow& window) {
	image_.draw(window);
	text_.draw(window);
}

void Character::load(const string& filename) {
	image_.load(filename);
	text_.load(NORMAL_FONT);
}

void Character::setName(const string& name) {
	name_ = name;
	
	// Update display text
	text_.set(name);
	text_.size(24);
	text_.color(sf::Color::White);
}

void Character::setPosition(double x, double y) {
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
void Character::startMoving(int direction, bool tell_server) {
	// Just change direction
	if (moving_ && direction_ == direction)
		return;
		
	// TODO: Tell the server we're moving IF IT'S THE REAL PLAYER
	if (tell_server) {
		auto packet = PacketCreator::move(x_, y_, direction, true);
		Base::network().send(packet);
	}
	
	moving_ = true;
	direction_ = direction;
	
	started_moving_.start();
}

void Character::stopMoving(bool tell_server) {
	if (!moving_)
		return;
		
	// TODO: Tell the server we're NOT moving IF IT'S THE REAL PLAYER
	if (tell_server) {
		auto packet = PacketCreator::move(x_, y_, -1, false);
		Base::network().send(packet);
	}
	
	moving_ = false;
}

void Character::move() {
	if (!moving_)
		return;
	
	auto time_elapsed = started_moving_.restart();
	double pixels = moving_speed_ * time_elapsed;
	
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

void Character::setID(size_t id) {
	id_ = id;
}

size_t Character::getID() const {
	return id_;
}

void Character::setMovingSpeed(double speed) {
	moving_speed_ = speed;
}
#include "Character.h"
#include "PacketCreator.h"
#include "Engine.h"
#include "Base.h"
#include "Log.h"

using namespace std;

Character::Character() {
	moving_ = false;
	collision_ = false;
}

Character::~Character() {}

void Character::draw(sf::RenderWindow& window) {
	image_.draw(window);
	text_.draw(window);
}

void Character::load(const string& filename) {
	image_.load(filename);
	text_.load(NORMAL_FONT);
	
	// TODO: Remove this later
	image_.scale(0.65);
}

void Character::setName(const string& name) {
	name_ = name;
	
	// Update display text
	text_.set(name);
	text_.size(20);
	text_.color(sf::Color::White);
}

void Character::setCollision(bool collision) {
	collision_ = collision;
}

void Character::setPosition(double x, double y) {
	// Get size of player image and text
	auto image_size = image_.getSize();
	auto text_size = text_.getSize();
	
	// Set text to centered of player image
	int text_x = image_size.width / 2 - text_size.width / 2;
	int text_y = -text_size.height - 8;
	
	text_.position(x + text_x, y + text_y);
	image_.position(x, y);
		
	x_ = x;
	y_ = y;
	
	//Log(DEBUG) << "Setting position to " << x_ << " " << y_ << endl;
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
	
	if (!isPlayerInsideMap(x, y)) {
		switch (direction_) {
			case PLAYER_MOVE_UP: y = 0;
				break;
				
			case PLAYER_MOVE_DOWN: y = getMaxY();
				break;
				
			case PLAYER_MOVE_LEFT: x = 0;
				break;
				
			case PLAYER_MOVE_RIGHT: x = getMaxX();
				break;
		}
	}
	
	// Test new position with collisions
	image_.position(x, y);
	
	// Only use boots for collision detection
	auto rect = image_.internal().getGlobalBounds();
	auto body = (double)rect.height * 0.7;
	auto boots = (double)rect.width * 0.5;
	
	rect.top += body;
	rect.height -= body;
	rect.left += boots / 2;
	rect.width -= boots;
	
	if (Base::game().isCollision(rect, *this)) {
		// Did not work, revert to old values
		image_.position(x_, y_);
		return;
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

double Character::getX() const {
	return x_;
}

double Character::getY() const {
	return y_;
}

// Used for placing the character in middle of the screen during rendering
double Character::getMiddleX() {
	// Get size of player image and text
	auto image_size = image_.getSize();

	return x_ + image_size.width / 2;
}

double Character::getMiddleY() {
	// Get size of player image and text
	auto image_size = image_.getSize();

	return y_ + image_size.height / 2;
}

// Are the new coordinates x and y inside of the map?
bool Character::isPlayerInsideMap(double x, double y) {
	// Don't move outside of the map
	if (x < 0 || y < 0)
		return false;
		
	if (x > getMaxX() || y > getMaxY())
		return false;
		
	return true;
}

double Character::getMaxX() {
	auto image_size = image_.getSize();
	
	return Base::game().getMap().getSize().x - image_size.width;
}

double Character::getMaxY() {
	auto image_size = image_.getSize();
	
	return Base::game().getMap().getSize().y - image_size.height;
}

bool Character::isCollision(Character& character) {
	if (!collision_)
		return false;
		
	return character.image_.internal().getGlobalBounds().intersects(image_.internal().getGlobalBounds());
}
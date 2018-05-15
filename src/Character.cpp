#include "Character.h"
#include "PacketCreator.h"
#include "Engine.h"
#include "Base.h"
#include "Log.h"

using namespace std;

void Character::setHealth(double full, double current) {
	full_health_ = full;
	current_health_ = current;
	
	health_bar_.setSize({ image_.internal().getGlobalBounds().width, 4 });
	health_bar_.setFillColor(sf::Color::White);
	health_bar_.setOutlineColor(sf::Color::Black);
	health_bar_.setOutlineThickness(1);
	
	health_bar_fill_.setOutlineThickness(0);
	health_bar_fill_.setFillColor(sf::Color::Red);
	
	double fill = current / full;
	fill *= health_bar_.getSize().x;
	
	health_bar_fill_.setSize({ (float)fill, 4 });
}

double Character::getFullHealth() const {
	return full_health_;
}

double Character::getCurrentHealth() const {
	return current_health_;
}

void Character::setName(const string& name) {
	name_ = name;
	
	// Update display text
	text_.set(name);
	text_.size(20);
	text_.color(sf::Color::White);
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

void Character::updatePosition() {
	setPosition(x_, y_);
}

void Character::setPosition(double x, double y) {
	// Get size of player image and text
	auto image_size = image_.getSize();
	auto text_size = text_.getSize();
	
	// Set text to centered of player image
	int text_x = image_size.width / 2 - text_size.width / 2;
	int text_y = -text_size.height - 8 - health_bar_.getSize().y;
	int health_y = -health_bar_.getSize().y;
	
	text_.position(x + text_x, y + text_y);
	image_.position(x, y);
	health_bar_.setPosition(x, y + health_y);
	health_bar_fill_.setPosition(health_bar_.getPosition());
		
	x_ = x;
	y_ = y;
}

void Character::setShowHealth(bool status) {
	show_health_ = status;
}

void Character::draw(sf::RenderWindow& window) {
	image_.draw(window);
	text_.draw(window);
	
	if (show_health_) {
		window.draw(health_bar_);
		window.draw(health_bar_fill_);
	}
}

void Character::load(int id) {
	// filename is the character ID
	image_.load(id);
	text_.load(NORMAL_FONT_ID);
	
	image_.scale(Base::engine().getObjectInformation(id).getScale());
	
	object_id_ = id;
}
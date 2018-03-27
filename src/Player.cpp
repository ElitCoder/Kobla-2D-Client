#include "Player.h"
#include "Engine.h"

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

void Player::setPosition(int x, int y) {
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
}
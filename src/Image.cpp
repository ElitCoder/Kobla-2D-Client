#include "Image.h"
#include "Base.h"
#include "Log.h"

using namespace std;

void Image::load(int id) {
	sprite_ = AnimatedSprite(sf::seconds(0.2), true, true);
	texture_id_ = id;
	
	// Set animation to standard so the text will be aligned
	setAnimation(PLAYER_MOVE_RIGHT);
}

void Image::draw(sf::RenderWindow& window) {
	if (!loaded_)
		return;
		
	window.draw(sprite_);
}

sf::FloatRect Image::getSize() {
	if (!loaded_)
		return sf::FloatRect();
		
	return sprite_.getGlobalBounds();
}

void Image::position(int x, int y) {
	sprite_.setPosition(x, y);
}

void Image::scale(double value) {
	sprite_.setScale(value, value);
}

AnimatedSprite& Image::internal() {
	return sprite_;
}

void Image::setAnimation(int direction) {
	if (texture_id_ < 0)
		return;
	
	sprite_.play(Base::engine().getCharacterInformation(texture_id_).getAnimation(direction));
	
	if (!loaded_)
		loaded_ = true;
}
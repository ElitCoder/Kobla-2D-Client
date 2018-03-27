#include "Image.h"
#include "Base.h"

using namespace std;

void Image::load(const string& filename) {
	string path = Base::engine().getTexturePath();
	path += filename;
	
	sprite_.setTexture(Base::engine().getTexture(path));
}

void Image::draw(sf::RenderWindow& window) {
	window.draw(sprite_);
}

sf::FloatRect Image::getSize() {
	return sprite_.getGlobalBounds();	
}

void Image::position(int x, int y) {
	sprite_.setPosition(x, y);
}
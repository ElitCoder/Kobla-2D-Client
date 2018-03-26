#include "Image.h"
#include "Base.h"

using namespace std;

void Image::load(const string& filename) {
	sprite_.setTexture(Base::engine().getTexture(filename));
}

void Image::draw(sf::RenderWindow& window) {
	window.draw(sprite_);
}
#include "Text.h"
#include "Base.h"

using namespace std;

void Text::load(const string& filename) {
	string path = Base::engine().getFontPath();
	path += filename;
	
	text_.setFont(Base::engine().getFont(path));
}

void Text::draw(sf::RenderWindow& window) {
	window.draw(text_);
}

void Text::set(const string& text) {
	text_.setString(text);
}

void Text::size(int pixels) {
	text_.setCharacterSize(pixels);
}

void Text::color(const sf::Color& color) {
	text_.setColor(color);
}

void Text::position(int x, int y) {
	text_.setPosition(x, y);
}

sf::FloatRect Text::getSize() {
	return text_.getGlobalBounds();
}
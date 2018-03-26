#include "Text.h"
#include "Base.h"

using namespace std;

void Text::load(const string& filename) {
	text_.setFont(Base::engine().getFont(filename));
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
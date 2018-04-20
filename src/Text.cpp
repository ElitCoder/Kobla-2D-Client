#include "Text.h"
#include "Base.h"
#include "Engine.h"

using namespace std;

void Text::load(int id) {
	// Ignore warning of not using id
	if (id) {}
	
	text_.setFont(*Base::engine().getFont(NORMAL_FONT));
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
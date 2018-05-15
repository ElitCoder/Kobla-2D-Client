#include "Text.h"
#include "Base.h"
#include "Engine.h"
#include "Game.h"

using namespace std;

void Text::load(int id) {
	// Ignore warning of not using id
	if (id) {}
	
	text_.setFont(*Base::engine().getFont(NORMAL_FONT));
}

void Text::draw(sf::RenderWindow& window) {
	// Update position with owner's position
	if (owner_ >= 0) {
		auto* owner = Base::game().getCharacter(owner_);
		
		// If the owner is dead, don't render the text
		if (owner == nullptr)
			return;
			
		position(owner->getX() + 24, owner->getY() - getSize().height - 24);
	}
	
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

void Text::setOwner(int id) {
	owner_ = id;
}

int Text::getOwner() const {
	return owner_;
}
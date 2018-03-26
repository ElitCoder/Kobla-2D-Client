#ifndef TEXT_H
#define TEXT_H

#include "Entity.h"

class Text : public Entity {
public:
	virtual void load(const std::string& filename) override;
	virtual void draw(sf::RenderWindow& window) override;
	
	void set(const std::string& text);
	void size(int pixels);
	void color(const sf::Color& color);
	
private:
	sf::Text text_;	
};

#endif
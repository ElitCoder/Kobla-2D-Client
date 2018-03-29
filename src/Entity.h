#ifndef ENTITY_H
#define ENTITY_H

// SFML
#include <SFML/Graphics.hpp>
#include <TGUI/TGUI.hpp>

#include <string>

enum {
	ENTITY_TYPE_MAP
};

class Entity {
public:
	virtual ~Entity();
	
	virtual void draw(sf::RenderWindow& window) = 0;
	virtual void load(const std::string& file) = 0;
	
protected:
	Entity();
};

#endif
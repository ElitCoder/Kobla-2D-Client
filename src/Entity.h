#ifndef ENTITY_H
#define ENTITY_H

// SFML
#include <SFML/Graphics.hpp>
#include <TGUI/TGUI.hpp>

#include <string>

class Entity {
public:
	virtual ~Entity();
	
	virtual void draw(sf::RenderWindow& window) = 0;
	virtual void load(int id) = 0;
	
protected:
	Entity();
};

#endif
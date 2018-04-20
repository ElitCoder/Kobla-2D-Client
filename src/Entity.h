#pragma once
#ifndef ENTITY_H
#define ENTITY_H

// SFML
//#include <SFML/Graphics.hpp>
//#include <TGUI/TGUI.hpp>

namespace sf {
	class RenderWindow;
}

/*
	Entity is a class for general drawing and loading, everything is based on entities, e.g Characters, GUI and Objects
*/
class Entity {
public:
	virtual ~Entity();
	
	virtual void draw(sf::RenderWindow& window) = 0;
	virtual void load(int id) = 0;
	
protected:
	Entity();
};

#endif
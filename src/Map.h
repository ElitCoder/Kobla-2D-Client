#pragma once
#ifndef MAP_H
#define MAP_H

#include "Entity.h"

#include <tmx/MapLoader.hpp>

class Map : public Entity {
public:
	Map();
	
	virtual void load(int id) override;
	virtual void draw(sf::RenderWindow& window) override;
	
	sf::Vector2u getSize();
	
	bool isCollision(const sf::FloatRect& bound);

private:
	tmx::MapLoader map_loader_;
	bool loaded_;
};

#endif
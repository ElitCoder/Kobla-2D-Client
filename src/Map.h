#ifndef MAP_H
#define MAP_H

#include "Entity.h"

#include <tmx/MapLoader.hpp>

class Map : public Entity {
public:
	Map();
	
	virtual void load(const std::string& filename) override;
	virtual void draw(sf::RenderWindow& window) override;

private:
	tmx::MapLoader map_loader_;
	bool loaded_;
};

#endif
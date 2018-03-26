#ifndef IMAGE_H
#define IMAGE_H

#include "Entity.h"

class Image : public Entity {
public:
	virtual void draw(sf::RenderWindow& window) override;
	virtual void load(const std::string& filename) override;
	
private:
	sf::Sprite sprite_;
};

#endif
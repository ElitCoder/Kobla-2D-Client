#ifndef IMAGE_H
#define IMAGE_H

#include "Entity.h"

class Image : public Entity {
public:
	virtual void draw(sf::RenderWindow& window) override;
	virtual void load(const std::string& filename) override;
	sf::Sprite& internal();
	
	sf::FloatRect getSize();
	
	void position(int x, int y);
	void scale(double value);
	
private:
	sf::Sprite sprite_;
};

#endif
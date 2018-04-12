#ifndef IMAGE_H
#define IMAGE_H

#include "Entity.h"

// GitHub Foaly
#include "AnimatedSprite.hpp"

class Image : public Entity {
public:
	virtual void draw(sf::RenderWindow& window) override;
	virtual void load(int id) override;
	AnimatedSprite& internal();
	
	sf::FloatRect getSize();
	
	void position(int x, int y);
	void scale(double value);
	
	void setAnimation(int direction);
	
private:
	AnimatedSprite sprite_;
	int texture_id_ = -1;
	bool loaded_	= false;
};

#endif
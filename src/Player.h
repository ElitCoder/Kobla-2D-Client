#ifndef PLAYER_H
#define PLAYER_H

#include "Entity.h"
#include "Image.h"
#include "Text.h"

class Player : public Entity {
public:
	virtual void draw(sf::RenderWindow& window) override;
	virtual void load(const std::string& filename) override;
	
	void setName(const std::string& name);
	void setPosition(int x, int y);
	
private:
	Image image_;
	Text text_;
	
	std::string name_;
	int x_;
	int y_;
};

#endif
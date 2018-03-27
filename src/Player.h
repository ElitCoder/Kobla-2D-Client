#ifndef PLAYER_H
#define PLAYER_H

#include "Entity.h"
#include "Image.h"
#include "Text.h"
#include "Timer.h"

enum {
	PLAYER_MOVE_UP,
	PLAYER_MOVE_DOWN,
	PLAYER_MOVE_LEFT,
	PLAYER_MOVE_RIGHT
};

class Player : public Entity {
public:
	virtual void draw(sf::RenderWindow& window) override;
	virtual void load(const std::string& filename) override;
	
	void setName(const std::string& name);
	void setPosition(double x, double y);
	
	void startMoving(int direction, bool tell_server);
	void move(double speed);
	void stopMoving(bool tell_server);
	
private:
	Image image_;
	Text text_;
	
	std::string name_;
	double x_;
	double y_;
	
	Timer started_moving_;
	bool moving_;
	int direction_;
};

#endif
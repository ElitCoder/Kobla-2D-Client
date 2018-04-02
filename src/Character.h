#ifndef CHARACTER_H
#define CHARACTER_H

#include "Image.h"
#include "Text.h"
#include "Timer.h"

enum {
	PLAYER_MOVE_UP,
	PLAYER_MOVE_DOWN,
	PLAYER_MOVE_LEFT,
	PLAYER_MOVE_RIGHT
};

class Character : public Entity {
public:
	virtual ~Character();
	
	virtual void draw(sf::RenderWindow& window) override;
	virtual void load(const std::string& filename) override;
	
	void setName(const std::string& name);
	void setPosition(double x, double y);
	void setID(size_t id);
	void setMovingSpeed(double speed);
	void setCollision(bool collision);
	void setHealth(double full, double current);
	
	void startMoving(int direction, bool tell_server);
	void move();
	void stopMoving(bool tell_server);
	
	size_t getID() const;
	double getX() const;
	double getY() const;
	
	bool isCollision(Character& moving_player);
	
	// Used for rendering
	double getMiddleX();
	double getMiddleY();
	double getMaxX();
	double getMaxY();
	
protected:
	Character();
	
	bool isPlayerInsideMap(double x, double y);
	
	size_t id_;
	std::string name_;
	
	double x_;
	double y_;
	
	Image image_;
	Text text_;
	
	Timer started_moving_;
	bool moving_;
	int direction_;
	double moving_speed_;
	bool collision_;
	
	double current_health_;
	double full_health_;
};

#endif
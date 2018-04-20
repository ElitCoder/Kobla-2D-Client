#pragma once
#ifndef OBJECT_H
#define OBJECT_H

#include "Entity.h"
#include "Config.h"
#include "Image.h"
#include "Timer.h"

//#include <SFML/Graphics/Rect.hpp>

#include <array>

enum {
	PLAYER_MOVE_RIGHT,
	PLAYER_MOVE_DOWN,
	PLAYER_MOVE_LEFT,
	PLAYER_MOVE_UP,
	PLAYER_MOVE_MAX
};

class Animation;

namespace sf {
	class Time;
}

class ObjectInformation {
public:
	void setConfig(const Config& config);
	
	const Animation& getAnimation(int direction);
	double getScale() const;
	
	std::array<double, 2> getCollisionScale() const;
	
private:
	Config config_;
	
	std::vector<int> animation_lines_;
	std::vector<Animation> animations_;
	bool animated_				= false;
	int texture_id_				= -1;
	double scale_				= 1;
	double collision_scale_x	= 1;
	double collision_scale_y	= 1;
};

class Object : public Entity {
public:
	virtual void draw(sf::RenderWindow& window) override;
	virtual void load(int id) override;
	
	virtual void setPosition(double x, double y);
	void setID(size_t id);
	void setMovingSpeed(double speed);
	void setCollision(bool collision);
	void setPredeterminedDistance(double distance);
	
	void startMoving(int direction, bool tell_server);
	bool move(sf::Time& frame_time);
	void stopMoving(bool tell_server);
	
	size_t getID() const;
	double getX() const;
	double getY() const;
	double getPredetermindedDistance() const;
	
	bool isCollision(const sf::FloatRect& box);
	bool isMoving() const;
	
protected:
	bool isPlayerInsideMap(const sf::FloatRect& box);
	sf::FloatRect getCollisionBox(bool only_boots);
	
	size_t id_;
	std::string name_;
	
	double x_;
	double y_;
	
	double original_x_;
	double original_y_;
	
	Image image_;
	
	Timer started_moving_;
	int direction_;
	double moving_speed_;
	bool moving_					= false;
	bool collision_					= false;
	double predetermined_distance_	= -1;
	double distance_moved_ 			= 0;
	
	int object_id_ = -1;
};

#endif
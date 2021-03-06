#pragma once
#ifndef OBJECT_H
#define OBJECT_H

#include "Entity.h"
#include "Config.h"
#include "Image.h"
#include "Timer.h"

#include <array>

enum {
	PLAYER_MOVE_RIGHT,
	PLAYER_MOVE_DOWN,
	PLAYER_MOVE_LEFT,
	PLAYER_MOVE_UP,
	PLAYER_MOVE_MAX
};

enum {
	COLLISION_PLAYERS,
	COLLISION_NPCS,
	COLLISION_MONSTERS,
	COLLISION_MAP,
	COLLISION_MAX
};

enum {
	OBJECT_TYPE_PLAYER,
	OBJECT_TYPE_MONSTER,
	OBJECT_TYPE_NPC,
	OBJECT_TYPE_TEMP
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
	double getAnimationSpeed() const;
	
	std::array<double, 2> getCollisionScale() const;
	
private:
	Config config_;
	
	std::vector<int> animation_lines_;
	std::vector<Animation> animations_;
	int animation_size_width_	= -1;
	int animation_size_height_	= -1;
	bool animated_				= false;
	double animation_speed_		= 1;
	int texture_id_				= -1;
	double scale_				= 1;
	double collision_scale_x	= 1;
	double collision_scale_y	= 1;
};

class Object : public Entity {
public:
	static int translateObjectTypeToCollision(int type);
	
	virtual void draw(sf::RenderWindow& window) override;
	virtual void load(int id) override;
	
	virtual void setPosition(double x, double y);
	void setID(int id);
	void setMovingSpeed(double speed);
	void setCollisions(const std::vector<bool>& collisions);
	void setColliding(bool status);
	void setObjectType(int type);
	void setDirection(int direction);
	void setDeterminedDestination(double x, double y);
	void setFollowing(bool status, int id = -1);
	
	void startMoving(int direction, bool tell_server);
	bool move(sf::Time& frame_time);
	void stopMoving(bool tell_server);
	
	int getID() const;
	double getX() const;
	double getY() const;
	bool getCollision(int type) const;
	int getObjectType() const;
	double getDistanceTo(Object* object);
	Image& getImage();
	
	bool isCollision(const sf::FloatRect& box);
	bool isMoving() const;
	bool isColliding() const;
	bool isFollowing() const;
	
protected:
	Object();
	
	bool isPlayerInsideMap(const sf::FloatRect& box);
	sf::FloatRect getCollisionBox(bool only_boots);
	
	int id_;
	std::string name_;
	
	double x_;
	double y_;
	
	double original_x_;
	double original_y_;
	
	Image image_;
	
	Timer started_moving_;
	int direction_;
	double moving_speed_			= -1;
	bool moving_					= false;
	double destination_x_			= -1;
	double destination_y_			= -1;
	bool reached_distance_x_		= false;
	bool reached_distance_y_		= false;
	bool determined_destination_	= false;
	bool following_					= false;
	int following_id_				= -1;
	
	std::vector<bool> collisions_;
	bool colliding_					= false;
	
	int object_id_					= -1;
	int object_type_				= -1;
};

#endif
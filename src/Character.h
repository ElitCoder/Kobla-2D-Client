#ifndef CHARACTER_H
#define CHARACTER_H

#include "Image.h"
#include "Text.h"
#include "Timer.h"
#include "Config.h"

enum {
	PLAYER_MOVE_RIGHT,
	PLAYER_MOVE_DOWN,
	PLAYER_MOVE_LEFT,
	PLAYER_MOVE_UP,
	PLAYER_MOVE_MAX
};

class CharacterInformation {
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

class Character : public Entity {
public:
	virtual ~Character();
	
	virtual void draw(sf::RenderWindow& window) override;
	virtual void load(int id) override;
	
	void setName(const std::string& name);
	void setPosition(double x, double y);
	void setID(size_t id);
	void setMovingSpeed(double speed);
	void setCollision(bool collision);
	void setHealth(double full, double current);
	void setPredeterminedDistance(double distance);
	
	void startMoving(int direction, bool tell_server);
	void move(sf::Time& frame_time);
	void stopMoving(bool tell_server);
	
	size_t getID() const;
	double getX() const;
	double getY() const;
	
	double getFullHealth() const;
	double getCurrentHealth() const;
	double getPredetermindedDistance() const;
	
	bool isCollision(const sf::FloatRect& box);
	
	// Used for rendering
	double getMiddleX();
	double getMiddleY();
	
protected:
	Character();
	
	bool isPlayerInsideMap(const sf::FloatRect& box);
	sf::FloatRect getCollisionBox(bool only_boots);
	
	size_t id_;
	std::string name_;
	
	double x_;
	double y_;
	
	double original_x_;
	double original_y_;
	
	Image image_;
	Text text_;
	
	Timer started_moving_;
	bool moving_;
	int direction_;
	double moving_speed_;
	bool collision_;
	double predetermined_distance_	= -1;
	double distance_moved_ 			= 0;
	
	double current_health_;
	double full_health_;
	
	int character_id_ = -1;
};

#endif
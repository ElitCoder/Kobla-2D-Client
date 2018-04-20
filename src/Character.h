#ifndef CHARACTER_H
#define CHARACTER_H

#include "Object.h"
#include "Text.h"

class Character : public Object {
public:
	virtual void draw(sf::RenderWindow& window) override;
	virtual void load(int id) override;
	
	// Needs updating the text as well
	virtual void setPosition(double x, double y) override;
	void updatePosition();
	void setHealth(double full, double current);
	void setName(const std::string& name);
	
	double getFullHealth() const;
	double getCurrentHealth() const;
	
	// Used for rendering
	double getMiddleX();
	double getMiddleY();
	
private:
	Text text_;
	
	double current_health_;
	double full_health_;
};

#endif
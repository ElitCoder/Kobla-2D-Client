#pragma once
#ifndef CHARACTER_H
#define CHARACTER_H

#include "Object.h"
#include "Text.h"

#include <SFML/Graphics/RectangleShape.hpp>

class Character : public Object {
public:
	virtual void draw(sf::RenderWindow& window) override;
	virtual void load(int id) override;
	
	// Needs updating the text as well
	virtual void setPosition(double x, double y) override;
	void updatePosition();
	void setHealth(double full, double current);
	void setName(const std::string& name);
	void setShowHealth(bool status);
	
	double getFullHealth() const;
	double getCurrentHealth() const;
	
	// Used for rendering
	double getMiddleX();
	double getMiddleY();
	
private:
	Text text_;
	sf::RectangleShape health_bar_;
	sf::RectangleShape health_bar_fill_;
	
	double current_health_;
	double full_health_;
	
	bool show_health_ = true;
};

#endif
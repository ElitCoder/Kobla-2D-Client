#pragma once
#ifndef TEXT_H
#define TEXT_H

#include "Entity.h"

#include <SFML/Graphics/Text.hpp>

#define NORMAL_FONT			("GROBOLD.ttf")
#define NORMAL_FONT_ID		(0)

#define FONT_SIZE_NAME		(20)
#define FONT_SIZE_CHAT		(18)
#define FONT_SIZE_CHATBOX	(16)

class Text : public Entity {
public:
	virtual void load(int id) override;
	virtual void draw(sf::RenderWindow& window) override;
	
	void set(const std::string& text);
	void size(int pixels);
	void color(const sf::Color& color);
	void position(int x, int y);
	
	void setOwner(int id);
	int getOwner() const;
	
	sf::FloatRect getSize() const;
	
private:
	sf::Text text_;
	
	int owner_ = -1;
};

#endif
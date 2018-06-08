#pragma once
#ifndef GUI_H
#define GUI_H

#include "Entity.h"

#include <TGUI/TGUI.hpp>

namespace sf {
	class View;
}

class GUI : public Entity {
public:
	GUI(sf::RenderWindow& window);
	
	virtual void draw(sf::RenderWindow& window) override;
	virtual void load(int id) override;
	void update(const sf::View& view);
	
	tgui::Gui& internal();
	
	void addChatText(const std::string& who, const std::string& message);
	void setChatInput(const std::string& input);
	
private:
	tgui::Gui gui_;
	
	std::vector<std::string> chat_;
};

#endif
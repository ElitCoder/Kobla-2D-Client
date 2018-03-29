#ifndef GUI_H
#define GUI_H

#include "Entity.h"

class GUI : public Entity {
public:
	GUI(sf::RenderWindow& window);
	
	virtual void draw(sf::RenderWindow& window) override;
	virtual void load(const std::string& file) override;
	void update();
	
	tgui::Gui& internal();
	
private:
	tgui::Gui gui_;
};

#endif
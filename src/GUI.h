#ifndef GUI_H
#define GUI_H

#include "Entity.h"

class GUI : public Entity {
public:
	GUI(sf::RenderWindow& window);
	
	virtual void draw(sf::RenderWindow& window) override;
	virtual void load(int id) override;
	void update(const sf::View& view);
	
	tgui::Gui& internal();
	
	void updateHealthBar(double full, double current);
	
private:
	tgui::Gui gui_;
};

#endif
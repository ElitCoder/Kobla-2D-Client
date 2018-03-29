#include "GUI.h"
#include "Log.h"

using namespace std;

GUI::GUI(sf::RenderWindow& window) :
	gui_(window) {}
	
tgui::Gui& GUI::internal() {
	return gui_;
}

void GUI::draw(sf::RenderWindow& window) {
	gui_.draw();
}

void GUI::load(const string& file) {
	// Nothing to load from file right now
	return;
	
	// Lower bar
	unsigned int chatbox_size[] = { gui_.getTarget()->getSize().x, 50 };
	auto chatbox = tgui::ChatBox::create();
	
	chatbox->setSize(chatbox_size[0] + 20, chatbox_size[1] + 10);
	chatbox->setPosition(-10, gui_.getTarget()->getSize().y - chatbox_size[1]);
	
	gui_.add(chatbox, "bottom_bar");
	
	// Top right bar (HP?)
	
	gui_.setOpacity(0.5);
}

// Update sizes after resize window to avoid auto re-scale
void GUI::update() {
	/*
	// Lower bar
	unsigned int chatbox_size[] = { gui_.getTarget()->getSize().x, 50 };
	auto widget = gui_.get("bottom_bar");
		
	widget->setSize(chatbox_size[0] + 20, chatbox_size[1] + 10);
	widget->setPosition(-10, gui_.getTarget()->getSize().y - chatbox_size[1]);
	*/
}
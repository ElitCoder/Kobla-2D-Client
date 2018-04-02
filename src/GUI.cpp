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
	//return;
	
	// Lower bar
	unsigned int chatbox_size[] = { gui_.getTarget()->getSize().x, 50 };
	auto chatbox = tgui::ChatBox::create();
	
	chatbox->setSize(chatbox_size[0], chatbox_size[1]);
	chatbox->setPosition(0, gui_.getTarget()->getSize().y - chatbox_size[1]);

	// Top right bar (HP?)
	unsigned int top_right_size[] = { 200, 80 };
	auto top_right = tgui::ChatBox::create();
	
	top_right->setSize(top_right_size[0], top_right_size[1]);
	top_right->setPosition(gui_.getTarget()->getSize().x - top_right_size[0], 0);
	
	gui_.add(chatbox, "bottom_bar");
	gui_.add(top_right, "top_right");
	
	gui_.setOpacity(0.6);
}

// Update sizes after resize window to avoid auto re-scale
void GUI::update(const sf::View& view) {
	// Update the view and remove the current widgets
	gui_.setView(view);
	gui_.removeAllWidgets();
	
	// Load the GUI again
	load("");
	
	/*
	// Lower bar
	unsigned int chatbox_size[] = { gui_.getTarget()->getSize().x, 50 };
	auto widget = gui_.get("bottom_bar");
		
	widget->setSize(chatbox_size[0] + 20, chatbox_size[1] + 10);
	widget->setPosition(-10, gui_.getTarget()->getSize().y - chatbox_size[1]);
	*/
}
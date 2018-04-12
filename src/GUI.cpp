#include "GUI.h"
#include "Log.h"
#include "Base.h"

using namespace std;

GUI::GUI(sf::RenderWindow& window) :
	gui_(window) {}
	
tgui::Gui& GUI::internal() {
	return gui_;
}

void GUI::draw(sf::RenderWindow& window) {
	// Remove warning from g++
	if (window.isOpen()) {}
		
	gui_.draw();
}

static array<long, 2> getMiddlePlacement(double outside_width, double outside_height,
	double outside_x, double outside_y, double inside_width, double inside_height) {	
	double x = outside_x + outside_width / 2 - inside_width / 2;
	double y = outside_y + outside_height / 2 - inside_height / 2;
	
	return {{ lround(x), lround(y) }};
}

static array<long, 2> addTextToWidgetPlacement(double x, double y, double width,
	double text_width, double text_height) {
	double p_x = x + width / 2 - text_width / 2;
	double p_y = y - text_height - 4;
	
	return {{ lround(p_x), lround(p_y) }};
}

void GUI::load(int id) {
	// Remove g++ warning
	if (id) {}
	
	// Lower bar
	unsigned int chatbox_size[] = { gui_.getTarget()->getSize().x, 50 };
	auto chatbox = tgui::ChatBox::create();
	
	chatbox->setSize(chatbox_size[0], chatbox_size[1]);
	chatbox->setPosition(0, gui_.getTarget()->getSize().y - chatbox_size[1]);
	gui_.add(chatbox, "bottom_bar");

	// Top right bar (HP?)
	unsigned int top_right_size[] = { 200, 80 };
	auto top_right = tgui::ChatBox::create();
	
	top_right->setSize(top_right_size[0], top_right_size[1]);
	top_right->setPosition(/*gui_.getTarget()->getSize().x - top_right_size[0]*/ 0, 0);
	gui_.add(top_right, "top_right");
	
	// Add HP bar
	unsigned int hp_bar_size[] = { 100, 20 };
	auto hp_bar_placement = getMiddlePlacement(top_right_size[0], top_right_size[1], top_right->getPosition().x,
		top_right->getPosition().y, hp_bar_size[0], hp_bar_size[1]);
	
	auto hp_bar = tgui::ProgressBar::create();
	hp_bar->setPosition(hp_bar_placement[0], hp_bar_placement[1] + 20);
	hp_bar->setSize(hp_bar_size[0], hp_bar_size[1]);
	hp_bar->setValue(100);
	gui_.add(hp_bar, "hp_bar");
	
	// Add label to HP bar
	auto label = tgui::Label::create();
	label->setText("Health");
	label->setTextSize(18);
	
	auto label_placement = addTextToWidgetPlacement(hp_bar->getPosition().x, hp_bar->getPosition().y,
		hp_bar_size[0], label->getSize().x, label->getSize().y);
		
	label->setPosition(label_placement.front(), label_placement.back());
	gui_.add(label, "hp_bar_text");
	
	gui_.setOpacity(0.6);
	
	Log(DEBUG) << "GUI started at game status " << Base::game().getGameStatus() << endl;
}

// Update sizes after resize window to avoid auto re-scale
void GUI::update(const sf::View& view) {
	// Update the view and remove the current widgets
	gui_.setView(view);
	gui_.removeAllWidgets();
	
	// Load the GUI again
	load(0);
}

void GUI::updateHealthBar(double full, double current) {
	auto hp_bar = gui_.get<tgui::ProgressBar>("hp_bar");
	auto percent = (current / full) * 100.0;
	
	Log(DEBUG) << "Percent: " << percent << endl;
	
	hp_bar->setValue(percent);
}
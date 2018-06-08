#include "GUI.h"
#include "Log.h"
#include "Base.h"
#include "Game.h"
#include "Engine.h"

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

void GUI::load(int id) {
	// Remove g++ warning
	if (id) {}
	
	// Lower bar
	unsigned int lower_bar_size[] = { gui_.getTarget()->getSize().x, 40 };
	
	auto lower_bar = tgui::ChatBox::create();
	lower_bar->setSize(lower_bar_size[0], lower_bar_size[1]);
	lower_bar->setPosition(0, gui_.getTarget()->getSize().y - lower_bar_size[1]);
	
	gui_.add(lower_bar, "lower_bar");
	
	// Chatbox
	unsigned int chatbox_size[] = { gui_.getTarget()->getSize().x / 2, 100 };
	
	auto chatbox = tgui::ChatBox::create();
	chatbox->setSize(chatbox_size[0], chatbox_size[1]);
	chatbox->setTextSize(FONT_SIZE_CHATBOX);
	chatbox->setPosition(0, lower_bar->getPosition().y - chatbox_size[1]);
	chatbox->setLinesStartFromTop(true);
	
	gui_.add(chatbox, "chatbox");
	
	// Area behind chat_input
	unsigned int chat_input_size[] = { static_cast<unsigned int>(lround(chatbox->getSize().x)), chatbox->getTextSize() };
	unsigned int chat_input_area_size[] = { static_cast<unsigned int>(lround(chatbox->getSize().x)), chatbox->getTextSize() };
	auto chat_input_area = tgui::ChatBox::create();
	chat_input_area->setSize(chat_input_area_size[0], chat_input_area_size[1]);
	chat_input_area->setTextSize(FONT_SIZE_CHATBOX);
	chat_input_area->setPosition(0, chatbox->getPosition().y - chat_input_size[1]);
	
	gui_.add(chat_input_area, "chat_input_area");
	
	// Input text to chatbox
	auto chat_input = tgui::Label::create();
	chat_input->setSize(chat_input_size[0], chat_input_size[1]);
	chat_input->setTextSize(FONT_SIZE_CHATBOX);
	chat_input->setPosition(0, chatbox->getPosition().y - chat_input_size[1]);
	
	gui_.add(chat_input, "chat_input");
	
	gui_.setFont(*Base::engine().getFont(NORMAL_FONT));
	gui_.setOpacity(0.66);
	
	Log(DEBUG) << "GUI started at game status " << Base::game().getGameStatus() << endl;
}

// Update sizes after resize window to avoid auto re-scale
void GUI::update(const sf::View& view) {
	// Save chat_input text
	auto chat_input = gui_.get<tgui::Label>("chat_input");
	auto chatbox = gui_.get<tgui::ChatBox>("chatbox");
	string input_text;
	vector<string> chats;
	
	if (chat_input != nullptr)
		input_text = chat_input->getText();
		
	if (chatbox != nullptr) {
		size_t i = 0;
		
		while (!string(chatbox->getLine(i)).empty()) {
			chats.push_back(string(chatbox->getLine(i)));
			i++;
		}
	}
		
	// Update the view and remove the current widgets
	gui_.setView(view);
	gui_.removeAllWidgets();
	
	// Load the GUI again
	load(0);
	
	chat_input = gui_.get<tgui::Label>("chat_input");
	chatbox = gui_.get<tgui::ChatBox>("chatbox");
	
	chat_input->setText(input_text);
	
	for (auto& message : chats)
		chatbox->addLine(message);
}

void GUI::addChatText(const string& who, const string& message) {
	auto chatbox = gui_.get<tgui::ChatBox>("chatbox");
	
	if (chatbox == nullptr)
		return;
		
	chatbox->addLine(who + ": " + message);
	chat_.push_back(who + ": " + message);
}

void GUI::setChatInput(const string& input) {
	auto chat_input = gui_.get<tgui::Label>("chat_input");
	
	if (chat_input == nullptr)
		return;
		
	chat_input->setText(input);	
}
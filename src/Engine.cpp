#include "Engine.h"
#include "Log.h"
#include "Base.h"

using namespace std;

extern mutex g_main_sync;

Engine::Engine() {}

Engine::~Engine() {
	// Clean fonts and textures
	for_each(textures_.begin(), textures_.end(), [] (auto& peer) { delete peer.second; });
	for_each(fonts_.begin(), fonts_.end(), [] (auto& peer) { delete peer.second; });
	
	// Destroy GUI
	Base::destroyGUI();
}

void Engine::start() {
	int w = Base::settings().get<int>("resolution_w");
	int h = Base::settings().get<int>("resolution_h");
	
	window_.create(sf::VideoMode(w, h), "Kobla-2D-Client");
	window_.setVerticalSyncEnabled(Base::settings().get<bool>("vsync"));
	
	// Create the GUI
	Base::createGUI(window_);
}

bool Engine::running() {
	return window_.isOpen();
}

// Fall through to basic stuff like window handling
void Engine::checkEventNotHandled(sf::Event& event) {
	switch (event.type) {
		case sf::Event::Closed: {
			window_.close();
			// Simple way to exit
			exit(1);
			break;
		}
		
		case sf::Event::Resized: {
			sf::FloatRect visibleArea(0, 0, event.size.width, event.size.height);
        	window_.setView(sf::View(visibleArea));
			
			Base::gui().update(window_.getView());
			break;
		}
		
		case sf::Event::LostFocus: Base::game().pause();
			break;
			
		case sf::Event::GainedFocus: Base::game().resume();
			break;
		
		default:
			;
	}
}

void Engine::render() {
	sf::Event event;
	
	while (window_.pollEvent(event)) {
		if (!Base::game().input(event))
			checkEventNotHandled(event);
			
		Base::gui().internal().handleEvent(event);
	}
	
	window_.clear(sf::Color::Black);
	
	// Draw everything
	Base::game().render(window_);
	Base::gui().draw(window_);
	
	// Unlock main thread sync
	g_main_sync.unlock();
	
	// This does not need to be protected
	window_.display();
}

void Engine::load() {
	Log(DEBUG) << "Loading up basic game\n";
	
	// TODO: Add things here if needed
}

sf::Texture* Engine::getTexture(const string& filename) {
	auto iterator = find_if(textures_.begin(), textures_.end(), [&filename] (auto& peer) { return peer.first == filename; });
	
	if (iterator == textures_.end()) {
		sf::Texture* texture = new sf::Texture;
		
		if (!texture->loadFromFile(filename))
			Log(WARNING) << "Could not load texture " << filename << endl;
			
		textures_.push_back({ filename, texture });	
		return texture;
	} else {
		return iterator->second;
	}
}

sf::Font* Engine::getFont(const string& filename) {
	auto iterator = find_if(fonts_.begin(), fonts_.end(), [&filename] (auto& peer) { return peer.first == filename; });
	
	if (iterator == fonts_.end()) {
		sf::Font* font = new sf::Font;
		
		if (!font->loadFromFile(filename))
			Log(WARNING) << "Could not load font " << filename << endl;
			
		fonts_.push_back({ filename, font });	
		return font;
	} else {
		return iterator->second;
	}
}

static void loadDataID(vector<pair<int, string>>& container, const string& path) {
	if (!container.empty())
		return;
		
	Config config;
	config.parse(path);
	
	for (auto& peer : config.internal()) {
		Log(DEBUG) << peer.first << endl;
		Log(DEBUG) << peer.second << endl;
		
		container.push_back({ stoi(peer.first), peer.second });
	}
}

static string findDataId(vector<pair<int, string>>& container, int id) {
	auto iterator = find_if(container.begin(), container.end(), [id] (auto& peer) { return peer.first == id; });
	
	if (iterator == container.end()) {
		Log(WARNING) << "Can't find ID from parsing config\n";
		
		return "";
	}
	
	return (*iterator).second;
}

string Engine::getMapName(int id) {
	loadDataID(map_names_, "data/maps/id");
	
	return findDataId(map_names_, id);
}

string Engine::getTextureName(int id) {
	loadDataID(texture_names_, "data/textures/id");
	
	return findDataId(texture_names_, id);
}

string Engine::getTexturePath() {
	return "data/textures/";
}

string Engine::getFontPath() {
	return "data/fonts/";
}
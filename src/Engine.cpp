#include "Engine.h"
#include "Log.h"
#include "Base.h"
#include "Config.h"
#include "GUI.h"
#include "Game.h"

#include <SFML/Graphics.hpp>

#include <algorithm>

using namespace std;

extern mutex g_main_sync;

Engine::Engine() {}

Engine::~Engine() {}

void Engine::start() {
	int w = Base::settings().get<int>("resolution_w", 640);
	int h = Base::settings().get<int>("resolution_h", 480);
	
	window_.create(sf::VideoMode(w, h), "Kobla-2D-Client");
	window_.setVerticalSyncEnabled(Base::settings().get<bool>("vsync", false));
	
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
			quick_exit(-1);
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

shared_ptr<sf::Texture>& Engine::getTexture(int id) {
	auto filename = getTextureName(id);
	auto iterator = find_if(textures_.begin(), textures_.end(), [&filename] (auto& peer) { return peer.first == filename; });
	
	if (iterator == textures_.end()) {
		shared_ptr<sf::Texture> texture = make_shared<sf::Texture>();
		
		if (!texture->loadFromFile(getTexturePath() + filename))
			Log(WARNING) << "Could not load texture " << filename << endl;
			
		textures_.push_back({ filename, texture });	
		return textures_.back().second;
	} else {
		return iterator->second;
	}
}

shared_ptr<sf::Font>& Engine::getFont(const string& filename) {
	auto iterator = find_if(fonts_.begin(), fonts_.end(), [&filename] (auto& peer) { return peer.first == filename; });
	
	if (iterator == fonts_.end()) {
		shared_ptr<sf::Font> font = make_shared<sf::Font>();
		
		if (!font->loadFromFile(getFontPath() + filename))
			Log(WARNING) << "Could not load font " << filename << endl;
			
		fonts_.push_back({ filename, font });	
		return fonts_.back().second;
	} else {
		return iterator->second;
	}
}

ObjectInformation& Engine::getObjectInformation(int id) {
	auto filename = getObjectInformationName(id);
	auto iterator = find_if(objects_.begin(), objects_.end(), [&filename] (auto& peer) { return peer.first == filename; });
	
	if (iterator == objects_.end()) {
		Config config;
		config.parse(getCharacterPath() + filename);
		
		ObjectInformation information;
		information.setConfig(config);
		
		objects_.push_back({ filename, information });
		return objects_.back().second;
	} else {
		return iterator->second;
	}
}

static void loadDataID(vector<pair<int, string>>& container, const string& path) {
	if (!container.empty())
		return;
		
	Config config;
	config.parse(path);
	
	for (auto& peer : config.internal()) 
		container.push_back({ stoi(peer.first), peer.second.back() });
}

static string findDataId(vector<pair<int, string>>& container, int id) {
	auto iterator = find_if(container.begin(), container.end(), [id] (auto& peer) { return peer.first == id; });
	
	if (iterator == container.end()) {
		Log(WARNING) << "Can't find ID " << id << " from parsing config\n";
		
		return "";
	}
	
	return (*iterator).second;
}

// Force pre-loading everything
void Engine::runWarm() {
	getFont(NORMAL_FONT);
	
	// Load up vectors
	getTextureName(0);
	getMapName(0);
	getObjectInformationName(0);
	
	// Load data from disk
	for (auto& peer : texture_names_)
		getTexture(peer.first);
		
	for (auto& peer : object_names_)
		getObjectInformation(peer.first);
}

string Engine::getMapName(int id) {
	loadDataID(map_names_, "data/maps/id");
	
	return findDataId(map_names_, id);
}

string Engine::getTextureName(int id) {
	loadDataID(texture_names_, "data/textures/id");
	
	return findDataId(texture_names_, id);
}

string Engine::getObjectInformationName(int id) {
	loadDataID(object_names_, "data/objects/id");
	
	return findDataId(object_names_, id);
}

string Engine::getTexturePath() {
	return "data/textures/";
}

string Engine::getFontPath() {
	return "data/fonts/";
}

string Engine::getCharacterPath() {
	return "data/objects/";
}
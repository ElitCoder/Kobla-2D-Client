#include "Engine.h"
#include "Log.h"
#include "Base.h"

using namespace std;

extern mutex g_main_sync;

Engine::Engine() {}

Engine::~Engine() {}

void Engine::start() {
	int w = Base::settings().get<int>("resolution_w");
	int h = Base::settings().get<int>("resolution_h");
	
	window_.create(sf::VideoMode(w, h), "Kobla-2D-Client");
	window_.setVerticalSyncEnabled(Base::settings().get<bool>("vsync"));
}

bool Engine::running() {
	return window_.isOpen();
}

void Engine::render() {
	sf::Event event;
	
	while (window_.pollEvent(event)) {
		switch (event.type) {
			case sf::Event::Closed: {
				window_.close();
				
				// Simple way to exit
				exit(1);
				
				break;
			}
			
			case sf::Event::Resized: {
				int w = Base::settings().get<int>("resolution_w");
				int h = Base::settings().get<int>("resolution_h");
				
				window_.setSize(sf::Vector2u(w, h));
				
				break;
			}
			
			case sf::Event::TextEntered: {
				if (event.text.unicode < 128)
					Log(DEBUG) << "Entered " << static_cast<char>(event.text.unicode) << endl;
					
				break;	
			}
			
			default:
				break;
		}
	}
	
	window_.clear(sf::Color::Black);
	
	// Draw everything
	Base::game().render(window_);
	
	// Unlock main thread sync
	g_main_sync.unlock();
	
	// This does not need to be protected
	window_.display();
}

void Engine::load() {
	Log(DEBUG) << "Loading up basic game\n";
	
	// TODO: Add things here if needed
}

sf::Texture& Engine::getTexture(const string& filename) {
	auto iterator = find_if(textures_.begin(), textures_.end(), [&filename] (auto& peer) { return peer.first == filename; });
	sf::Texture* looking_for = nullptr;
	
	if (iterator == textures_.end()) {
		sf::Texture texture;
		
		if (!texture.loadFromFile(filename))
			Log(WARNING) << "Could not load texture " << filename << endl;
			
		textures_.push_back({ filename, texture });	
		looking_for = &textures_.back().second;
	} else {
		looking_for = &(*iterator).second;
	}
	
	return *looking_for;
}

sf::Font& Engine::getFont(const string& filename) {
	auto iterator = find_if(fonts_.begin(), fonts_.end(), [&filename] (auto& peer) { return peer.first == filename; });
	sf::Font* looking_for = nullptr;
	
	if (iterator == fonts_.end()) {
		sf::Font font;
		
		if (!font.loadFromFile(filename))
			Log(WARNING) << "Could not load font " << filename << endl;
			
		fonts_.push_back({ filename, font });	
		looking_for = &fonts_.back().second;
	} else {
		looking_for = &(*iterator).second;
	}
	
	return *looking_for;
}

string Engine::getMapName(int id) {
	if (maps_.empty()) {
		Config config;
		config.parse("data/maps/id");
		
		for (auto& peer : config.internal()) {
			Log(DEBUG) << peer.first << endl;
			Log(DEBUG) << peer.second << endl;
			
			maps_.push_back({ stoi(peer.first), peer.second });
		}
	}
	
	auto iterator = find_if(maps_.begin(), maps_.end(), [id] (auto& peer) { return peer.first == id; });
	
	if (iterator == maps_.end()) {
		Log(WARNING) << "Can't find map from parsing config\n";
		
		return "";
	}
	
	return (*iterator).second;
}
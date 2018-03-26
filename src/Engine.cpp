#include "Engine.h"
#include "Log.h"
#include "Base.h"	

using namespace std;
using namespace sf;

Engine::Engine() {}

void Engine::start() {
	int w = Base::settings().get<int>("resolution_w");
	int h = Base::settings().get<int>("resolution_h");
	
	window_.create(VideoMode(w, h), "Kobla-2D-Client");
	window_.setVerticalSyncEnabled(Base::settings().get<bool>("vsync"));
	
	window_.setActive(false);
}

bool Engine::running() {
	return window_.isOpen();
}

void Engine::render() {
	Event event;
	
	while (window_.pollEvent(event)) {
		switch (event.type) {
			case Event::Closed: {
				window_.close();
				
				// Simple way to exit
				exit(1);
				
				break;
			}
			
			case Event::Resized: {
				window_.setSize(Vector2u(800, 600));
				
				break;
			}
			
			case Event::TextEntered: {
				if (event.text.unicode < 128)
					Log(DEBUG) << "Entered " << static_cast<char>(event.text.unicode) << endl;
					
				break;	
			}
			
			default:
				break;
		}
	}
	
	window_.clear(Color::Black);
	
	// Draw everything here
	
	window_.display();
}
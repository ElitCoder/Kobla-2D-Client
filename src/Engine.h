#ifndef ENGINE_H
#define ENGINE_H

// SFML
#include <SFML/Graphics.hpp>

class Engine {
public:
	Engine();
	
	void start();
	bool running();
	
	void render();
	
private:
	sf::RenderWindow window_;
};

#endif
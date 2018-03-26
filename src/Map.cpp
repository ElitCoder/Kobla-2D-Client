#include "Map.h"
#include "Log.h"

using namespace std;

Map::Map() : map_loader_("data/maps") {}

void Map::load(const string& filename) {
	if (!map_loader_.load(filename))
		Log(WARNING) << "Map " << filename << " could not be loaded\n";
}

void Map::draw(sf::RenderWindow& window) {
	window.draw(map_loader_);
}
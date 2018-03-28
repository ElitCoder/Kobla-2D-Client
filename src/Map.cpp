#include "Map.h"
#include "Log.h"

using namespace std;

Map::Map() : map_loader_("data/maps") {
	loaded_ = false;
}

void Map::load(const string& filename) {
	if (!map_loader_.load(filename))
		Log(WARNING) << "Map " << filename << " could not be loaded\n";
	else
		loaded_ = true;
}

void Map::draw(sf::RenderWindow& window) {
	if (!loaded_)
		return;
		
	window.draw(map_loader_);
}

sf::Vector2u Map::getSize() {
	return map_loader_.getMapSize();
}
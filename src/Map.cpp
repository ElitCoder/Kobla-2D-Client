#include "Map.h"
#include "Log.h"
#include "Base.h"
#include "Engine.h"

using namespace std;

Map::Map() : map_loader_("data/maps") {
	loaded_ = false;
}

void Map::load(int id) {
	auto filename = Base::engine().getMapName(id);
	
	if (!map_loader_.load(filename))
		Log(WARNING) << "Map " << filename << " could not be loaded\n";
	else
		loaded_ = true;
		
	if (!loaded_)
		return;
		
	const auto& layers = map_loader_.getLayers();
	
	for (const auto& layer : layers) {
		Log(DEBUG) << "Layer name: " << layer.name << endl;
	}
}

void Map::draw(sf::RenderWindow& window) {
	if (!loaded_)
		return;
		
	window.draw(map_loader_);
}

sf::Vector2u Map::getSize() {
	return map_loader_.getMapSize();
}

bool Map::isCollision(const sf::FloatRect& bound) {
	const auto& layers = map_loader_.getLayers();
	
	for (const auto& layer : layers) {
		if (layer.name != "Collision")
			continue;
			
		for(const auto& object : layer.objects) {
			if (object.getAABB().intersects(bound))
				return true;
        }
	}
	
	return false;
}
#include "Entity.h"
#include "Log.h"

using namespace std;
using namespace sf;

Entity::Entity() {
	static size_t current_id_;
	draw_id_ = current_id_++;
	
	Log(DEBUG) << "Running Entity constructor\n";
}

Entity::~Entity() {}
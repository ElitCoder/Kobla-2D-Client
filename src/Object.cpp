#include "Object.h"
#include "Base.h"
#include "PacketCreator.h"
#include "Log.h"
#include "Engine.h"
#include "Packet.h"
#include "NetworkCommunication.h"
#include "Game.h"

using namespace std;

extern int g_last_collision_id_;

/*
	ObjectInformation
*/

void ObjectInformation::setConfig(const Config& config) {
	config_ = config;
	
	texture_id_ = config_.get<int>("texture", -1);
	scale_ = config_.get<double>("scale", 1);
	animated_ = config_.get<bool>("animated", false);
	
	// Set size
	if (animated_) {
		animation_lines_.push_back(config_.get<int>("animation_right", -1));
		animation_lines_.push_back(config_.get<int>("animation_down", -1));
		animation_lines_.push_back(config_.get<int>("animation_left", -1));
		animation_lines_.push_back(config_.get<int>("animation_up", -1));
		
		animation_speed_ = 1.0 / config_.get<double>("animation_speed", animation_speed_);
	}
	
	collision_scale_x = config_.get<double>("collision_scale_x", 1);
	collision_scale_y = config_.get<double>("collision_scale_y", 1);
}

double ObjectInformation::getScale() const {
	return scale_;
}

double ObjectInformation::getAnimationSpeed() const {
	return animation_speed_;
}

const Animation& ObjectInformation::getAnimation(int direction) {
	if (direction < 0)
		Log(WARNING) << "direction < 0!\n";
		
	if (animations_.empty()) {
		auto* texture = Base::engine().getTexture(texture_id_);
		
		if (animated_) {
			// Load animations
			for (auto& line : animation_lines_) {
				Animation direction;
				direction.setSpriteSheet(*texture);
				
				// Get size of texture
				int size = lround((double)texture->getSize().y / (double)animation_lines_.size());
				
				for (size_t i = 0; i < texture->getSize().x; i += size)
					direction.addFrame(sf::IntRect(i, line * size, size, size));
					
				animations_.push_back(direction);
			}
		} else {
			Animation picture;
			picture.setSpriteSheet(*texture);
			
			auto size = texture->getSize();
			picture.addFrame(sf::IntRect(0, 0, size.x, size.y));
			
			animations_.push_back(picture);
		}
	}
	
	if (animations_.empty())
		Log(WARNING) << "animations_ is empty, there are no sprites available\n";
	
	if (direction < 0 || direction >= (int)animations_.size())
		return animations_.front();
	
	return animations_.at(direction);
}

array<double, 2> ObjectInformation::getCollisionScale() const {
	return {{ collision_scale_x, collision_scale_y }};
}

/*
	Object
*/

Object::Object() {}

// Static since translation doesn't need an instance
int Object::translateObjectTypeToCollision(int type) {
	switch (type) {
		case OBJECT_TYPE_MONSTER:	return COLLISION_MONSTERS;
		case OBJECT_TYPE_NPC:		return COLLISION_NPCS;
		case OBJECT_TYPE_PLAYER:	return COLLISION_PLAYERS;
	}
	
	Log(WARNING) << "Object type translation " << type << " failed\n";
	
	return -1;
}

void Object::draw(sf::RenderWindow& window) {
	image_.draw(window);
	//text_.draw(window);
}

void Object::load(int id) {
	// filename is the character ID
	image_.load(id);
	
	image_.scale(Base::engine().getObjectInformation(id).getScale());
	
	object_id_ = id;
}

void Object::setPosition(double x, double y) {
	image_.position(x, y);
		
	x_ = x;
	y_ = y;
}

// Start moving player in one direction
void Object::startMoving(int direction, bool tell_server) {
	// Just change direction
	if (moving_ && direction_ == direction)
		return;
		
	// TODO: Tell the server we're moving IF IT'S THE REAL PLAYER
	if (tell_server) {
		auto packet = PacketCreator::move(x_, y_, direction, true);
		Base::network().send(packet);
		
		Log(DEBUG) << "Position " << x_ << " " << y_ << endl;
	}
	
	moving_ = true;
	direction_ = direction;
	
	original_x_ = x_;
	original_y_ = y_;
	
	destination_x_ = -1;
	destination_y_ = -1;
	
	reached_distance_x_ = false;
	reached_distance_y_ = false;
	determined_destination_ = false;
	
	started_moving_.start();
}

void Object::stopMoving(bool tell_server) {
	if (!moving_)
		return;
		
	// TODO: Tell the server we're NOT moving IF IT'S THE REAL PLAYER
	if (tell_server) {
		auto packet = PacketCreator::move(x_, y_, -1, false);
		Base::network().send(packet);
	}
	
	moving_ = false;
}

bool Object::move(sf::Time& frame_time) {
	g_last_collision_id_ = -1;
	
	image_.setAnimation(direction_);
	
	if (!moving_) {
		image_.internal().stop();
		image_.internal().update(frame_time);
		
		return false;
	}
	
	image_.internal().update(frame_time);
	
	auto time_elapsed = started_moving_.restart();
	double pixels = moving_speed_ * time_elapsed;
	
	double x = x_;
	double y = y_;
	
	double diff_x = destination_x_ - x;
	double diff_y = destination_y_ - y;
	
	bool is_reaching_x = false;
	bool is_reaching_y = false;
	
	if (determined_destination_) {
		// Move to target
		double diff_x_squared = pow(diff_x, 2);
		double diff_y_squared = pow(diff_y, 2);
		
		double distance = diff_x_squared + diff_y_squared;
		
		double move_x = diff_x_squared / distance * pixels;
		double move_y = diff_y_squared / distance * pixels;
		
		if (move_x >= abs(diff_x))
			is_reaching_x = true;
			
		if (move_y >= abs(diff_y))
			is_reaching_y = true;
			
		if (diff_x < 0)
			x -= move_x;
		else
			x += move_x;
		
		if (diff_y < 0)
			y -= move_y;
		else
			y += move_y;
	} else {
		switch (direction_) {
			case PLAYER_MOVE_UP: y -= pixels;
				break;
				
			case PLAYER_MOVE_DOWN: y += pixels;
				break;
				
			case PLAYER_MOVE_LEFT: x -= pixels;
				break;
				
			case PLAYER_MOVE_RIGHT: x += pixels;
				break;
		}
	}
	
	// Test new position with collisions
	image_.position(x, y);
	auto box = getCollisionBox(true);
	
	// Just ignore updating position if it would be outside map
	if (!isPlayerInsideMap(box) || Base::game().isCollision(box, *this)) {
		// Did not work, revert to old values
		image_.position(x_, y_);
		return false;
	}
	
	if (determined_destination_) {
		if (is_reaching_x)
			reached_distance_x_ = true;
		
		if (is_reaching_y)
			reached_distance_y_ = true;
		
		if (reached_distance_x_ && reached_distance_y_) {
			setPosition(destination_x_, destination_y_);
			
			// Stop moving since the Server defined position
			stopMoving(false);
			
			return true;
		}
	}
		
	setPosition(x, y);
	
	return true;
}

void Object::setID(size_t id) {
	id_ = id;
}

size_t Object::getID() const {
	return id_;
}

void Object::setMovingSpeed(double speed) {
	moving_speed_ = speed;
	
	// Set animation speed based on movement speed (this fits the animated player for now)
	double frame_time = (1 / moving_speed_) * 10 * Base::engine().getObjectInformation(object_id_).getAnimationSpeed();
	
	image_.internal().setFrameTime(sf::seconds(frame_time));
}

double Object::getX() const {
	return x_;
}

double Object::getY() const {
	return y_;
}

// Are the new coordinates x and y inside of the map?
bool Object::isPlayerInsideMap(const sf::FloatRect& box) {
	// Don't move outside of the map
	if (box.left < 0 || box.top < 0)
		return false;
		
	if ((box.left + box.width) > Base::game().getMap().getSize().x || (box.top + box.height) > Base::game().getMap().getSize().y)
		return false;
		
	return true;
}

sf::FloatRect Object::getCollisionBox(bool only_boots) {
	// Calculate the actual collision detection box, since using the image full size can be too hard on the map
	auto box = image_.internal().getGlobalBounds();
	auto collision_scale = Base::engine().getObjectInformation(object_id_).getCollisionScale();
	auto size_x = box.width * collision_scale.front() * (only_boots ? 0.5 : 1.0);
	auto size_y = box.height * collision_scale.back();
	
	box.top += box.height / 2 - size_y / 2;
	box.left += box.width / 2 - size_x / 2;
	box.width = size_x;
	box.height = size_y;
	
	// Only collision check with the boots
	if (only_boots) {
		auto body = box.height * 0.7;
		auto boots = box.width * 0.8;
		
		box.top += body;
		box.height -= body;
		box.left += box.width / 2 - boots / 2;
		box.width = boots;
	}
	
	return box;
}

bool Object::isCollision(const sf::FloatRect& box) {
	return box.intersects(image_.internal().getGlobalBounds());
}

#if 0
void Object::setPredeterminedDistance(double distance) {
	predetermined_distance_ = distance;
}

double Object::getPredetermindedDistance() const {
	return predetermined_distance_;
}
#endif

bool Object::isMoving() const {
	return moving_;
}

void Object::setObjectType(int type) {
	object_type_ = type;
}

void Object::setColliding(bool status) {
	colliding_ = status;
}

void Object::setCollisions(const vector<bool>& collisions) {
	collisions_ = collisions;
}

bool Object::getCollision(int type) const {
	if (type >= (int)collisions_.size() || type < 0) {
		Log(WARNING) << "Trying to get collision information with not populated collisions_\n";
		
		return false;
	}
		
	return collisions_.at(type);
}

bool Object::isColliding() const {
	return colliding_;
}

int Object::getObjectType() const {
	return object_type_;
}

void Object::setDirection(int direction) {
	direction_ = direction;
	
	image_.setAnimation(direction);
}

Image& Object::getImage() {
	return image_;
}

double Object::getDistanceTo(Object* object) {
	auto first_size = image_.getSize();
	auto second_size = object->getImage().getSize();
	
	double first_x = getX() + first_size.left / 2.0;
	double first_y = getY() + first_size.top / 2.0;
	
	double second_x = object->getX() + second_size.left / 2.0;
	double second_y = object->getY() + second_size.top / 2.0;
	
	double distance = pow(first_x - second_x, 2) + pow(first_y - second_y, 2);
	
	return sqrt(distance);
}

void Object::setDeterminedDestination(double x, double y) {
	destination_x_ = x;
	destination_y_ = y;
	
	reached_distance_x_ = false;
	reached_distance_y_ = false;
	determined_destination_ = true;
}
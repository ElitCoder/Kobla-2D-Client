#include "Object.h"
#include "Base.h"
#include "PacketCreator.h"
#include "Log.h"

using namespace std;

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
	}
	
	collision_scale_x = config_.get<double>("collision_scale_x", 1);
	collision_scale_y = config_.get<double>("collision_scale_y", 1);
}

double ObjectInformation::getScale() const {
	return scale_;
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
				int size = texture->getSize().y / animation_lines_.size();
				
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

void Object::draw(sf::RenderWindow& window) {
	image_.draw(window);
	//text_.draw(window);
}

void Object::load(int id) {
	// filename is the character ID
	image_.load(id);
	
	image_.scale(Base::engine().getObjectInformation(id).getScale());
	
	// Set animation speed based on movement speed (this fits the animated player for now)
	double frame_time = (1 / moving_speed_) * 10;
	image_.internal().setFrameTime(sf::seconds(frame_time));
	
	object_id_ = id;
}

void Object::setCollision(bool collision) {
	collision_ = collision;
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
	}
	
	moving_ = true;
	direction_ = direction;
	
	original_x_ = x_;
	original_y_ = y_;
	
	predetermined_distance_ = -1;
	distance_moved_ = 0;
	
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
	
	// Add to distance moved
	distance_moved_ += pixels;
	
	if (predetermined_distance_ > 0 && distance_moved_ >= predetermined_distance_) {
		// We had a predetermined distance to move, just set it to that value
		switch (direction_) {
			case PLAYER_MOVE_UP: y = original_y_ - predetermined_distance_;
				break;
				
			case PLAYER_MOVE_DOWN: y = original_y_ + predetermined_distance_;
				break;
				
			case PLAYER_MOVE_LEFT: x = original_x_ - predetermined_distance_;
				break;
				
			case PLAYER_MOVE_RIGHT: x = original_x_ + predetermined_distance_;
				break;
		}
		
		setPosition(x, y);
		
		// Stop moving since the Server defined walking length
		stopMoving(false);
		
		return true;
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
	if (!collision_)
		return false;
	
	return box.intersects(image_.internal().getGlobalBounds());
}

void Object::setPredeterminedDistance(double distance) {
	predetermined_distance_ = distance;
}

double Object::getPredetermindedDistance() const {
	return predetermined_distance_;
}

bool Object::isMoving() const {
	return moving_;
}
#include "TemporaryObject.h"

void TemporaryObject::setType(int type) {
	object_type_ = type;
}

void TemporaryObject::setOwner(int id) {
	owner_ = id;
}

int TemporaryObject::getOwner() const {
	return owner_;
}
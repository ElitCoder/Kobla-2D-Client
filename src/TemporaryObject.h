#pragma once
#ifndef TEMPORARY_OBJECT_H
#define TEMPORARY_OBJECT_H

#include "Object.h"

enum {
	TEMP_OBJECT_BULLET
};

class TemporaryObject : public Object {
public:
	void setType(int type);
	
	void setOwner(int id);
	int getOwner() const;
	
private:
	int object_type_ 	= -1;
	int owner_			= -1;
};

#endif
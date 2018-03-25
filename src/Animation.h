#ifndef ANIMATION_H
#define ANIMATION_H

#include <iostream>
#include <vector>
#include <SDL.h>

class Animation
{
public:
	Animation()
	{
		this->frame = 1;
		this->fps = 0;

		this->nextChange = 0;
		this->disTime = 0;

		this->x = 0;
		this->y = 0;

		this->cX = 0;
		this->cY = 0;

		this->inWin = false;
		this->deletePic = false;

		this->disTimeSec = 450;
	}

	std::vector<SDL_Surface*> ani;

	int frame;
	int fps;

	long nextChange;
	long disTime;

	int id;

	int x;
	int y;

	int cX;
	int cY;

	bool inWin;
	bool deletePic;

	long disTimeSec;
};

void LoadAnimations();
void UnloadAnimations();

Animation *GetSkillAnimation(int id);

#endif
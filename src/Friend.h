#ifndef FRIEND_H
#define FRIEND_H

#include <iostream>
#include <vector>
#include <SDL.h>

#include "Game.h"

class ChatManager
{
public:
	ChatManager()
	{
		this->startY = 0;

		this->goUp = false;
		this->goDown = false;
	}

	void CleanChat();
	void AddMessage(Chat c);

	void StartDrag(int mx, int my);
	void Drag(int mx, int my);

	void CheckUpDown();

	void SetLast();

	std::vector<Chat> tps;
	int startY;

	bool goUp;
	bool goDown;
};

class PMessage
{
public:
	PMessage()
	{
		this->id = 0;
		this->toname = "";
		this->tonamep = NULL;

		this->x = 0;
		this->y = 0;

		this->startY = 0;

		this->text = "";

		this->goUp = false;
		this->goDown = false;
		
		this->chatPlace = 0;
		this->chatPlaceW = 0;

		this->textp = NULL;
	}

	void Clean();

	void StartDrag(int mx, int my);
	void Drag(int mx, int my);

	void CheckUpDown();

	void SetLast();

	int id;
	std::string toname;
	SDL_Surface *tonamep;

	std::vector<std::string> wrote;
	std::vector<SDL_Surface*> wrotep;

	std::string text;
	SDL_Surface *textp;

	int x, y, startY;

	bool goUp;
	bool goDown;

	int chatPlace;
	int chatPlaceW;
};

class PrivateMessage
{
public:
	PrivateMessage()
	{
		this->win = NULL;
		this->quitx = NULL;

		this->marked = "";
	}

	void Clean();
	void AddWindow(const char *name);
	void RemoveWindow(const char *name);
	void AddMessage(const char *name, const char *fullmsg);
	PMessage *GetMarked();

	SDL_Surface *win;
	SDL_Surface *quitx;

	std::vector<PMessage> pm;

	std::string marked;
};

class Friend
{
public:
	Friend()
	{
		this->id = 0;
		this->name = "";
		this->status = false;
		this->level = 1;
		this->job = 0;

		this->namep = NULL;
		this->levelp = NULL;

		this->showMenu = false;
	}

	int id;
	std::string name;
	bool status;
	int level;
	int job;

	SDL_Surface *namep;
	SDL_Surface *levelp;

	std::vector<MenuButton> menu;
	bool showMenu;
};

class FriendWindow
{
public:
	FriendWindow()
	{
		this->x = 0;
		this->y = 0;

		this->win = NULL;
		this->quitx = NULL;
		this->noteup = NULL;
		this->notedown = NULL;

		this->enable = false;

		this->startY = 0;

		this->goUp = false;
		this->goDown = false;
	}

	void Clean();
	void CleanNames();
	void UpdateNames();

	void StartDrag(int mx, int my);
	void Drag(int mx, int my);
	void StopDrag();

	void CheckUpDown();

	int x;
	int y;

	SDL_Surface *win;
	SDL_Surface *quitx;
	SDL_Surface *noteup;
	SDL_Surface *notedown;

	std::vector<Friend> tps;

	bool enable;

	int startY;

	bool goUp;
	bool goDown;
};

#endif
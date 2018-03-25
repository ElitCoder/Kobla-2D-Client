#ifndef LOGIN_H
#define LOGIN_H

#define INVENTORYITEM 90
#define SKILL 91
#define INVENTORYITEMEQUIP 93
#define TASKBARINVENTORYITEM 94
#define TASKBARSKILL 95
#define TASKBARINVENTORYITEMEQUIP 96
#define BUFF 97
#define SKILLWINDOW 98
#define BUYITEM 99
#define CARTITEM 100
#define INPUTITEM 101
#define DROPITEM 102
#define TRADEITEMSIDEONE 103
#define TRADEITEMSIDETWO 104
#define WINDOW_ADDSTAT 105

#define PLAYER 10000
#define MONSTER 10002
#define BULLET 10003
#define WEAPON 10004
#define BULLETSLOWSPEED 10005
#define BULLETPOISON 10006

#include <SDL.h>

#include "Game.h"

int Login();
void Register();
void Logout();

void CharacterList();
void EditCharacter();
void CreateCharacter();
void DeleteCharacter();

class Damage
{
public:
	Damage()
	{
		this->dmg = NULL;
		this->dmgItem = NULL;

		this->x = 0;
		this->y = 0;
		
		this->deletetime = 0;

		this->move = Timer();
	}
	
	SDL_Surface *dmg;
	SDL_Surface *dmgItem;

	float x;
	float y;

	long deletetime;

	Timer move;
};

class NameSurface
{
public:
	NameSurface()
	{
	}

	void clean()
	{
		for(unsigned int i = 0; i < this->surs.size(); i++)
			SDL_FreeSurface(this->surs.at(i));

		this->surs.clear();
	}

	SDL_Surface *first()
	{
		if(this->surs.size() > 0)
			return this->surs.at(0);

		else
			return NULL;
	}

	void draw(int x, int y);

	std::vector<SDL_Surface*> surs;
};

class Character
{
public:
	Character()
	{
		this->level = 1;
		this->power = 2;
		this->magicpower = 1;
		this->attack = 3;
		this->accuracy = 50;
		this->agility = 2;

		strcpy_s(this->name, "");

		this->x = 0;
		this->y = 0;

		this->cMove = RIGHT;
		this->wMove = RIGHT;

		this->moving = false;

		this->map = 1;

		this->hp = 1;
		this->maxhp = 3;

		this->speed = 8;

		this->equipped = false;
		this->dead = false;

		this->exp = 0.0f;

		this->c = NULL;

		this->hpPoint = 0;
		this->pAv = 0;

		this->inParty = false;

		this->cX = 0;
		this->cY = 0;

		this->inWin = false;

		this->gold = 0;

		this->move = Timer();

		this->pixSec = 500;

		this->weapon = NULL;
		this->hat = NULL;
		this->suit = NULL;
		this->belt = NULL;

		this->weaponid = 0;
		this->hatid = 0;
		this->suitid = 0;
		this->beltid = 0;

		this->proc = 100;
		
		this->showProc = false;

		this->timerProc = 0;
		this->enableTimer = false;
		this->procentType = 0;

		this->id = 0;

		this->showMenu = false;

		this->followId = 0;
		this->follow = false;

		this->intt = 2;

		this->mana = 0;
		this->maxmana = 0;
		this->energy = 0;
		this->maxenergy = 0;

		this->manaSurface = NULL;
		this->energySurface = NULL;

		this->job = 1;
	}

	void Load(int job);

	char name[40];

	int level;
	int power;
	int magicpower;
	int attack;
	int accuracy;
	int agility;
	int intt;

	int hpPoint;
	int pAv;

	int id;

	float x;
	float y;

	int cMove;

	SDL_Surface *my;
	NameSurface nameSur;

	SDL_Surface *weapon;
	SDL_Surface *deadImg;

	SDL_Surface *hat;
	SDL_Surface *suit;
	SDL_Surface *belt;

	int weaponid;
	int hatid;
	int suitid;
	int beltid;

	ChatWindow *c;

	int wMove;

	bool moving;

	int map;

	int hp;
	int maxhp;

	int speed;

	bool equipped;
	bool dead;

	float exp;

	bool inParty;

	int cX;
	int cY;

	bool inWin;

	int gold;

	Timer move;

	int pixSec;

	int proc;

	bool showProc;

	long timerProc;
	bool enableTimer;
	int procentType;

	std::vector<MenuButton> menu;

	bool showMenu;

	int followId;
	bool follow;

	int mana;
	int maxmana;
	int energy;
	int maxenergy;

	SDL_Surface *manaSurface;
	SDL_Surface *energySurface;

	int job;
};

class Monster
{
public:
	Monster()
	{
		this->level = 1;

		this->hp = 1;
		this->maxhp = 1;

		this->attack = 1;

		this->moving = false;

		this->x = 0;
		this->y = 0;

		this->id = 0;
		this->mId = 0;

		this->cX = 0;
		this->cY = 0;

		this->inWin = false;

		this->followId = 0;

		this->beforeX = 0;
		this->beforeY = 0;

		this->nextNotice = 0;

		this->move = Timer();

		this->speed = 0;

		this->proc = 100;

		this->showProc = false;

		this->timerProc = 0;
		this->enableTimer = false;
		this->procentType = 0;

		this->whenMarkName = NULL;
		this->showLvl = false;

		this->fades = false;
		this->fadeway = false;
		this->fadetime = 0;
		this->fadedie = 0;
		this->fadep = 0;
		this->dead = false;
	}

	void Load();

	int id;
	int mId;

	int level;

	int hp;
	int maxhp;

	int attack;

	bool moving;
	float x;
	float y;

	int moveWay;

	SDL_Surface *m;
	SDL_Surface *nameSurface;
	SDL_Surface *whenMarkName;

	int cX;
	int cY;

	bool inWin;

	int followId;

	float beforeX;
	float beforeY;

	long nextNotice;

	Timer move;
	
	int speed;

	int proc;

	bool showProc;

	long timerProc;
	bool enableTimer;
	int procentType;

	bool showLvl;

	bool fades;
	bool fadeway;
	long fadetime;
	long fadedie;
	int fadep;
	bool dead;
};

#endif
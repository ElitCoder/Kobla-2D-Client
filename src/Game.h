#ifndef GAME_H
#define GAME_H

#define HPUPDATE 100
#define MANAUPDATE 101
#define ENERGYUPDATE 102
#define HPWINDOW 2000
#define CHATWINDOW 2001
#define INFOWINDOW 2002
#define ADDSTATWINDOW 2003
#define INVITEINPUTWINDOW 2004
#define KICKINPUTWINDOW 2005
#define FRIENDINPUTWINDOW 2006

#define LEFT 1
#define RIGHT 2
#define UP 3
#define DOWN 4

#define STR 20
#define STA 21

#define SPEED 30
#define STAMINA 31
#define STRENGTH 32
#define SPEEDMINUS 33
#define HPDRAIN 34
#define AGILITY 35
#define CRIT 36
#define TOTALDMG 37
#define DEFENCE 38
#define HPREGEN 39
#define HPPLUS 40
#define LIFESTEAL 41
#define ATTACK 42
#define BLOCK 43
#define HPPROCENTPLUS 44
#define HPPROCENTMINUS 45
#define HPMINUS 46
#define IGENOM 47
#define INTT 48
#define MANAPLUS 49
#define MANAMINUS 50
#define MANAPROCENTPLUS 51
#define MANAPROCENTMINUS 52
#define ATTACKSPEED 53

#define BUTTON1 200
#define BUTTON2 201
#define BUTTON3 202

#define NORMALSHOT 11

#define TRANS 1
#define SOUND 2
#define MUSIC 3

#define PARTY 100000
#define TRADE 100001
#define FRIEND 100002

#define INVENTORYREMOVEITEM 50
#define INVENTORYDROPITEM 51
#define NPCTRADESELL 52
#define PLAYERTRADEADD 53

#ifdef _WIN32
#include <winsock2.h>
#include <windows.h>
#endif

#include <SDL.h>
#include <SDL_mixer.h>
#include <vector>

extern bool drag;
extern int isMasked;

bool GameLoop();

class PPAK
{
public:
	PPAK()
	{
		this->buf = NULL;
		this->len = 0;
	}

	unsigned char *buf;
	int len;
};

class Timer
{
public:
    Timer()
	{
		this->startTicks = 0;
		this->started = false;
	}

    void start(bool fast);
    int get_ticks(bool fast);

private:
    int startTicks;
	bool started;
};

class ChatWindow
{
public:
	ChatWindow()
	{
		this->removeTime = 0;

		this->text1 = NULL;
		this->text2 = NULL;

		this->theWindow = NULL;
	}

	long removeTime;

	SDL_Surface *text1;
	SDL_Surface *text2;

	SDL_Surface *theWindow;
};

class Window
{
public:
	Window()
	{
		this->x = 0;
		this->y = 0;

		this->enable = false;
		this->enableX = false;

		this->hpStartX = 0;
		this->hpY = 0;

		this->TYPE = 0;

		this->w = NULL;
		this->quitx = NULL;
		this->inviteButton = NULL;

		this->levelSurface = NULL;
		this->expSurface = NULL;
		this->hpSurface = NULL;
	}

	int x;
	int y;

	int hpStartX;
	int hpY;

	bool enable;
	bool enableX;

	int TYPE;

	SDL_Surface *w;
	SDL_Surface *quitx;

	SDL_Surface *inviteButton;

	SDL_Surface *hpSurface;
	SDL_Surface *levelSurface;
	SDL_Surface *expSurface;
};

class PartyMember
{
public:
	PartyMember()
	{
		strcpy_s(this->name, "");

		this->level = 0;

		this->names = NULL;
		this->lvls = NULL;
	}

	char name[40];
	SDL_Surface *names;

	int level;
	SDL_Surface *lvls;
};

class PartyInvite
{
public:
	PartyInvite()
	{
		this->yes = NULL;
		this->no = NULL;

		this->win = NULL;
		this->from = NULL;

		this->x = 0;
		this->y = 0;

		strcpy_s(name, "");

		this->nameid = 0;
		this->type = PARTY;
	}

	SDL_Surface *yes;
	SDL_Surface *no;

	SDL_Surface *win;
	SDL_Surface *from;

	int x;
	int y;

	char name[40];

	int nameid;
	int type;
};

class PartyWindow
{
public:
	PartyWindow()
	{
		this->leave = NULL;
		this->kick = NULL;
		this->invite = NULL;

		this->window = NULL;
		this->quitx = NULL;

		this->x = 0;
		this->y = 0;

		this->enable = false;
		this->enablePress = true;

		this->level = 1;
		this->exp = 0;

		this->levelSur = NULL;
	}

	SDL_Surface *leave;
	SDL_Surface *kick;
	SDL_Surface *invite;

	SDL_Surface *window;
	SDL_Surface *quitx;

	SDL_Surface *levelSur;
	
	std::vector<PartyMember> pm;

	int x;
	int y;

	bool enable;
	bool enablePress;

	int level;
	int exp;
};

class AddStatWindow
{
public:
	AddStatWindow()
	{
		this->x = 0;
		this->y = 0;

		this->power = NULL;
		this->hp = NULL;
		this->pointsav = NULL;
		this->add = NULL;
		this->window = NULL;
		this->quitx = NULL;
		this->pkpoints = NULL;
		this->servername = NULL;
		this->names = NULL;
		this->agility = NULL;
		this->intt = NULL;

		this->enable = false;
	}

	SDL_Surface *power;
	SDL_Surface *hp;
	SDL_Surface *pointsav;
	SDL_Surface *pkpoints;
	SDL_Surface *servername;
	SDL_Surface *names;
	SDL_Surface *agility;
	SDL_Surface *intt;

	SDL_Surface *add;

	SDL_Surface *window;

	SDL_Surface *quitx;

	int x;
	int y;

	bool enable;
};

class Chat
{
public:
	Chat()
	{
		this->x = 0;
		this->y = 0;

		this->message = NULL;
		this->msg = "";
		this->name = "";
	}

	SDL_Surface *message;
	std::string msg;
	std::string name;

	int x;
	int y;
};

class MenuButton
{
public:
	MenuButton()
	{
		strcpy_s(this->text, "");
		this->tSur = NULL;
		this->btn = NULL;
	}

	char text[80];
	SDL_Surface *tSur;
	SDL_Surface *btn;
};

class NPCText
{
public:
	NPCText()
	{
		this->text = NULL;

		this->x = 0;
		this->y = 0;
	}

	SDL_Surface *text;

	int x;
	int y;
};

class NPC
{
public:
	NPC()
	{
		this->x = 0;
		this->y = 0;

		this->id = 0;

		strcpy_s(this->name, "");

		this->my = NULL;
		this->nameSurface = NULL;

		this->showMenu = false;

		this->dialog = "";
		this->showDialog = false;

		this->dialogWindow = NULL;
		this->dialogWindowName = NULL;

		this->dialogX = 0;
		this->dialogY = 0;

		this->quitx = NULL;

		this->cX = 0;
		this->cY = 0;

		this->inWin = false;

		this->c = NULL;

		this->showShop = false;
	}

	void Load();

	int x;
	int y;

	int id;

	char name[100];

	SDL_Surface *my;
	SDL_Surface *nameSurface;
	SDL_Surface *dialogWindow;
	SDL_Surface *dialogWindowName;

	std::vector<MenuButton> menu;
	std::vector<MenuButton> diab;
	std::vector<NPCText> npct;

	std::string dialog;

	bool showMenu;
	bool showDialog;

	int dialogX;
	int dialogY;

	SDL_Surface *quitx;

	int cX;
	int cY;

	bool inWin;

	ChatWindow *c;

	bool showShop;
};

class Notifer
{
public:
	Notifer()
	{
		this->x = 0;
		this->y = 0;

		this->dTime = 0;
		this->sTime = 0;

		this->msg = NULL;

		this->penya = false;
	}

	int x;
	int y;

	SDL_Surface *msg;

	long dTime;
	long sTime;

	bool penya;
};

class BulletHit
{
public:
	BulletHit()
	{
		this->id = 0;
		this->type = 0;
	}

	int id;
	int type;
};

class Bullet
{
public:
	Bullet()
	{
		this->cX = 0;
		this->cY = 0;

		this->sX = 0;
		this->sY = 0;

		this->startY = 0;
		this->startX = 0;
		
		this->cWay = 0;
		this->param = 0;

		this->owner = 0;

		this->cPic = NULL;

		this->inWin = false;

		this->move = Timer();

		this->igenom = false;
		this->hits = 1;

		this->target = 0;
		this->type = 2;

		this->speed = 0;
		this->rot = 0;

		this->disX = 650;
	}

	float cX;
	float cY;

	int sX;
	int sY;

	int startY;
	int startX;

	int cWay;
	int param;

	int owner;

	bool igenom;
	int hits;

	SDL_Surface *cPic;

	bool inWin;

	Timer move;

	std::vector<BulletHit> hitted;

	int target;
	int type;

	int speed;
	int rot;

	int disX;
};

class Icon
{
public:
	Icon()
	{
		this->x = 0;
		this->y = 0;

		this->timeLeftX = 5;
		this->timeLeftY = 28;

		this->timeLeft = 0;

		this->nextRemove = 0;

		this->icon = NULL;
		this->tSurface = NULL;

		this->name1 = NULL;
		this->name2 = NULL;

		this->name1X = 0;
		this->name2X = 0;

		this->name1Y = 0;
		this->name2Y = 0;
		
		this->lvl = 1;
		this->effect = 0;
		this->changeEffect = 0;
	}

	int x;
	int y;

	int timeLeftX;
	int timeLeftY;

	float timeLeft;

	long nextRemove;

	SDL_Surface *icon;
	SDL_Surface *tSurface;

	SDL_Surface *name1;
	SDL_Surface *name2;

	int name1X;
	int name2X;

	int name1Y;
	int name2Y;

	int lvl;
	int effect;
	int changeEffect;
};

class Mark
{
public:
	Mark()
	{
		this->enable = false;

		this->leftdown = NULL;
		this->rightdown = NULL;
		this->leftup = NULL;
		this->rightup = NULL;

		this->leftdX = 0;
		this->leftdY = 0;
		this->rightdX = 0;
		this->rightdY = 0;

		this->leftuX = 0;
		this->leftuY = 0;
		this->rightuX = 0;
		this->rightuY = 0;

		this->npcmark = 0;
		this->marktype = 0;
	}

	void Load();

	bool enable;

	SDL_Surface *leftdown;
	SDL_Surface *rightdown;
	SDL_Surface *leftup;
	SDL_Surface *rightup;

	int leftdX;
	int leftdY;
	int rightdX;
	int rightdY;

	int leftuX;
	int leftuY;
	int rightuX;
	int rightuY;

	int npcmark;
	int marktype;
};

class Item
{
public:
	Item()
	{
		this->pic = NULL;
		this->amount = NULL;

		this->id = 0;

		this->stack = 0;
		this->value = 0;

		this->x = 0;
		this->y = 0;

		this->slot = 0;

		this->maxstack = 1;

		this->val1 = 0;
		this->val2 = 0;
		this->val3 = 0;

		this->effect = 0;
	}

	SDL_Surface *pic;
	SDL_Surface *amount;

	int id;

	int stack;
	int value;

	int x;
	int y;

	int slot;

	int maxstack;

	int val1;
	int val2;
	int val3;

	int effect;
};

class Drop
{
public:
	Drop()
	{
		this->itemid = 0;
		this->itemvalue = 0;
		this->id = 0;

		this->x = 0;
		this->y = 0;

		this->cX = 0;
		this->cY = 0;

		this->pic = NULL;

		this->inWin = false;

		this->dropNext = 0;
	}

	int itemid;
	int itemvalue;
	int id;

	int x;
	int y;

	int cX;
	int cY;

	SDL_Surface *pic;

	bool inWin;

	long dropNext;

	int val1;
	int val2;
	int val3;
};

class Inventory
{
public:
	Inventory()
	{
		for(unsigned int i = 0; i < 25; i++)
			this->items[i] = NULL;

		for(unsigned int i = 0; i < 4; i++)
			this->eq[i] = NULL;

		this->inv = NULL;
		this->golds = NULL;
		this->quitx = NULL;

		this->x = 0;
		this->y = 0;

		this->gold = 0;

		this->enable = false;
	}

	Item *items[25];
	Item *eq[4];

	SDL_Surface *inv;
	SDL_Surface *golds;
	SDL_Surface *quitx;

	int x;
	int y;

	int gold;

	bool enable;
};

class Picture
{
public:
	Picture()
	{
		this->item = NULL;
		this->stack = NULL;

		this->x = 0;
		this->y = 0;

		this->itemX = 0;
		this->itemY = 0;

		this->type = 0;
		this->slot = 0;
		this->itemid = 0;

		this->enable = false;

		this->value = 0;
		this->effect = 0;

		this->eqinv = false;

		this->coold = NULL;
		this->coolDown = 0;
		this->cooldsec = 0;
	}

	SDL_Surface *item;
	SDL_Surface *stack;
	SDL_Surface *coold;

	int x;
	int y;

	int itemX;
	int itemY;

	int type;
	int slot;
	int itemid;

	bool enable;

	int value;
	int effect;

	bool eqinv;

	long coolDown;
	long cooldsec;
};

class Taskbar
{
public:
	Taskbar()
	{
		for(int i = 0; i < 9; i++)
			this->pic[i] = NULL;

		this->bar = NULL;
		this->menu = NULL;

		this->enableTask = false;
		this->enablePress = false;
		this->enableMenu = false;

		this->x = 0;
		this->y = 0;
	}

	Picture *pic[9];

	SDL_Surface *bar;
	SDL_Surface *menu;

	bool enableTask;
	bool enablePress;
	bool enableMenu;

	int x;
	int y;
};

class SkillWindow
{
public:
	SkillWindow()
	{
		this->enable = false;

		this->win = NULL;
		this->quitx = NULL;

		this->x = 0;
		this->y = 0;
	}

	bool enable;

	SDL_Surface *win;
	SDL_Surface *quitx;

	int x;
	int y;
};

class Skill
{
public:
	Skill()
	{
		this->id = 0;

		this->x = 0;
		this->y = 0;

		this->pic = NULL;
		this->text = NULL;
		this->nameSkill = NULL;

		this->take = 0;
		this->takeamount = 0;
	}

	int id;

	int level;

	int x;
	int y;

	SDL_Surface *pic;
	SDL_Surface *text;
	SDL_Surface *nameSkill;

	int take;
	int takeamount;
};

class Button
{
public:
	Button()
	{
		this->x = 0;
		this->y = 0;

		this->sur = NULL;

		this->effect = 0;
	}

	int x;
	int y;

	SDL_Surface *sur;

	int effect;
};

class Message
{
public:
	Message()
	{
		this->win = NULL;
		this->text = NULL;
		this->head = NULL;

		this->x = 0;
		this->y = 0;
	}

	SDL_Surface *win;
	SDL_Surface *text;
	SDL_Surface *head;

	int x;
	int y;

	std::vector<Button*> btn;
};

class FourInt
{
public:
	FourInt()
	{
		this->int1 = 0;
		this->int2 = 0;
		this->int3 = 0;
		this->int4 = 0;
		this->int5 = 0;
	}

	int int1;
	int int2;
	int int3;
	int int4;
	int int5;
};

class SurfaceWithInt
{
public:
	SurfaceWithInt()
	{
		this->s = NULL;

		this->i = 0;
	}

	SDL_Surface *s;

	int i;
};

class ObjectInfoWindow
{
public:
	ObjectInfoWindow()
	{
		this->enable = false;

		this->x = 0;
		this->y = 0;

		this->start = NULL;
		this->bar = NULL;
		this->stop = NULL;

		this->nBars = 0;

		this->currType = 0;
		this->currItem = 0;

		this->names = "";
	}

	bool enable;

	int x;
	int y;

	SDL_Surface *start;
	SDL_Surface *bar;
	SDL_Surface *stop;

	int nBars;

	std::vector<SurfaceWithInt> text;

	int currType;
	int currItem;

	std::string names;
};

class Option
{
public:
	Option()
	{
		this->effect = 0;

		this->active = false;

		this->name = NULL;
	}

	int effect;

	bool active;

	SDL_Surface *name;
};

class OptionWindow
{
public:
	OptionWindow()
	{
		this->enable = false;

		this->x = 0;
		this->y = 0;
		
		this->win = NULL;
		this->quitx = NULL;

		this->buttonFull = NULL;
		this->buttonEmpty = NULL;
	}

	bool enable;

	int x;
	int y;

	SDL_Surface *win;
	SDL_Surface *quitx;

	SDL_Surface *buttonFull;
	SDL_Surface *buttonEmpty;

	std::vector<Option> op;
};

struct ConfigOption
{
	char arb[160];
	char command[200];

	int int1;
	int int2;

	int param;
};

struct Drag
{
	int id;

	int x;
	int y;

	int type;
	int slot;

	SDL_Surface *pic;

	int qty;

	int *winX;
	int *winY;

	int *winSizeX;
	int *winSizeY;

	bool *winEnable;

	std::string name;
};

class ShopItem
{
public:
	ShopItem()
	{
		this->pic = NULL;
		this->picStack = NULL;

		this->x = 0;
		this->y = 0;

		this->itemId = 0;
		this->itemPrice = 0;
		this->itemQty = 0;
	}

	SDL_Surface *pic;
	SDL_Surface *picStack;

	int x;
	int y;

	int itemId;
	int itemPrice;
	int itemQty;
};

class ShopWindow
{
public:
	ShopWindow()
	{
		this->win = NULL;
		this->quitx = NULL;

		this->x = 0;
		this->y = 0;

		this->enable = false;

		for(int i = 0; i < 20; i++)
		{
			this->si[i] = ShopItem();
			this->cart[i] = ShopItem();
		}

		this->currPrice = NULL;
		this->currQty = NULL;

		this->totalPrice = NULL;

		this->shopName = NULL;

		this->npcId = 0;

		this->cMark = 0;
	}

	void Clean();
	void SetupPos();
	void UpdateTotalPrice();

	SDL_Surface *win;
	SDL_Surface *quitx;

	int x;
	int y;

	bool enable;

	ShopItem si[20];
	ShopItem cart[20];

	SDL_Surface *currPrice;
	SDL_Surface *currQty;

	SDL_Surface *totalPrice;

	SDL_Surface *shopName;

	int npcId;

	int cMark;
};

class PlayerTrade
{
public:
	PlayerTrade()
	{
		this->win = NULL;
		this->quitx = NULL;

		this->ok = NULL;
		this->cancel = NULL;

		this->x = 0;
		this->y = 0;

		this->enable = false;

		for(int i = 0; i < 20; i++)
		{
			this->si[i] = ShopItem();
			this->si2[i] = ShopItem();
		}

		this->myPenya = NULL;
		this->otherPenya = NULL;

		this->penyaMe = 0;
		this->penyaOther = 0;

		this->tradeName1 = NULL;
		this->tradeName2 = NULL;
		this->status1 = NULL;
		this->status2 = NULL;

		this->mySide = RIGHT;

		this->side1 = 0;
		this->side2 = 0;
	}

	void Clean();
	void SetupPos();

	SDL_Surface *win;
	SDL_Surface *quitx;
	SDL_Surface *myPenya;
	SDL_Surface *otherPenya;
	SDL_Surface *ok;
	SDL_Surface *cancel;
	SDL_Surface *status1;
	SDL_Surface *status2;

	int penyaMe;
	int penyaOther;

	int mySide;

	int x;
	int y;

	bool enable;

	ShopItem si[20];
	ShopItem si2[20];

	SDL_Surface *tradeName1;
	SDL_Surface *tradeName2;

	int side1;
	int side2;
};

struct PKAREA
{
	int xMin;
	int xMax;

	int yMin;
	int yMax;
};

class Tile
{
public:
	Tile()
	{
		this->id = 0;
		this->walk = true;

		this->posx = 0;
		this->posy = 0;

		this->cX = 0;
		this->cY = 0;

		this->inWin = true;

		this->su = NULL;
		this->got = false;
	}

	int id;
	bool walk;

	int posx;
	int posy;

	int cX;
	int cY;

	bool inWin;
	
	SDL_Surface *su;
	bool got;
};

class Map
{
public:
	Map()
	{
		this->pkEnable = false;
		this->weaponEnable = false;

		this->xSize = 0;
		this->ySize = 0;

		this->mapId = 0;

		pkArea.clear();
		tiles.clear();

		this->camShowX = 0;
		this->camShowToX = 0;
		this->camShowY = 0;
		this->camShowToX = 0;
	}

	bool pkEnable;
	bool weaponEnable;

	int xSize;
	int ySize;

	int mapId;

	std::vector<PKAREA> pkArea;
	std::vector<Tile> tiles;

	int camShowX;
	int camShowToX;
	int camShowY;
	int camShowToY;
};

extern Drag dragged;

class InputWindow
{
public:
	InputWindow()
	{
		this->win = NULL;
		this->quitx = NULL;

		this->btn1 = NULL;
		this->btn2 = NULL;
		this->btn3 = NULL;
		this->btn4 = NULL;

		this->desc1 = NULL;
		this->desc2 = NULL;

		this->input = NULL;

		this->inputstr = "";

		this->type = 0;
		
		this->arg1 = 0;
		this->arg2 = 0;
		this->arg2TEXT = "";
		this->arg3 = 0;

		this->x = 0;
		this->y = 0;
	}

	void Clean(int i)
	{
		if(i == dragged.id && dragged.type == 2)
			drag = false;

		SDL_FreeSurface(this->win);
		SDL_FreeSurface(this->quitx);

		if(this->btn1 != NULL)
			SDL_FreeSurface(this->btn1);

		if(this->btn2 != NULL)
			SDL_FreeSurface(this->btn2);

		if(this->btn4 != NULL)
			SDL_FreeSurface(this->btn4);

		if(this->desc1 != NULL)
			SDL_FreeSurface(this->desc1);

		if(this->desc2 != NULL)
			SDL_FreeSurface(this->desc2);

		if(this->input != NULL)
			SDL_FreeSurface(this->input);
	}

	SDL_Surface *win;
	SDL_Surface *quitx;

	SDL_Surface *btn1;
	SDL_Surface *btn2;
	SDL_Surface *btn3;
	SDL_Surface *btn4;

	SDL_Surface *desc1;
	SDL_Surface *desc2;

	SDL_Surface *input;

	std::string inputstr;

	int type;

	int arg1;
	int arg2;
	std::string arg2TEXT;
	int arg3;

	int x;
	int y;
};

struct SFXSound
{
	int p;
	int channel;
	Mix_Chunk *s;
};

class SFXManager
{
public:
	SFXManager()
	{
	}

	~SFXManager()
	{
		for(unsigned int i = 0; i < sounds.size(); i++)
			Mix_FreeChunk(sounds.at(i).s);

		sounds.clear();
	}

	void playSound(int PARAM, int loop);
	void stopSound(int PARAM);
	void createSound(char *fname, int type, int param);
	void deleteSound(int PARAM);

	Mix_Chunk *getSound(int PARAM);

	std::vector<SFXSound> sounds;
};

class TeleportObject
{
public:
	TeleportObject()
	{
		this->x = 0;
		this->y = 0;

		this->target = "";
		this->cost = 0;

		this->world = NULL;
		this->price = NULL;
	}

	void Clean()
	{
		if(this->world != NULL)
			SDL_FreeSurface(this->world);

		if(this->price != NULL)
			SDL_FreeSurface(this->price);

		this->x = 0;
		this->y = 0;

		this->target = "";
		this->cost = 0;

		this->world = NULL;
		this->price = NULL;
	}

	int x;
	int y;

	std::string target;
	int cost;

	SDL_Surface *world;
	SDL_Surface *price;
};

class TeleportWindow
{
public:
	TeleportWindow()
	{
		this->x = 0;
		this->y = 0;

		this->npcId = 0;

		this->win = NULL;
		this->quitx = NULL;

		this->npcText = NULL;

		this->enable = false;
	}

	void Clean()
	{
		for(unsigned int i = 0; i < this->tps.size(); i++)
			this->tps.at(i).Clean();

		this->tps.clear();

		this->npcId = 0;

		if(this->npcText != NULL)
			SDL_FreeSurface(this->npcText);

		this->npcText = NULL;
	}

	int x;
	int y;

	SDL_Surface *win;
	SDL_Surface *quitx;

	std::vector<TeleportObject> tps;

	bool enable;

	int npcId;
	SDL_Surface *npcText;
};

class LoginMessage
{
public:
	LoginMessage()
	{
		this->win = NULL;
		this->names = NULL;
		
		this->effect = 0;

		this->x = 0;
		this->y = 0;
	}

	void Clean()
	{
		if(this->win != NULL)
			SDL_FreeSurface(this->win);

		if(this->names != NULL)
			SDL_FreeSurface(this->names);
	}

	SDL_Surface *win;
	SDL_Surface *names;

	int effect;

	int x;
	int y;
};

class CharacterSelect
{
public:
	CharacterSelect()
	{
		this->name = NULL;
		this->str = NULL;
		this->sta = NULL;
		this->dex = NULL;
		this->title = NULL;
		this->level = NULL;
		this->createHaha = NULL;
		this->deletes = NULL;
		this->intt = NULL;
		this->job = NULL;

		this->slot = 0;

		this->name2 = "";
	}

	SDL_Surface *name;
	SDL_Surface *str;
	SDL_Surface *sta;
	SDL_Surface *dex;
	SDL_Surface *title;
	SDL_Surface *level;
	SDL_Surface *createHaha;
	SDL_Surface *deletes;
	SDL_Surface *intt;
	SDL_Surface *job;

	int slot;

	std::string name2;
};

class LoginInput
{
public:
	LoginInput()
	{
		this->win = NULL;

		this->t = 0;
		this->i = 0;
	}

	SDL_Surface *win;

	int t;
	int i;
};

class LoginWriter
{
public:
	LoginWriter()
	{
		this->ar1 = NULL;
		this->ar2 = NULL;
		
		this->carea = 0;

		this->ar1s = "";
		this->ar2s = "";

		this->enable = false;
		this->enable1 = false;
		this->enable2 = false;

		this->place1 = 0;
		this->place2 = 0;
	}

	void StartWrite(bool a1, bool a2, int x1, int y1, int x2, int y2, int carea1)
	{
		if(this->ar1 != NULL)
			SDL_FreeSurface(this->ar1);

		this->ar1 = NULL;

		if(this->ar2 != NULL)
			SDL_FreeSurface(this->ar2);

		this->ar2 = NULL;

		this->carea = carea1;

		this->ar1s = "";
		this->ar2s = "";

		this->wr1x = x1;
		this->wr1y = y1;
		this->wr2x = x2;
		this->wr2y = y2;

		this->enable = true;
		this->enable1 = a1;
		this->enable2 = a2;

		this->place1 = 0;
		this->place2 = 0;

		SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);
	}

	void StopWrite()
	{
		if(this->ar1 != NULL)
			SDL_FreeSurface(this->ar1);

		this->ar1 = NULL;

		if(this->ar2 != NULL)
			SDL_FreeSurface(this->ar2);

		this->ar2 = NULL;

		this->carea = 0;

		this->ar1s = "";
		this->ar2s = "";

		this->wr1x = 0;
		this->wr1y = 0;
		this->wr2x = 0;
		this->wr2y = 0;

		this->enable = false;
		this->enable1 = false;
		this->enable2 = false;

		this->place1 = 0;
		this->place2 = 0;

		SDL_EnableKeyRepeat(0, SDL_DEFAULT_REPEAT_INTERVAL);

		isMasked = 0;
	}

	const char *GetText(int etc)
	{
		if(etc == 1)
			return this->ar1s.c_str();

		else
			return this->ar2s.c_str();
	}

	SDL_Surface *GetSurface(int etc)
	{
		if(etc == 1)
			return this->ar1;

		else
			return this->ar2;
	}

	void NewSurface(int etc);
	void ShowStrings();

	void ChangeFocus(int s)
	{
		if(s == 1)
		{
			if(this->enable2)
				this->carea = 2;
		}

		else if(s == 2)
		{
			if(this->enable1)
				this->carea = 1;
		}

		else if(s == 3)
		{
			if(this->carea == 1)
			{
				if(this->enable2)
					this->carea = 2;
			}

			else if(this->carea == 2)
			{
				if(this->enable1)
					this->carea = 1;
			}
		}
	}

	int GetWriterW(int etc)
	{
		int rets = 0;

		if(etc == 1)
		{
			rets += this->wr1x;

			if(this->ar1 != NULL)
				rets += this->ar1->w;
		}

		else if(etc == 2)
		{
			rets += this->wr2x;

			if(this->ar2 != NULL)
				rets += this->ar2->w;
		}

		return rets;
	}

	int GetWriterH(int etc)
	{
		int rets = 0;

		if(etc == 1)
			rets += this->wr1y;

		else if(etc == 2)
			rets += this->wr2y;

		return rets;
	}

	int GetWriteFocus()
	{
		return this->carea;
	}

	void Clean()
	{
		if(this->ar1 != NULL)
			SDL_FreeSurface(this->ar1);

		this->ar1 = NULL;

		if(this->ar2 != NULL)
			SDL_FreeSurface(this->ar2);

		this->ar2 = NULL;
	}

	SDL_Surface *ar1;
	SDL_Surface *ar2;

	int carea;

	std::string ar1s;
	std::string ar2s;

	int wr1x;
	int wr1y;
	int wr2x;
	int wr2y;

	bool enable;
	bool enable1;
	bool enable2;

	int place1;
	int place2;

	bool cleaned;
};

DWORD WINAPI RECVTHREAD(LPVOID lpParam);
DWORD WINAPI PROCESS(LPVOID lpParam);

void UpdateAll();
void UpdateImage(SDL_Surface *_image, int _x, int _y);
void UpdateLoadingImage();

SDL_Surface *getSurface(int PARAM);
SDL_Surface *getMonsterSurface(int PARAM);
SDL_Surface *getIconSurface(int PARAM);

void PreLoad();
void CleanPreLoad();

void StartUp();
void CleanUp();

void RenderThread(bool &move);

#endif
#ifndef MISC_H
#define MISC_H

#define DRAGSKILLWINDOW 1
#define DRAGINPUTWINDOW 2
#define DRAGTRADEWINDOW 3
#define DRAGINVENTORYWINDOW 4
#define DRAGMSGWINDOW 5
#define DRAGPARTINVITE 6
#define DRAGADDSTATWINDOW 7
#define DRAGOPTIONWINDOW 8
#define DRAGWINDOW 9
#define DRAGPARTYWINDOW 10
#define DRAGTELEPORTWINDOW 11
#define DRAGNPCSHOPWINDOW 12
#define DRAGBUFFS 13
#define DRAGFRIENDWINDOW 14
#define DRAGFRIENDLIST 15
#define DRAGCHATLIST 16
#define DRAGPMSWINDOW 17
#define DRAGPMSLIST 18

#define MAPX 1336
#define MAPY 1337

#include <SDL_ttf.h>

#include "Login.h"
#include "Preload.h"
#include "Game.h"

class Fragment
{
public:
	Fragment()
	{
		this->buf = NULL;
		this->len = 0;
	}

	unsigned char *buf;
	int len;
};

class CurrentPacket
{
public:
	CurrentPacket()
	{
		this->totallen = 0;
		this->fullbuf = NULL;
		this->waiting = false;
		this->iwant = 0;
	}

	void eraseme()
	{
		if(this->fullbuf != NULL)
			delete[] this->fullbuf;

		this->fullbuf = NULL;

		this->totallen = 0;

		for(unsigned int i = 0; i < this->frag.size(); i++)
		{
			if(this->frag.at(i).buf != NULL)
				delete[] this->frag.at(i).buf;
		}

		this->frag.clear();
		this->waiting = false;
		this->iwant = 0;
	}

	void compilefrag()
	{
		if(this->fullbuf != NULL)
			delete[] this->fullbuf;

		this->fullbuf = NULL;
		this->totallen = 0;

		if(this->frag.size() > 0)
		{
			for(unsigned int i = 0; i < this->frag.size(); i++)
				this->totallen += this->frag.at(i).len;

			this->fullbuf = new unsigned char[this->totallen];

			int cplace = 0;

			for(unsigned int i = 0; i < this->frag.size(); i++)
			{
				memcpy(this->fullbuf + cplace, this->frag.at(i).buf, this->frag.at(i).len);
				cplace += this->frag.at(i).len;
			}
		}
	}

	int getsize()
	{
		return this->frag.size();
	}

	int totallen;
	unsigned char *fullbuf;
	int iwant;

	std::vector<Fragment> frag;

	bool waiting;
};

class SendPacket
{
public:
	SendPacket()
	{
		this->bu = NULL;
		this->len = 0;
		this->cp = 0;
		this->forward = false;
	}

	unsigned char *bu;
	int len;
	int cp;
	bool forward;
};

bool CharacterSlotExist(std::vector<int> s, int c);

void CleanPlayer(Character *p);

bool OverButton(int buttonx, int buttony, int buttonw, int buttonh, int mousex, int mousey);

void MarkMe(NPC *tar);
void MarkMePlayer(Character *tar);
void MarkMeMonster(Monster *tar);
void UpdateMark(NPC *tar);
void UpdateMarkPlayer(Character *tar);
void UpdateMarkMonster(Monster *tar);

bool InRange(char *name);

int Random(int min, int max);

bool NotInsideMe(Monster *m, Character *c);
bool NotInsideMePK(Character *m, Character *c);

bool WalkTooFar(Monster *m);

SDL_Surface *loadimage(const char *path, bool trans, int r, int g, int b);

void PressTaskbar(int num);

bool fullInventory();
bool fullInventoryItem(int id);

int recvpacket(unsigned char *buf, unsigned char *buf2);

bool OnMap(int x, int y);

void CreateSystemMessage(char *label, char *text);

void Bot();
void SetKeyDeactivate(int key);
bool RemoveAllMoveKeys(int key);

int getEquipPosX(int slot);
int getEquipPosY(int slot);

std::string getItemEffect(int effect);
std::string getItemEffectShop(int effect);

NPC *getNPCById(int id);
Character *getPlayerById(int id);
Monster *getMonsterById(int id);

int GetAttackDelay();

bool PlayerKillArea(Character *c);

void pingPacket();

bool thisMarkedMonster(Monster *m);
bool thisMarkedPlayer(Character *p);

void MoveToPlayer(Character *fol, Character *tar);

void MeMove(int DIR, float Vel);

void CreateInputWindow(int type, int numOfArgs, ...);

void AddChatText(SDL_Event &event, std::string &tempSTR, int ts);

unsigned int getTime(bool fast);

void CreateNameSurface(NameSurface *sur, TTF_Font *font, SDL_Color clr, char *string, int type);
int CompareColors(SDL_Color clr1, SDL_Color clr2);

void StopMusic();

SDL_Surface *RenderText_Solid(TTF_Font *font, const char *string, SDL_Color clr);
SDL_Surface *RenderText_Blended(TTF_Font *font, const char *string, SDL_Color clr);

void StartDragWindow(int t, int x, int y, int *sx, int *sy, int i, bool *enable, int *winX, int *winY);
void MoveKoblaWindow(int x, int y);

void SetupInventory();

void KMessageBox(char *titel, UINT params, char *text, ...);
void LMessage(int effect, char *text, ...);

void CreateLoginInput(int TYPE, int i);

int getInventoryStack(int id);
SDL_Surface *getInventoryStackPic(int id);

void UpdateTaskbar(int type, int id, int slot);
void UpdateBuffPosition();

std::string getItemExtraValue(int value, int proc2);

int msize(int pav);
void LoadNewMap(int maps);

void CopySkillInfo(InfoSkill *cs, Skill *ts);
void SetSkillLevel(int lvl, int id);

int InitFD();
int HandlePackets(unsigned char *buf, int by, bool &move);

bool AttackedTarget(Bullet *b, int id, int type, bool doit);

bool BotBullet(float xTemp, Bullet *b, bool changeTarget);

void SetTrans(bool what);

#endif
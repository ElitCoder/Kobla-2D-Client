#include <vector>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <fstream>
#include <assert.h>
#include <vector>

#include "Misc.h"
#include "Packets.h"
#include "Packet.h"
#include "Process.h"
#include "Game.h"
#include "Preload.h"
#include "Music.h"
#include "Main.h"
#include "Motion.h"
#include "Friend.h"

using namespace std;

extern Mark *mark;
extern Taskbar *task;
extern SkillWindow *sk;
extern Inventory *inv;
extern ShopWindow *sw;
extern PlayerTrade *ptrade;
extern TeleportWindow *tp;
extern OptionWindow *option;
extern FriendWindow *fw;
extern ChatManager *chatFunc;
extern PrivateMessage *pms;

extern SDL_Surface *screen;

extern TTF_Font *cour;
extern TTF_Font *courMedium;
extern TTF_Font *courBold;
extern TTF_Font *courOutline;

extern SDL_Color clrFgBlack;

extern bool colorkey;

extern Character *me;

extern std::vector<Character*> cv;
extern std::vector<Monster*> cm;
extern std::vector<NPC*> npc;

extern int camShowX;
extern int camShowToX;
extern int camShowY;
extern int camShowToY;

long nextTask[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

extern SOCKET sock;

extern vector<int> programex;

extern vector<Window*> win;
extern vector<AddStatWindow*> addstatwindow;
extern vector<PartyWindow*> partyw;
extern vector<PartyInvite*> pi;
extern vector<Message*> msgs;
extern vector<InputWindow*> iw;
extern vector<LoginMessage*> lmsgs;
extern vector<LoginInput*> linputs;
extern vector<Icon*> icons;
extern vector<Skill*> skill;

int botTarget = 0;
int botType = 0;
int currentMark = 0;

extern bool keys[365];
extern int keyPressed;

extern int currMoveWay;

extern Map *map;

extern LARGE_INTEGER pingTimes;
extern SDL_Surface *pingSurface;
extern SDL_Color clrFg;
extern SDL_Color clrFgBlue;
extern bool sentPingPacket;

extern float currBackY;
extern float currBackX;

extern float cameraX;
extern float cameraY;

extern bool chatWindow;
extern long nextInput;
extern int chatTries;
extern string lastInput;
extern string chat;
extern SDL_Surface *chatInfo;

extern bool chatInputWindow;

extern MusicManager *music;

extern bool noSound;

extern Drag dragged;
extern bool drag;

extern int logplace;
extern int isMasked;

int lastPing = 0;

extern Skill *skills;
extern int skillLoaded;

extern fd_set f_readset, f_writeset, f_exset;

extern CurrentPacket cp;
extern std::vector<SendPacket> toserv;

extern bool dc;
extern bool transWindow;

extern int resX;
extern int resY;

extern int bonusAttackSpeed;

extern SDL_Surface *skillShow;

bool CharacterSlotExist(std::vector<int> s, int c)
{
	bool ret = false;

	for(unsigned int i = 0; i < s.size(); i++)
	{
		if(s.at(i) == c)
		{
			ret = true;
			break;
		}
	}

	return ret;
}

void CleanPlayer(Character *p)
{
	strcpy_s(p->name, "");
	p->level = 1;
	p->power = 0;
	p->magicpower = 0;
	p->attack = 0;
	p->accuracy = 0;
	p->agility = 0;
	p->hpPoint = 0;
	p->pAv = 0;
	p->id = 0;
	p->x = 0.0f;
	p->y = 0.0f;
	p->cMove = RIGHT;
	p->nameSur.clean();
	p->weapon = NULL;

	if(p->hat != NULL)
		SDL_FreeSurface(p->hat);

	p->hat = NULL;

	if(p->suit != NULL)
		SDL_FreeSurface(p->suit);

	p->suit = NULL;

	if(p->belt != NULL)
		SDL_FreeSurface(p->belt);

	p->belt = NULL;

	p->weaponid = 0;
	p->hatid = 0;
	p->suitid = 0;
	p->beltid = 0;

	if(p->c != NULL)
	{
		if(p->c->theWindow != NULL)
			SDL_FreeSurface(p->c->theWindow);

		p->c->theWindow = NULL;

		if(p->c->text1 != NULL)
			SDL_FreeSurface(p->c->text1);

		p->c->text1 = NULL;

		if(p->c->text2 != NULL)
			SDL_FreeSurface(p->c->text2);

		p->c->text2 = NULL;

		delete p->c;
		p->c = NULL;
	}

	p->wMove = RIGHT;
	p->moving = false;
	p->map = 0;
	p->hp = 0;
	p->maxhp = 0;
	p->speed = 0;
	p->equipped = false;
	p->dead = false;
	p->exp = 0.0f;
	p->inParty = false;
	p->cX = 0;
	p->cY = 0;
	p->inWin = false;
	p->gold = 0;
	p->pixSec = 0;
	p->proc = 0;
	p->showProc = false;
	p->timerProc = 0;
	p->enableTimer = false;
	p->procentType = 0;

	for(unsigned int i = 0; i < p->menu.size(); i++)
	{
		if(p->menu.at(i).tSur != NULL)
			SDL_FreeSurface(p->menu.at(i).tSur);

		p->menu.at(i).tSur = NULL;

		if(p->menu.at(i).btn != NULL)
			SDL_FreeSurface(p->menu.at(i).btn);

		p->menu.at(i).btn = NULL;
	}

	p->menu.clear();

	p->showMenu = false;
	p->followId = 0;
	p->follow = false;

	if(p->manaSurface != NULL)
		SDL_FreeSurface(p->manaSurface);

	p->manaSurface = NULL;

	if(p->energySurface != NULL)
		SDL_FreeSurface(p->energySurface);

	p->energySurface = NULL;
}

bool OverButton(int buttonx, int buttony, int buttonw, int buttonh, int mousex, int mousey)
{
	if((mousex >= buttonx) && (mousex <= (buttonx + buttonw)) && (mousey >= buttony) && (mousey <= buttony + buttonh))
		return true;

	else
		return false;
}

void Mark::Load()
{
	this->enable = false;

	this->leftdown = loadimage("Data/Icon/LeftDownMark-Icon.png", true, 255, 255, 255);
	this->rightdown = loadimage("Data/Icon/RightDownMark-Icon.png", true, 255, 255, 255);
	this->leftup = loadimage("Data/Icon/LeftUpMark-Icon.png", true, 255, 255, 255);
	this->rightup = loadimage("Data/Icon/RightUpMark-Icon.png", true, 255, 255, 255);
}

void MarkMe(NPC *tar)
{
	for(unsigned int i = 0; i < cv.size(); i++)
	{
		if(cv.at(i)->showMenu)
			cv.at(i)->showMenu = false;
	}

	mark->leftdX = ((tar->cX - 7) - mark->leftdown->w);
	mark->leftdY = ((tar->cY + tar->my->h) + 7);

	mark->rightdX = ((tar->cX + 7) + tar->my->w);
	mark->rightdY = ((tar->cY + tar->my->h) + 7);

	mark->leftuX = ((tar->cX - 7) - mark->leftup->w);
	mark->leftuY = ((tar->cY - 7) - mark->leftup->h);

	mark->rightuX = ((tar->cX + 7) + tar->my->w);
	mark->rightuY = ((tar->cY - 7) - mark->leftup->h);

	mark->enable = true;
	tar->showMenu = true;

	mark->marktype = 3;
	mark->npcmark = tar->id;
}

void MarkMePlayer(Character *tar)
{
	mark->leftdX = ((tar->cX - 7) - mark->leftdown->w);
	mark->leftdY = ((tar->cY + tar->my->h) + 7);

	mark->rightdX = ((tar->cX + 7) + tar->my->w);
	mark->rightdY = ((tar->cY + tar->my->h) + 7);

	mark->leftuX = ((tar->cX - 7) - mark->leftup->w);
	mark->leftuY = ((tar->cY - 7) - mark->leftup->h);

	mark->rightuX = ((tar->cX + 7) + tar->my->w);
	mark->rightuY = ((tar->cY - 7) - mark->leftup->h);

	mark->enable = true;
	tar->showMenu = true;
	tar->showProc = true;

	mark->marktype = 1;
	mark->npcmark = tar->id;
}

void MarkMeMonster(Monster *tar)
{
	mark->leftdX = ((tar->cX - 7) - mark->leftdown->w);
	mark->leftdY = ((tar->cY + tar->m->h) + 7);

	mark->rightdX = ((tar->cX + 7) + tar->m->w);
	mark->rightdY = ((tar->cY + tar->m->h) + 7);

	mark->leftuX = ((tar->cX - 7) - mark->leftup->w);
	mark->leftuY = ((tar->cY - 7) - mark->leftup->h);

	mark->rightuX = ((tar->cX + 7) + tar->m->w);
	mark->rightuY = ((tar->cY - 7) - mark->leftup->h);

	mark->enable = true;
	mark->marktype = 2;

	mark->npcmark = tar->mId;
}

void UpdateMark(NPC *tar)
{
	if(tar != NULL)
	{
		if(((tar->x + tar->my->w) >= camShowX && camShowToX >= tar->x) && ((tar->y + tar->my->h) >=camShowY && camShowToY >= tar->y))
		{
			int t = screen->w;
			int t2 = screen->h;

			tar->cX = (camShowToX - tar->x);
			tar->cY = (camShowToY - tar->y);

			t -= tar->cX;
			t2 -= tar->cY;

			tar->cX = t;
			tar->cY = t2;
		}

		mark->leftdX = ((tar->cX - 7) - mark->leftdown->w);
		mark->leftdY = ((tar->cY + tar->my->h) + 7);

		mark->rightdX = ((tar->cX + 7) + tar->my->w);
		mark->rightdY = ((tar->cY + tar->my->h) + 7);

		mark->leftuX = ((tar->cX - 7) - mark->leftup->w);
		mark->leftuY = ((tar->cY - 7) - mark->leftup->h);

		mark->rightuX = ((tar->cX + 7) + tar->my->w);
		mark->rightuY = ((tar->cY - 7) - mark->leftup->h);

		mark->enable = true;
		mark->marktype = 3;
	}

	else
	{
		mark->enable = false;
		mark->marktype = 0;
		mark->npcmark = 0;
	}
}

void UpdateMarkPlayer(Character *tar)
{
	if(tar != NULL)
	{
		if(((tar->x + tar->my->w) >= camShowX && camShowToX >= tar->x) && ((tar->y + tar->my->h) >=camShowY && camShowToY >= tar->y))
		{
			int t = screen->w;
			int t2 = screen->h;

			tar->cX = (int)(camShowToX - tar->x);
			tar->cY = (int)(camShowToY - tar->y);

			t -= tar->cX;
			t2 -= tar->cY;

			tar->cX = t;
			tar->cY = t2;
		}

		mark->leftdX = ((tar->cX - 7) - mark->leftdown->w);
		mark->leftdY = ((tar->cY + tar->my->h) + 7);

		mark->rightdX = ((tar->cX + 7) + tar->my->w);
		mark->rightdY = ((tar->cY + tar->my->h) + 7);

		mark->leftuX = ((tar->cX - 7) - mark->leftup->w);
		mark->leftuY = ((tar->cY - 7) - mark->leftup->h);

		mark->rightuX = ((tar->cX + 7) + tar->my->w);
		mark->rightuY = ((tar->cY - 7) - mark->leftup->h);

		mark->enable = true;
		mark->marktype = 1;
	}

	else
	{
		mark->enable = false;
		mark->marktype = 0;
		mark->npcmark = 0;
	}
}

void UpdateMarkMonster(Monster *tar)
{
	if(tar != NULL)
	{
		if(((tar->x + tar->m->w) >= camShowX && camShowToX >= tar->x) && ((tar->y + tar->m->h) >=camShowY && camShowToY >= tar->y))
		{
			int t = screen->w;
			int t2 = screen->h;

			tar->cX = (int)(camShowToX - tar->x);
			tar->cY = (int)(camShowToY - tar->y);

			t -= tar->cX;
			t2 -= tar->cY;

			tar->cX = t;
			tar->cY = t2;
		}

		mark->leftdX = ((tar->cX - 7) - mark->leftdown->w);
		mark->leftdY = ((tar->cY + tar->m->h) + 7);

		mark->rightdX = ((tar->cX + 7) + tar->m->w);
		mark->rightdY = ((tar->cY + tar->m->h) + 7);

		mark->leftuX = ((tar->cX - 7) - mark->leftup->w);
		mark->leftuY = ((tar->cY - 7) - mark->leftup->h);

		mark->rightuX = ((tar->cX + 7) + tar->m->w);
		mark->rightuY = ((tar->cY - 7) - mark->leftup->h);

		mark->enable = true;
		mark->marktype = 2;
	}

	else
	{
		mark->enable = false;
		mark->marktype = 0;
		mark->npcmark = 0;
	}
}

bool InRange(char *name)
{
	if(strcmp(name, me->name) == 0)
		return true;

	for(unsigned int i = 0; i < cv.size(); i++)
	{
		if((cv.at(i)->cX >= camShowX && camShowToX >= cv.at(i)->cX) && (cv.at(i)->cY >=camShowY && camShowToY >= cv.at(i)->cY))
			return true;
	}

	for(unsigned int i = 0; i < npc.size(); i++)
	{
		if((npc.at(i)->cX >= camShowX && camShowToX >= npc.at(i)->cX) && (npc.at(i)->cY >=camShowY && camShowToY >= npc.at(i)->cY))
			return true;
	}

	return false;
}

int Random(int min, int max)
{
	int typeRange = ((max - min) + 1);
	int type = (min + int(typeRange * rand() / (RAND_MAX + 1.0)));

	return type;
}

bool NotInsideMe(Monster *m, Character *c)
{
	if(c->wMove == LEFT && c->equipped)
	{
		if(((c->x - m->m->w - c->weapon->w - 3) <= m->x && (c->x + c->my->w) >= m->x) && ((c->y - m->m->h) <= m->y && (c->y + c->my->h) >= m->y))
			return false;
		else
			return true;
	}

	else if(c->wMove == RIGHT && c->equipped)
	{
		if(((c->x - m->m->w) <= m->x && (c->x + c->my->w + c->weapon->w + 3) >= m->x) && ((c->y - m->m->h) <= m->y && (c->y + c->my->h) >= m->y))
			return false;
		else
			return true;
	}

	else if(!c->equipped)
	{
		if(((c->x - m->m->w) <= m->x && (c->x + c->my->w) >= m->x) && ((c->y - m->m->h) <= m->y && (c->y + c->my->h) >= m->y))
			return false;
		else
			return true;
	}

	return true;
}

bool NotInsideMePK(Character *m, Character *c)
{
	if(c->wMove == LEFT && c->equipped)
	{
		if(((c->x - m->my->w - c->weapon->w - 3) <= m->x && (c->x + c->my->w) >= m->x) && ((c->y - m->my->h) <= m->y && (c->y + c->my->h) >= m->y))
			return false;
		else
			return true;
	}

	else if(c->wMove == RIGHT && c->equipped)
	{
		if(((c->x - m->my->w) <= m->x && (c->x + c->my->w + c->weapon->w + 3) >= m->x) && ((c->y - m->my->h) <= m->y && (c->y + c->my->h) >= m->y))
			return false;
		else
			return true;
	}

	else if(!c->equipped)
	{
		if(((c->x - m->my->w) <= m->x && (c->x + c->my->w) >= m->x) && ((c->y - m->my->h) <= m->y && (c->y + c->my->h) >= m->y))
			return false;
		else
			return true;
	}

	return true;
}

SDL_Surface *loadimage(const char *path, bool trans, int r, int g, int b)
{
	bool accept = true;

	SDL_Surface *temp1 = IMG_Load(path);

	if(temp1 == NULL)
	{
		Log("Error, could not load file: %s.", path);
		accept = false;
	}

	if(accept)
	{
		if(colorkey && trans)
			SDL_SetColorKey(temp1, SDL_SRCCOLORKEY | SDL_RLEACCEL, SDL_MapRGB(temp1->format, r, g, b));

		SDL_Surface *temp2 = SDL_DisplayFormat(temp1);

		SDL_FreeSurface(temp1);
	
		return temp2;
	}

	else
		return NULL;
}

int getFirstItemSlotWithId(int id)
{
	for(int i = 0; i < 25; i++)
	{
		if(inv->items[i] != NULL)
		{
			if(inv->items[i]->id == id)
				return (i + 1);
		}
	}

	return 0;
}

void PressTaskbar(int num)
{
	if((unsigned)nextTask[(num - 1)] <= getTime(true) && !sw->enable)
	{
		try
		{
			if(task->pic[(num - 1)]->enable)
			{
				if(task->pic[(num - 1)]->type == INVENTORYITEM)
				{
					if(getItemStack(task->pic[(num - 1)]->itemid) > 1)
						PacketInventoryUseItem(getFirstItemSlotWithId(task->pic[(num - 1)]->itemid));

					else
						PacketUseTaskbarItem(num);
				}

				else
					PacketUseTaskbarItem(num);
			}
		}

		catch(...)
		{
			Log("Error at ::PressTaskbar().");
		}

		nextTask[(num - 1)] = (getTime(true) + 100);
	}
}

bool fullInventory()
{
	for(int i = 0; i < 25; i++)
	{
		if(inv->items[i] == NULL)
			return false;
		else
		{
			if(inv->items[i]->maxstack != inv->items[i]->stack)
				return false;
		}
	}

	return true;
}

bool fullInventoryItem(int id)
{
	if(id == 2)
		return false;

	for(int i = 0; i < 25; i++)
	{
		if(inv->items[i] == NULL)
			return false;

		else
		{
			if(inv->items[i]->id == id)
			{
				if(inv->items[i]->maxstack != inv->items[i]->stack)
					return false;
			}
		}
	}

	return true;
}

bool OnMap(int x, int y)
{
	if(partyw.at(0)->enable)
	{
		if(OverButton(partyw.at(0)->x, partyw.at(0)->y, partyw.at(0)->window->w, partyw.at(0)->window->h, x, y))
			return false;
	}

	if(addstatwindow.at(0)->enable)
	{
		if(OverButton(addstatwindow.at(0)->x, addstatwindow.at(0)->y, addstatwindow.at(0)->window->w, addstatwindow.at(0)->window->h, x, y))
			return false;
	}

	if(task->enableTask)
	{
		if(OverButton(task->x, task->y, task->bar->w, task->bar->h, x, y))
			return false;
	}

	if(task->enableMenu)
	{
		if(OverButton(((task->x + task->bar->w) - task->menu->w), (task->y - task->menu->h), task->menu->w, task->menu->h, x, y))
			return false;
	}

	if(inv->enable)
	{
		if(OverButton(inv->x, inv->y, inv->inv->w, inv->inv->h, x, y))
			return false;
	}

	if(sk->enable)
	{
		if(OverButton(sk->x, sk->y, sk->win->w, sk->win->h, x, y))
			return false;
	}

	if(pi.size() > 0)
	{
		for(unsigned int i = 0; i < pi.size(); i++)
		{
			if(OverButton(pi.at(i)->x, pi.at(i)->y, pi.at(i)->win->w, pi.at(i)->win->h, x, y))
				return false;
		}
	}

	if(msgs.size() > 0)
	{
		for(unsigned int i = 0; i < msgs.size(); i++)
		{
			if(OverButton(msgs.at(i)->x, msgs.at(i)->y, msgs.at(i)->win->w, msgs.at(i)->win->h, x, y))
				return false;
		}
	}

	if(win.size() > 0)
	{
		for(unsigned int i = 0; i < win.size(); i++)
		{
			if(OverButton(win.at(i)->x, win.at(i)->y, win.at(i)->w->w, win.at(i)->w->h, x, y))
				return false;
		}
	}

	if(sw->enable)
	{
		if(OverButton(sw->x, sw->y, sw->win->w, sw->win->h, x, y))
			return false;
	}

	if(ptrade->enable)
	{
		if(OverButton(ptrade->x, ptrade->y, ptrade->win->w, ptrade->win->h, x, y))
			return false;
	}

	if(iw.size() > 0)
	{
		for(unsigned int i = 0; i < iw.size(); i++)
		{
			if(OverButton(iw.at(i)->x, iw.at(i)->y, iw.at(i)->win->w, iw.at(i)->win->h, x, y))
				return false;
		}
	}

	if(tp->enable)
	{
		if(OverButton(tp->x, tp->y, tp->win->w, tp->win->h, x, y))
			return false;
	}

	return true;
}

void CreateSystemMessage(char *label, char *text)
{
	Message *msg = new Message();
	Button *btn = new Button();

	msg->win = loadimage("Data/Interface/Message-Map.png", false, 0, 0, 0);
	msg->x = ((screen->w / 2) - (msg->win->w / 2));
	msg->y = (screen->h / 10);
	msg->text = RenderText_Solid(courMedium, text, clrFgBlack);
	msg->head = RenderText_Solid(cour, label, clrFgBlack);

	if(transWindow)
		SDL_SetAlpha(msg->win, SDL_SRCALPHA | SDL_RLEACCEL, 128);

	btn->effect = DISCONNECTED;
	btn->sur = loadimage("Data/Interface/OK-Button.png", true, 255, 255, 255);
	btn->y = (msg->y + 81);
	btn->x = ((msg->x + ((msg->win->w / 2) - (btn->sur->w / 2))));

	msg->btn.push_back(btn);
	msgs.push_back(msg);
}

void CreateInputWindow(int type, int numOfArgs, ...)
{
	InputWindow *iws = new InputWindow();

	char temp[20];

	va_list ap;
	va_start(ap, numOfArgs);

	iws->win = loadimage("Data/Interface/InputWindow-Window.png", false, 0, 0, 0);
	iws->quitx = loadimage("Data/Interface/Quit-Button.png", true, 255, 255, 255);

	if(transWindow)
		SDL_SetAlpha(iws->win, SDL_SRCALPHA | SDL_RLEACCEL, 128);

	iws->type = type;
	iws->x = ((screen->w / 2) - (iws->win->w / 2));
	iws->y = (((screen->h / 2) - (iws->win->h / 2)) + (screen->h / 7));

	iws->desc2 = RenderText_Solid(cour, "Quantity:", clrFgBlack);

	iws->inputstr = "";

	chatInputWindow = true;

	if(type == INVENTORYREMOVEITEM)
	{
		int slot = va_arg(ap, int);
		int qty = va_arg(ap, int);
		int id = va_arg(ap, int);

		_itoa_s(qty, temp, 20, 10);

		iws->btn1 = loadimage("Data/Interface/OK-Button.png", true, 255, 255, 255);
		iws->btn2 = loadimage("Data/Interface/Cancel-Button.png", true, 255, 255, 255);
		iws->btn3 = getIconSurface(id);

		if(getItemStack(id) > 1)
			iws->btn4 = RenderText_Blended(courBold, temp, clrFgBlue);

		iws->desc1 = RenderText_Solid(cour, "Delete item", clrFgBlack);
		iws->input = RenderText_Solid(cour, temp, clrFgBlack);

		iws->arg1 = slot;
		iws->arg2 = qty;
		iws->arg3 = id;

		iws->inputstr = temp;
		iws->arg2TEXT = temp;
	}

	else if(type == INVENTORYDROPITEM)
	{
		int slot = va_arg(ap, int);
		int qty = va_arg(ap, int);
		int id = va_arg(ap, int);

		_itoa_s(qty, temp, 20, 10);

		iws->btn1 = loadimage("Data/Interface/OK-Button.png", true, 255, 255, 255);
		iws->btn2 = loadimage("Data/Interface/Cancel-Button.png", true, 255, 255, 255);
		iws->btn3 = getIconSurface(id);

		if(getItemStack(id) > 1)
			iws->btn4 = RenderText_Blended(courBold, temp, clrFgBlue);

		iws->desc1 = RenderText_Solid(cour, "Drop item", clrFgBlack);
		iws->input = RenderText_Solid(cour, temp, clrFgBlack);

		iws->arg1 = slot;
		iws->arg2 = qty;
		iws->arg3 = id;

		iws->inputstr = temp;
		iws->arg2TEXT = temp;
	}

	else if(type == NPCTRADESELL)
	{
		int slot = va_arg(ap, int);
		int qty = va_arg(ap, int);
		int id = va_arg(ap, int);

		_itoa_s(qty, temp, 20, 10);

		iws->btn1 = loadimage("Data/Interface/OK-Button.png", true, 255, 255, 255);
		iws->btn2 = loadimage("Data/Interface/Cancel-Button.png", true, 255, 255, 255);
		iws->btn3 = getIconSurface(id);

		if(getItemStack(id) > 1)
			iws->btn4 = RenderText_Blended(courBold, temp, clrFgBlue);

		iws->desc1 = RenderText_Solid(cour, "Sell item", clrFgBlack);
		iws->input = RenderText_Solid(cour, temp, clrFgBlack);

		iws->arg1 = slot;
		iws->arg2 = qty;
		iws->arg3 = id;

		iws->inputstr = temp;
		iws->arg2TEXT = temp;
	}

	else if(type == PLAYERTRADEADD)
	{
		int slot = va_arg(ap, int);
		int qty = va_arg(ap, int);
		int id = va_arg(ap, int);

		_itoa_s(qty, temp, 20, 10);

		iws->btn1 = loadimage("Data/Interface/OK-Button.png", true, 255, 255, 255);
		iws->btn2 = loadimage("Data/Interface/Cancel-Button.png", true, 255, 255, 255);
		iws->btn3 = getIconSurface(id);

		if(getItemStack(id) > 1)
			iws->btn4 = RenderText_Blended(courBold, temp, clrFgBlue);

		iws->desc1 = RenderText_Solid(cour, "Add item to trade", clrFgBlack);
		iws->input = RenderText_Solid(cour, temp, clrFgBlack);

		iws->arg1 = slot;
		iws->arg2 = qty;
		iws->arg3 = id;

		iws->inputstr = temp;
		iws->arg2TEXT = temp;
	}
	
	else
		Log("UNKNOWN PARAM AT CreateInputWindow(int, int, ...) [%d]", type);

	va_end(ap);

	iw.push_back(iws);
}

bool RemoveAllMoveKeys(int key)
{
	bool active = false;

	if(keys[SDLK_UP])
	{
		keys[SDLK_UP] = false;

		keyPressed -= 1;

		if(key != SDLK_UP)
			active = true;
	}

	if(keys[SDLK_DOWN])
	{
		keys[SDLK_DOWN] = false;

		keyPressed -= 1;

		if(key != SDLK_DOWN)
			active = true;
	}

	if(keys[SDLK_RIGHT])
	{
		keys[SDLK_RIGHT] = false;

		keyPressed -= 1;

		if(key != SDLK_RIGHT)
			active = true;
	}

	if(keys[SDLK_LEFT])
	{
		keys[SDLK_LEFT] = false;

		keyPressed -= 1;

		if(key != SDLK_LEFT)
			active = true;
	}

	return active;
}

void SetKeyActive(int key)
{
	RemoveAllMoveKeys(0);

	if(!keys[key])
	{
		keys[key] = true;

		keyPressed += 1;
	}
}

void SetKeyDeactivate(int key)
{
	if(keys[key])
	{
		keys[key] = false;

		keyPressed -= 1;
	}
}

void MovePiece(float &myX, float &myY, float speed2, float pX, float pY)
{
	float speed = 1.0f;

	while(speed2 > 0.0f)
	{
		if(speed2 < 1.0f)
			speed = speed2;
		
		else
			speed = 1.0f;

		speed2 -= 1.0f;

		float cX = 0;
		float cY = 0;

		bool yOver = false;
		bool xOver = false;

		if(pX < myX)
		{
			xOver = true;

			cX = (myX - pX);
		}

		else
		{
			cX = (pX - myX);
		}

		if(pY < myY)
		{
			yOver = true;

			cY = (myY - pY);
		}

		else
		{
			cY = (pY - myY);
		}

		if(cX == 0)
		{
			if(cY != 0)
			{
				if(yOver)
					myY -= speed;
				else
					myY += speed;
			}
		}

		else if(cY == 0)
		{
			if(cX != 0)
			{
				if(xOver)
				{
					myX -= speed;
				}

				else
				{
					myX += speed;
				}
			}
		}

		else
		{
			if(cX > cY)
			{
				if(xOver)
				{
					myX -= speed;
				}

				else
				{
					myX += speed;
				}
			}

			else if(cY > cX)
			{
				if(yOver)
					myY -= speed;
				else
					myY += speed;
			}

			else
			{
				if(xOver)
				{
					myX -= speed;
				}

				else
				{
					myX += speed;
				}
			}
		}
	}
}

void MoveToPlayer(Character *fol, Character *tar)
{
	float Vel = ((float)fol->pixSec * ((float)fol->move.get_ticks(false) / 1000.0f));

	float pX = tar->x;
	float pY = tar->y;

	float cX = 0;
	float cY = 0;

	bool yOver = false;
	bool xOver = false;

	if(pX < fol->x)
	{
		xOver = true;

		cX = (fol->x - pX);
	}

	else
	{
		cX = (pX - fol->x);
	}

	if(pY < fol->y)
	{
		yOver = true;

		cY = (fol->y - pY);
	}

	else
	{
		cY = (pY - fol->y);
	}

	if(cX == 0)
	{
		if(cY != 0)
		{
			if(yOver)
				fol->y -= Vel;
			else
				fol->y += Vel;
		}
	}

	else if(cY == 0)
	{
		if(cX != 0)
		{
			if(xOver)
			{
				fol->x -= Vel;

				if(fol->wMove != LEFT || fol->cMove != LEFT)
				{
					fol->wMove = LEFT;
					fol->cMove = LEFT;
				}
			}

			else
			{
				fol->x += Vel;

				if(fol->wMove != RIGHT || fol->cMove != RIGHT)
				{
					fol->wMove = RIGHT;
					fol->cMove = RIGHT;
				}
			}
		}
	}

	else
	{
		if(cX > cY)
		{
			if(xOver)
			{
				fol->x -= Vel;

				if(fol->wMove != LEFT || fol->cMove != LEFT)
				{
					fol->wMove = LEFT;
					fol->cMove = LEFT;
				}
			}

			else
			{
				fol->x += Vel;

				if(fol->wMove != RIGHT || fol->cMove != RIGHT)
				{
					fol->wMove = RIGHT;
					fol->cMove = RIGHT;
				}
			}
		}

		else if(cY > cX)
		{
			if(yOver)
				fol->y -= Vel;
			else
				fol->y += Vel;
		}

		else
		{
			if(xOver)
			{
				fol->x -= Vel;

				if(fol->wMove != LEFT || fol->cMove != LEFT)
				{
					fol->wMove = LEFT;
					fol->cMove = LEFT;
				}
			}

			else
			{
				fol->x += Vel;

				if(fol->wMove != RIGHT || fol->cMove != RIGHT)
				{
					fol->wMove = RIGHT;
					fol->cMove = RIGHT;
				}
			}
		}
	}
}

void MoveToMoverPK(Character *c)
{
	if(NotInsideMePK(c, me))
	{
		SetKeyDeactivate(SDLK_3);

		float pX = c->x;
		float pY = c->y;

		float cX = 0;
		float cY = 0;

		bool yOver = false;
		bool xOver = false;

		if(pX < me->x)
		{
			xOver = true;

			cX = (me->x - pX);
		}

		else
			cX = (pX - me->x);

		if(pY < me->y)
		{
			yOver = true;

			cY = (me->y - pY);
		}

		else
			cY = (pY - me->y);

		if(cX == 0)
		{
			if(cY != 0)
			{
				if(yOver)
					SetKeyActive(SDLK_UP);

				else
					SetKeyActive(SDLK_DOWN);
			}
		}

		else if(cY == 0)
		{
			if(cX != 0)
			{
				if(xOver)
					SetKeyActive(SDLK_LEFT);

				else
					SetKeyActive(SDLK_RIGHT);
			}
		}

		else
		{
			if(cX > cY)
			{
				if(xOver)
					SetKeyActive(SDLK_LEFT);

				else
					SetKeyActive(SDLK_RIGHT);
			}

			else if(cY > cX)
			{
				if(yOver)
					SetKeyActive(SDLK_UP);

				else
					SetKeyActive(SDLK_DOWN);
			}

			else
			{
				if(xOver)
					SetKeyActive(SDLK_LEFT);

				else
					SetKeyActive(SDLK_RIGHT);
			}
		}
	}

	else
		SetKeyActive(SDLK_3);
}

void MoveToMover(Monster *t)
{
	if(NotInsideMe(t, me))
	{
		SetKeyDeactivate(SDLK_3);

		float pX = t->x;
		float pY = t->y;

		float cX = 0;
		float cY = 0;

		bool yOver = false;
		bool xOver = false;

		if(pX < me->x)
		{
			xOver = true;

			cX = (me->x - pX);
		}

		else
			cX = (pX - me->x);

		if(pY < me->y)
		{
			yOver = true;

			cY = (me->y - pY);
		}

		else
			cY = (pY - me->y);

		if(cX == 0)
		{
			if(cY != 0)
			{
				if(yOver)
					SetKeyActive(SDLK_UP);

				else
					SetKeyActive(SDLK_DOWN);
			}
		}

		else if(cY == 0)
		{
			if(cX != 0)
			{
				if(xOver)
					SetKeyActive(SDLK_LEFT);

				else
					SetKeyActive(SDLK_RIGHT);
			}
		}

		else
		{
			if(cX > cY)
			{
				if(xOver)
					SetKeyActive(SDLK_LEFT);

				else
					SetKeyActive(SDLK_RIGHT);
			}

			else if(cY > cX)
			{
				if(yOver)
					SetKeyActive(SDLK_UP);

				else
					SetKeyActive(SDLK_DOWN);
			}

			else
			{
				if(xOver)
					SetKeyActive(SDLK_LEFT);

				else
					SetKeyActive(SDLK_RIGHT);
			}
		}
	}

	else
		SetKeyActive(SDLK_3);
}

int GetClosestMoverPK(float x, float y, bool doMark)
{
	int clos = 0;

	float cYY = 0;
	float cXX = 0;

	for(unsigned int i = 0; i < cv.size(); i++)
	{
		if(!cv.at(i)->dead)
		{
			Character *c = cv.at(i);

			float pX = c->x;
			float pY = c->y;

			float cX = 0;
			float cY = 0;

			bool yOver = false;
			bool xOver = false;

			if(pX < y)
			{
				xOver = true;

				cX = (x - pX);
			}

			else
				cX = (pX - x);

			if(pY < y)
			{
				yOver = true;

				cY = (y - pY);
			}

			else
				cY = (pY - y);

			if(((cY + cX) < (cYY + cXX)) || clos == 0)
			{
				clos = c->id;

				cYY = cY;
				cXX = cX;
			}
		}
	}

	if(clos != 0 && doMark)
		currentMark = 2;

	return clos;
}

int GetClosestMover(float x, float y, bool doMark)
{
	int clos = 0;

	float cYY = 0;
	float cXX = 0;

	for(unsigned int i = 0; i < cm.size(); i++)
	{
		Monster *t = cm.at(i);

		if(t->fades)
			continue;

		float pX = t->x;
		float pY = t->y;

		float cX = 0;
		float cY = 0;

		bool yOver = false;
		bool xOver = false;

		if(pX < x)
		{
			xOver = true;

			cX = (x - pX);
		}

		else
			cX = (pX - x);

		if(pY < y)
		{
			yOver = true;

			cY = (y - pY);
		}

		else
			cY = (pY - y);

		if(((cY + cX) < (cYY + cXX)) || clos == 0)
		{
			clos = t->mId;

			cYY = cY;
			cXX = cX;
		}
	}

	if(clos != 0 && doMark)
		currentMark = 1;

	return clos;
}

bool BotBullet(float xTemp, Bullet *b, bool changeTarget)
{
	int clos = 1;
	bool found = false;

	if(b->type == 0)
	{
		clos = b->target;

		if(clos != 0)
		{
			float mX = 0;
			float mY = 0;

			if(clos == me->id)
			{
				mX = me->x;
				mY = me->y;

				found = true;
			}

			if(!found)
			{
				for(unsigned int i = 0; i < cv.size(); i++)
				{
					if(cv.at(i)->id == clos)
					{
						mX = cv.at(i)->x;
						mY = cv.at(i)->y;

						found = true;

						break;
					}
				}
			}

			if(!found)
				return true;

			MovePiece(b->cX, b->cY, xTemp, mX, mY);

			return false;
		}

		else
			return true;
	}

	if(b->type == 1)
	{
		clos = b->target;

		if(clos != 0)
		{
			float mX = 0;
			float mY = 0;

			for(unsigned int i = 0; i < cm.size(); i++)
			{
				if(clos == cm.at(i)->mId)
				{
					mX = cm.at(i)->x;
					mY = cm.at(i)->y;

					found = true;

					break;
				}
			}

			if(!found)
				return true;

			MovePiece(b->cX, b->cY, xTemp, mX, mY);

			return false;
		}

		else
			return true;
	}

	return true;
}

void Bot()
{
	int tempP = 0;

	if(botType == 2)
		tempP = GetClosestMoverPK(me->x, me->y, true);

	if(currentMark == 2 || currentMark == 0)
	{
		if(botTarget == 0)
			botTarget = GetClosestMoverPK(me->x, me->y, true);

		else
		{
			Character *c = NULL;

			for(unsigned int i = 0; i < cv.size(); i++)
			{
				if(botTarget == cv.at(i)->id)
				{
					if(!cv.at(i)->dead)
						c = cv.at(i);

					else
					{
						botTarget = 0;
						currentMark = 0;
					}

					break;
				}
			}

			if(c != NULL)
				MoveToMoverPK(c);

			else
				botTarget = GetClosestMoverPK(me->x, me->y, true);
		}
	}

	if(currentMark == 1 || currentMark == 0)
	{
		if(botTarget == 0)
			botTarget = GetClosestMover(me->x, me->y, true);

		else
		{
			Monster *t = NULL;

			for(unsigned int i = 0; i < cm.size(); i++)
			{
				if(botTarget == cm.at(i)->mId)
				{
					t = cm.at(i);

					break;
				}
			}

			if(t != NULL)
				MoveToMover(t);

			else
				botTarget = GetClosestMover(me->x, me->y, true);
		}
	}
}

int getEquipPosX(int slot)
{
	switch(slot)
	{
		case 1: return 101;
			break;

		case 2: return 50;
			break;

		case 3: return 101;
			break;

		case 4: return 152;
			break;
	}

	return 0;
}

int getEquipPosY(int slot)
{
	switch(slot)
	{
		case 1: return 155;
			break;

		case 2: return 203;
			break;

		case 3: return 203;
			break;

		case 4: return 203;
			break;
	}

	return 0;
}

string getItemEffectShop(int effect)
{
	switch(effect)
	{
		case 4000: return "HP Recovery: ";
			break;

		case 4001: return "Attack: ";
			break;

		case 4002: case 4003: case 4004: return "Defence: ";
			break;

		case 4005: return "HP Recovery: ";
			break;
	}

	return "";
}

string getItemEffect(int effect)
{
	if(sw->enable)
		return "Selling price: ";

	switch(effect)
	{
		case 4000: case 4005: return "HP Recovery: ";
			break;

		case 4001: return "Attack: ";
			break;

		case 4002: case 4003: case 4004: return "Defence: ";
			break;
	}

	return "";
}

string getItemExtraValue(int value, int proc2)
{
	bool min = false;
	bool proc = false;
	bool nothing = false;

	string r = "";
	string nab = IntToString(proc2);

	switch(value)
	{
		case SPEED: r = "Speed";
			break;

		case STAMINA: r = "Stamina";
			break;

		case STRENGTH: r = "Strength";
			break;

		case SPEEDMINUS: r = "Speed"; min = true;
			break;

		case HPDRAIN: r = "Drain";
			break;

		case AGILITY: r = "Agility";
			break;

		case INTT: r = "Intelligence";
			break;

		case CRIT: r = "Critical Rate"; proc = true;
			break;

		case TOTALDMG: r = "Damage"; proc = true;
			break;

		case DEFENCE: r = "Defence";
			break;

		case HPREGEN: r = "HP regeneration";
			break;

		case HPPLUS: r = "HP";
			break;

		case LIFESTEAL: r = "Lifesteal"; proc = true;
			break;

		case ATTACK: r = "Attack";
			break;

		case BLOCK: r = "Block";
			break;

		case HPPROCENTPLUS: r = "HP"; proc = true;
			break;

		case HPPROCENTMINUS: r = "HP"; proc = true; min = true;
			break;

		case HPMINUS: r = "HP"; min = true;
			break;

		case IGENOM: r = "Shoots piercing bullets"; nothing = true;
			break;
	}

	if(!nothing)
	{
		r += " ";

		if(min)
			r += "-";
		else
			r += "+";

		r += nab;

		if(value == HPDRAIN || value == HPREGEN)
			r += " HP/s";

		else
		{
			if(proc)
				r += "%";
		}
	}

	return r;
}

NPC *getNPCById(int id)
{
	for(unsigned int i = 0; i < npc.size(); i++)
	{
		if(npc.at(i)->id == id)
			return npc.at(i);
	}

	return NULL;
}

void ShopWindow::Clean()
{
	if(this->currPrice != NULL)
		SDL_FreeSurface(this->currPrice);

	this->currPrice = NULL;

	if(this->currQty != NULL)
		SDL_FreeSurface(this->currQty);

	this->currQty = NULL;

	if(this->totalPrice != NULL)
		SDL_FreeSurface(this->totalPrice);

	this->totalPrice = NULL;

	if(this->shopName != NULL)
		SDL_FreeSurface(this->shopName);

	this->shopName = NULL;

	for(int i = 0; i < 20; i++)
	{
		if(this->cart[i].picStack != NULL)
			SDL_FreeSurface(this->cart[i].picStack);

		this->si[i] = ShopItem();
		this->cart[i] = ShopItem();
	}

	this->npcId = 0;
	this->cMark = 0;

	int minusMode = 0;

	for(unsigned int i = 0; i < iw.size(); i++)
	{
		if(minusMode == 1)
			i--;

		minusMode = 0;

		if(iw.at(i)->type == NPCTRADESELL)
		{
			iw.at(i)->Clean(i);

			delete iw.at(i);
			iw.erase((iw.begin() + i));

			if(iw.size() == 0)
				chatInputWindow = false;

			minusMode = 1;
		}
	}

	this->SetupPos();
}

void PlayerTrade::Clean()
{
	if(this->myPenya != NULL)
		SDL_FreeSurface(this->myPenya);

	this->myPenya = NULL;

	if(this->status1 != NULL)
		SDL_FreeSurface(this->status1);

	this->status1 = NULL;

	if(this->status2 != NULL)
		SDL_FreeSurface(this->status2);

	this->status2 = NULL;

	if(this->otherPenya != NULL)
		SDL_FreeSurface(this->otherPenya);

	this->otherPenya = NULL;

	if(this->tradeName1 != NULL)
		SDL_FreeSurface(this->tradeName1);

	this->tradeName1 = NULL;

	if(this->tradeName2 != NULL)
		SDL_FreeSurface(this->tradeName2);

	this->tradeName2 = NULL;

	for(int i = 0; i < 20; i++)
	{
		if(this->si[i].picStack != NULL)
			SDL_FreeSurface(this->si[i].picStack);

		if(this->si2[i].picStack != NULL)
			SDL_FreeSurface(this->si2[i].picStack);

		this->si[i] = ShopItem();
		this->si2[i] = ShopItem();
	}

	this->mySide = 0;

	this->penyaMe = 0;
	this->penyaOther = 0;

	this->side1 = 0;
	this->side2 = 0;

	this->SetupPos();
}

void PlayerTrade::SetupPos()
{
	int currX = (this->x + 41);
	int currY = (this->y + 122);

	int sideX = (this->x + 243);
	int sideY = (this->y + 122);

	int restart = 1;

	for(int i = 0; i < 20; i++)
	{
		this->si[i].x = currX;
		this->si[i].y = currY;

		this->si2[i].x = sideX;
		this->si2[i].y = sideY;

		currX += 33;
		sideX += 33;
		
		if(restart == 4)
		{
			currX = (this->x + 41);
			currY += 33;

			sideX = (this->x + 243);
			sideY += 33;

			restart = 0;
		}

		restart++;
	}
}

void ShopWindow::SetupPos()
{
	int currX = (this->x + 30);
	int currY = (this->y + 54);

	int cartX = (this->x + 30);
	int cartY = (this->y + 355);

	for(int i = 0; i < 20; i++)
	{
		this->si[i].x = currX;
		this->si[i].y = currY;

		this->cart[i].x = cartX;
		this->cart[i].y = cartY;

		currX += 33;
		cartX += 33;
		
		if(currX >= (this->x + 280))
		{
			currX = (this->x + 30);
			currY += 33;
		}

		if(cartX >= (this->x + 280))
		{
			cartX = (this->x + 30);
			cartY += 33;
		}
	}
}

void ShopWindow::UpdateTotalPrice()
{
	int totprice = 0;

	for(int i = 0; i < 20; i++)
	{
		if(this->cart[i].itemId != 0)
			totprice += this->cart[i].itemPrice;
	}

	if(this->totalPrice != NULL)
		SDL_FreeSurface(this->totalPrice);

	char temp[10] = "";

	_itoa_s(totprice, temp, 10, 10);

	this->totalPrice = RenderText_Solid(cour, temp, clrFgBlack);
}

int GetAttackDelay()
{
	long delay = 406;

	int agi = (me->agility * 2);
	agi += bonusAttackSpeed;

	if((delay - agi) <= 10)
		return 10;

	else
		return (delay - agi);
}

bool PlayerKillArea(Character *c)
{
	bool retValue = false;

	for(unsigned int i = 0; i < map->pkArea.size(); i++)
	{
		if(OverButton(map->pkArea.at(i).xMin, map->pkArea.at(i).yMin, (map->pkArea.at(i).xMax - map->pkArea.at(i).xMin), (map->pkArea.at(i).yMax - map->pkArea.at(i).yMin), (int)c->x, (int)c->y))
		{
			retValue = true;

			break;
		}
	}

	return retValue;
}

void pingPacket()
{
	LARGE_INTEGER currentTimer;
	LARGE_INTEGER timePerSecond;
	LARGE_INTEGER showTime;

	QueryPerformanceFrequency(&timePerSecond);
	QueryPerformanceCounter(&currentTimer);

	showTime.QuadPart = (currentTimer.QuadPart - pingTimes.QuadPart);
	showTime.QuadPart /= (timePerSecond.QuadPart / 1000);

	long times = (long)showTime.QuadPart;

	if(pingSurface != NULL)
		SDL_FreeSurface(pingSurface);

	string pingString = "Latency: ";
	char buf[10] = "";

	_itoa_s(times, buf, 10, 10);

	pingString += buf;
	pingString += " ms";

	pingSurface = RenderText_Solid(cour, pingString.c_str(), clrFg);

	sentPingPacket = false;

	lastPing = times;
}

Character *getPlayerById(int id)
{
	if(me->id == id)
		return me;

	for(unsigned int i = 0; i < cv.size(); i++)
	{
		if(cv.at(i)->id == id)
			return cv.at(i);
	}

	return NULL;
}

Monster *getMonsterById(int id)
{
	for(unsigned int i = 0; i < cm.size(); i++)
	{
		if(cm.at(i)->mId == id)
			return cm.at(i);
	}

	return NULL;
}

bool thisMarkedMonster(Monster *m)
{
	if(mark->enable && mark->marktype == 2 && mark->npcmark == m->mId)
		return true;

	else
		return false;
}

bool thisMarkedPlayer(Character *p)
{
	if(mark->enable && mark->marktype == 1 && mark->npcmark == p->id)
		return true;

	else
		return false;
}

int msize(int pav)
{
	if(map == NULL)
		return 0;

	if(pav == MAPX)
		return map->xSize;

	if(pav == MAPY)
		return map->ySize;

	return 0;
}

void MeMove(int DIR, float Vel)
{
	if(DIR == UP)
	{
		if((cameraY - Vel) < 26 && (int)cameraY != 26 && (int)cameraY > 26)
			Vel = (cameraY - 26.0f);

		if((cameraY - Vel) >= 26)
		{
			if(cameraY <= ((screen->h / 3) - me->my->h) && currBackY <= (0 - Vel))
			{
				currBackY += Vel;
				me->y -= Vel;

				camShowX = (int)(msize(MAPX) - (currBackX * (-1.0f)));
				camShowX = (msize(MAPX) - camShowX);
				camShowToX = (camShowX + screen->w);

				camShowY = (int)(msize(MAPY) - (currBackY * (-1.0f)));
				camShowY = (msize(MAPY) - camShowY);
				camShowToY = (camShowY + screen->h);

				if(mark->enable)
				{
					switch(mark->marktype)
					{
						case 1: UpdateMarkPlayer(getPlayerById(mark->npcmark));
							break;

						case 2: UpdateMarkMonster(getMonsterById(mark->npcmark));
							break;

						case 3: UpdateMark(getNPCById(mark->npcmark));
							break;
					}
				}
			}

			else
			{
				cameraY -= Vel;
				me->y -= Vel;
			}
		}
	}

	else if(DIR == DOWN)
	{
		if((me->y + Vel) > (msize(MAPY) - me->my->h) && (int)me->y != (msize(MAPY) - me->my->h) && (int)me->y < (msize(MAPY) - me->my->h))
			Vel = ((msize(MAPY) - me->my->h) - me->y);

		if((me->y + Vel) <= (msize(MAPY) - me->my->h))
		{
			if((cameraY >= (screen->h - (screen->h / 3))) && (msize(MAPY) + currBackY) > (screen->h + Vel))
			{
				currBackY -= Vel;
				me->y += Vel;

				camShowX = (int)(msize(MAPX) - (currBackX * (-1.0f)));
				camShowX = (msize(MAPX) - camShowX);
				camShowToX = (camShowX + screen->w);

				camShowY = (int)(msize(MAPY) - (currBackY * (-1.0f)));
				camShowY = (msize(MAPY) - camShowY);
				camShowToY = (camShowY + screen->h);

				if(mark->enable)
				{
					switch(mark->marktype)
					{
						case 1: UpdateMarkPlayer(getPlayerById(mark->npcmark));
							break;

						case 2: UpdateMarkMonster(getMonsterById(mark->npcmark));
							break;

						case 3: UpdateMark(getNPCById(mark->npcmark));
							break;
					}
				}
			}

			else
			{
				cameraY += Vel;
				me->y += Vel;
			}
		}
	}

	else if(DIR == LEFT)
	{
		if(me->wMove != LEFT || me->cMove != LEFT)
		{
			me->wMove = LEFT;
			me->cMove = LEFT;
		}

		if((cameraX - Vel) < 0 && (int)cameraX != 0 && (int)cameraX > 0)
			Vel = cameraX;

		if((cameraX - Vel) >= 0)
		{
			if(cameraX <= ((screen->w / 3) - me->my->w) && currBackX <= (0 - Vel))
			{
				currBackX += Vel;
				me->x -= Vel;

				camShowX = (int)(msize(MAPX) - (currBackX * (-1.0f)));
				camShowX = (msize(MAPX) - camShowX);
				camShowToX = (camShowX + screen->w);

				camShowY = (int)(msize(MAPY) - (currBackY * (-1.0f)));
				camShowY = (msize(MAPY) - camShowY);
				camShowToY = (camShowY + screen->h);

				if(mark->enable)
				{
					switch(mark->marktype)
					{
						case 1: UpdateMarkPlayer(getPlayerById(mark->npcmark));
							break;

						case 2: UpdateMarkMonster(getMonsterById(mark->npcmark));
							break;

						case 3: UpdateMark(getNPCById(mark->npcmark));
							break;
					}
				}
			}

			else
			{
				cameraX -= Vel;
				me->x -= Vel;
			}
		}
	}

	else if(DIR == RIGHT)
	{
		if(me->wMove != RIGHT || me->cMove != RIGHT)
		{
			me->wMove = RIGHT;
			me->cMove = RIGHT;
		}

		int moveMax = screen->w;

		if(screen->w > msize(MAPX))
			moveMax = msize(MAPX);

		if((cameraX + Vel) > (moveMax - me->my->w) && (int)cameraX != (moveMax - me->my->w) && (int)cameraX < (moveMax - me->my->w))
			Vel = ((moveMax - me->my->w) - cameraX);

		if((cameraX + Vel) <= (moveMax - me->my->w))
		{
			if((cameraX >= (screen->w - (screen->w / 3))) && (msize(MAPX) + currBackX) > (screen->w + Vel))
			{
				currBackX -= Vel;
				me->x += Vel;

				camShowX = (int)(msize(MAPX) - (currBackX * (-1.0f)));
				camShowX = (msize(MAPX) - camShowX);
				camShowToX = (camShowX + screen->w);

				camShowY = (int)(msize(MAPY) - (currBackY * (-1.0f)));
				camShowY = (msize(MAPY) - camShowY);
				camShowToY = (camShowY + screen->h);

				if(mark->enable)
				{
					switch(mark->marktype)
					{
						case 1: UpdateMarkPlayer(getPlayerById(mark->npcmark));
							break;

						case 2: UpdateMarkMonster(getMonsterById(mark->npcmark));
							break;

						case 3: UpdateMark(getNPCById(mark->npcmark));
							break;
					}
				}
			}

			else
			{
				cameraX += Vel;
				me->x += Vel;
			}
		}
	}
}

void AddChatText(SDL_Event &event, std::string &tempSTR, int ts)
{
	if(ts == 2)
	{
		if((event.key.keysym.unicode >= (Uint16)'0') && (event.key.keysym.unicode <= (Uint16)'9'))
			tempSTR += (char)event.key.keysym.unicode;
	}

	else
	{
		if(event.key.keysym.unicode == (Uint16)' ')
		{
			if(!chatWindow)
				tempSTR += (char)event.key.keysym.unicode;
		}

		else if((event.key.keysym.unicode >= (Uint16)'0') && (event.key.keysym.unicode <= (Uint16)'9'))
			tempSTR += (char)event.key.keysym.unicode;

		else if((event.key.keysym.unicode >= (Uint16)'A') && (event.key.keysym.unicode <= (Uint16)'Z'))
			tempSTR += (char)event.key.keysym.unicode;

		else if((event.key.keysym.unicode >= (Uint16)'a') && (event.key.keysym.unicode <= (Uint16)'z'))
			tempSTR += (char)event.key.keysym.unicode;
		else if((event.key.keysym.unicode >= (Uint16)'!') && (event.key.keysym.unicode <= (Uint16)'/'))
			tempSTR += (char)event.key.keysym.unicode;

		else if((event.key.keysym.unicode >= (Uint16)':') && (event.key.keysym.unicode <= (Uint16)'@'))
			tempSTR += (char)event.key.keysym.unicode;

		else if((event.key.keysym.unicode >= (Uint16)'[') && (event.key.keysym.unicode <= (Uint16)'`'))
			tempSTR += (char)event.key.keysym.unicode;

		else if((event.key.keysym.unicode >= (Uint16)'{') && (event.key.keysym.unicode <= (Uint16)'~'))
			tempSTR += (char)event.key.keysym.unicode;

		else if((event.key.keysym.unicode >= 161) && (event.key.keysym.unicode <= 255))
			tempSTR += (char)event.key.keysym.unicode;
	}
}

unsigned int getTime(bool fast)
{
	if(fast)
		return SDL_GetTicks();

	else
	{
		LARGE_INTEGER currentTimer;
		LARGE_INTEGER timePerSecond;

		QueryPerformanceFrequency(&timePerSecond);
		QueryPerformanceCounter(&currentTimer);

		currentTimer.QuadPart /= (timePerSecond.QuadPart / 1000);

		return (unsigned int)currentTimer.QuadPart;
	}
}

SDL_Surface *RenderText_Solid(TTF_Font *font, const char *string, SDL_Color clr)
{
	if(string == NULL)
		return NULL;

	if(strlen(string) <= 0)
		return NULL;

	SDL_Surface *solid = TTF_RenderText_Solid(font, string, clr);
	SDL_Surface *news = SDL_DisplayFormat(solid);
	
	SDL_FreeSurface(solid);

	return news;
}

SDL_Surface *RenderText_Blended(TTF_Font *font, const char *string, SDL_Color clr)
{
	if(string == NULL)
		return NULL;

	if(strlen(string) <= 0)
		return NULL;

	SDL_Surface *solid = TTF_RenderText_Blended(font, string, clr);

	return solid;
}

void CreateTheSurface(NameSurface *sur, TTF_Font *font, SDL_Color clr, char *string, int type)
{
	SDL_Surface *dones = NULL;
	SDL_Surface *newString = NULL;

	bool convert = false;

	if(type == 1 || type == 2)
		convert = true;
	
	if(!convert)
	{
		Log("Invalid param at CreateNameSurface().");
		return;
	}

	if(type == 1)
		newString = TTF_RenderText_Solid(font, string, clr);

	else if(type == 2)
		newString = TTF_RenderText_Blended(font, string, clr);

	dones = SDL_DisplayFormat(newString);
	SDL_FreeSurface(newString);

	sur->surs.push_back(dones);
}

void CreateNameSurface(NameSurface *sur, TTF_Font *font, SDL_Color clr, char *string, int type)
{
	if(string == NULL)
		return;

	CreateTheSurface(sur, font, clr, string, type);
}

void NameSurface::draw(int x, int y)
{
	for(unsigned int i = 0; i < this->surs.size(); i++)
		UpdateImage(this->surs.at(i), x, y);
}

int CompareColors(SDL_Color clr1, SDL_Color clr2)
{
	if(clr1.r == clr2.r && clr1.g == clr2.g && clr1.b == clr2.b)
		return 0;

	return 1;
}

void StopMusic()
{
	music->stopMusic();
}

void SFXManager::playSound(int PARAM, int loop)
{
	if(!noSound)
	{
		bool found = false;
		SFXSound thes;

		for(unsigned int i = 0; i < this->sounds.size(); i++)
		{
			if(this->sounds.at(i).p = PARAM)
			{
				thes = this->sounds.at(i);
				found = true;

				break;
			}
		}

		if(found)
			thes.channel = Mix_PlayChannel(-1, thes.s, loop);
	}
}

void SFXManager::stopSound(int PARAM)
{
	for(unsigned int i = 0; i < sounds.size(); i++)
	{
		if(sounds.at(i).p == PARAM)
		{
			Mix_HaltChannel(sounds.at(i).channel);
			sounds.at(i).channel = 0;

			return;
		}
	}
}

void SFXManager::createSound(char *fname, int type, int param)
{
	Mix_Chunk *the = NULL;

	if(type == 1)
		the = Mix_LoadWAV(fname);

	if(the != NULL)
	{
		SFXSound ss;

		ss.p = param;
		ss.s = the;
		ss.channel = 0;

		this->sounds.push_back(ss);
	}

	else
		Log("Unable to load music file: %s.", fname);
}

void SFXManager::deleteSound(int PARAM)
{
	for(unsigned int i = 0; i < this->sounds.size(); i++)
	{
		if(this->sounds.at(i).p == PARAM)
		{
			Mix_FreeChunk(this->sounds.at(i).s);
			this->sounds.erase(this->sounds.begin() + i);
			
			return;
		}
	}
}

Mix_Chunk *SFXManager::getSound(int PARAM)
{
	for(unsigned int i = 0; i < this->sounds.size(); i++)
	{
		if(this->sounds.at(i).p == PARAM)
			return this->sounds.at(i).s;
	}

	return NULL;
}

void StartDragWindow(int t, int x, int y, int *sx, int *sy, int i, bool *enable, int *winX, int *winY)
{
	dragged.type = t;
	dragged.id = i;
	dragged.x = x;
	dragged.y = y;
	dragged.pic = NULL;
	dragged.slot = (x - *sx);
	dragged.qty = (y - *sy);
	dragged.winX = sx;
	dragged.winY = sy;
	dragged.winEnable = enable;
	dragged.winSizeX = winX;
	dragged.winSizeY = winY;

	drag = true;
}

void MoveKoblaWindow(int x, int y)
{
	if(dragged.winEnable != NULL)
	{
		if(!*dragged.winEnable)
			drag = false;
	}

	int newX = (x - dragged.slot);
	int newY = (y - dragged.qty);

	if(newX >= 0 && (newX + *dragged.winSizeX) <= resX)
		*dragged.winX = newX;

	else
	{
		if(newX < 0)
			*dragged.winX = 0;

		else if((newX + *dragged.winSizeX) > resX)
			*dragged.winX = (resX - *dragged.winSizeX);
	}

	if(newY >= 0 && (newY + *dragged.winSizeY) <= resY - task->bar->h + 1)
		*dragged.winY = newY;

	else
	{
		if(newY < 0)
			*dragged.winY = 0;

		else if((newY + *dragged.winSizeY) > (resY - task->bar->h + 1))
			*dragged.winY = ((resY - task->bar->h + 1) - *dragged.winSizeY);
	}

	switch(dragged.type)
	{
		case DRAGINVENTORYWINDOW: SetupInventory();
			break;

		case DRAGTRADEWINDOW: ptrade->SetupPos();
			break;

		case DRAGNPCSHOPWINDOW: sw->SetupPos();
			break;

		case DRAGBUFFS: UpdateBuffPosition();
			break;

		case DRAGFRIENDWINDOW: fw->startY = fw->y + 65;
			break;

		case DRAGWINDOW:
			{
				if(dragged.id == 1)
					chatFunc->SetLast();
			break;
			}

		case DRAGPMSWINDOW:
			{
				for(unsigned int i = 0; i < pms->pm.size(); i++)
				{
					if(pms->pm.at(i).toname == dragged.name)
					{
						pms->pm.at(i).SetLast();
						break;
					}
				}
			break;
			}
	}
}

void SetupInventory()
{
	for(int i = 1; i <= 25; i++)
	{
		int rad = 1;

		int cX = (inv->x + 17);
		int cY = (inv->y + 284);

		for(int l = 0; l < 25; l++)
		{
			if(inv->items[l] != NULL)
			{
				if((i - 1) == l)
				{
					inv->items[l]->x = cX;
					inv->items[l]->y = cY;

					break;
				}
			}

			rad++;

			if(rad == 6)
			{
				rad = 1;

				cY += 36;
				cX = (inv->x + 17);
			}

			else
				cX += 43;
		}
	}
}

void KMessageBox(char *titel, UINT params, char *text, ...)
{
	va_list args;
	char buf[1024];

	va_start(args, text);
	vsnprintf_s(buf, 1024, 1024, text, args);
	va_end(args);

	MessageBox(NULL, text, titel, params);
	Log(buf);
}

void LMessage(int effect, char *text, ...)
{
	va_list args;
	char buf[1024];

	va_start(args, text);
	vsnprintf_s(buf, 1024, 1024, text, args);
	va_end(args);

	LoginMessage *lmsg = new LoginMessage();

	lmsg->win = loadimage("Data/Interface/Login-Message.png", false, 0, 0, 0);
	lmsg->names = RenderText_Solid(cour, buf, clrFgBlack);
	lmsg->effect = effect;
	lmsg->x = ((screen->w / 2) - (lmsg->win->w / 2));
	lmsg->y = ((screen->h / 2) - (lmsg->win->h / 2));

	lmsgs.push_back(lmsg);
}

void CreateLoginInput(int TYPE, int i)
{
	LoginInput *lin = new LoginInput();

	switch(TYPE)
	{
		case CREATECHARACTER: lin->win = loadimage("Data/Interface/Login-CreateCharacter.png", false, 0, 0, 0);
			break;

		case DELETECHARACTER: lin->win = loadimage("Data/Interface/Login-DeleteCharacter.png", false, 0, 0, 0);
			break;
	}

	lin->t = TYPE;
	lin->i = i;

	linputs.push_back(lin);
}

void LoginWriter::NewSurface(int etc)
{
	if(etc == 1)
	{
		if(this->ar1 != NULL)
			SDL_FreeSurface(this->ar1);

		this->ar1 = NULL;

		if(isMasked == DELETECHARACTER)
		{
			string newstring = "";

			for(unsigned int i = 0; i < this->ar1s.length(); i++)
				newstring += "*";

			this->ar1 = RenderText_Solid(cour, newstring.c_str(), clrFgBlack);
		}

		else
			this->ar1 = RenderText_Solid(cour, this->ar1s.c_str(), clrFgBlack);
	}
	
	else if(etc == 2)
	{
		if(this->ar2 != NULL)
			SDL_FreeSurface(this->ar2);

		this->ar2 = NULL;

		if(logplace == LOGINSCREEN)
		{
			string newstring = "";

			for(unsigned int i = 0; i < this->ar2s.length(); i++)
				newstring += "*";

			this->ar2 = RenderText_Solid(cour, newstring.c_str(), clrFgBlack);
		}

		else
			this->ar2 = RenderText_Solid(cour, this->ar2s.c_str(), clrFgBlack);
	}
}

void LoginWriter::ShowStrings()
{
	if(this->enable1)
	{
		if(this->ar1 != NULL)
			UpdateImage(this->ar1, this->wr1x, this->wr1y);
	}

	if(this->enable2)
	{
		if(this->ar2 != NULL)
			UpdateImage(this->ar2, this->wr2x, this->wr2y);
	}
}

int getInventoryStack(int id)
{
	int stack = 0;

	for(int i = 0; i < 25; i++)
	{
		if(inv->items[i] != NULL)
		{
			if(inv->items[i]->id == id)
				stack += inv->items[i]->stack;
		}
	}

	return stack;
}

SDL_Surface *getInventoryStackPic(int id)
{
	char epic[20];
	int stack = 0;

	for(int i = 0; i < 25; i++)
	{
		if(inv->items[i] != NULL)
		{
			if(inv->items[i]->id == id)
				stack += inv->items[i]->stack;
		}
	}

	_itoa_s(stack, epic, 20, 10);

	return RenderText_Blended(courBold, epic, clrFgBlue);
}

void UpdateTaskbar(int type, int id, int slot)
{
	if(type == REMOVEITEM)
	{
		for(int i = 0; i < 9; i++)
		{
			if(task->pic[i]->enable)
			{
				if(task->pic[i]->slot == slot && task->pic[i]->type == INVENTORYITEM && !task->pic[i]->eqinv)
				{
					if(getItemStack(task->pic[i]->itemid) > 1)
					{
						if(getInventoryStack(task->pic[i]->itemid) <= 0)
						{
							SDL_FreeSurface(task->pic[i]->stack);
							
							task->pic[i]->enable = false;
						}
					}

					else
						task->pic[i]->enable = false;
				}
			}
		}
	}

	for(int i = 0; i < 9; i++)
	{
		if(task->pic[i]->enable)
		{
			if(task->pic[i]->type == INVENTORYITEM && !task->pic[i]->eqinv)
			{
				if(getItemStack(task->pic[i]->itemid) > 1)
				{
					if(getInventoryStack(task->pic[i]->itemid) <= 0)
					{
						SDL_FreeSurface(task->pic[i]->stack);

						task->pic[i]->enable = false;
					}
				}
			}
		}
	}

	for(int i = 0; i < 9; i++)
	{
		if(task->pic[i]->enable)
		{
			if(task->pic[i]->type == INVENTORYITEM && !task->pic[i]->eqinv)
			{
				if(getItemStack(task->pic[i]->itemid) > 1)
				{
					SDL_FreeSurface(task->pic[i]->stack);

					task->pic[i]->stack = getInventoryStackPic(task->pic[i]->itemid);
				}
			}
		}
	}
}

void UpdateBuffPosition()
{
	if(icons.size() > 0)
	{
		for(unsigned int i = 0; i < icons.size(); i++)
		{
			Icon *ic = icons.at(i);

			ic->x = (icons.at(0)->x + ((i * 42) + 1));
			ic->y = icons.at(0)->y;

			ic->timeLeftX = (ic->x + 5);
			ic->timeLeftY = (ic->y + 28);

			ic->name1X = (ic->x + 5);
			ic->name1Y = (ic->y + 3);
			
			ic->name2X = (ic->x + 5);
			ic->name2Y = (ic->name1Y + 10);
		}
	}

	else
	{
		if(drag && dragged.type == DRAGBUFFS)
			drag = false;
	}
}

void LoadNewMap(int maps)
{
	if(map == NULL)
	{
		Log("Map %d does not exist.", maps);
		return;
	}

	CleanTiles();
	map->tiles.clear();

	string ops = "Data/World/World-";

	ops += IntToString(maps);
	ops += ".txt";

	ifstream opens(ops.c_str());
	if(!opens)
	{
		Log("Could not open %s.", ops.c_str()); opens.close();
		return;
	}

	int numti = 0;
	int xsizetile = 0;
	int ysizetile = 0;
	int xcsize = 0;
	int ycsize = 0;
	int ytemp = 0;

	opens >> numti;
	opens >> xsizetile;
	opens >> ysizetile;

	char temp[200] = "";

	for(int i = 0; i < ((map->xSize / xsizetile) * (map->ySize / ysizetile)); i++)
	{
		Tile t = Tile();

		t.posx = xcsize;
		t.posy = ycsize;

		xcsize += xsizetile;

		if(xcsize >= map->xSize)
		{
			xcsize = 0;
			ycsize += ysizetile;
		}

		map->tiles.push_back(t);

		if(map->ySize <= ycsize)
			break;
	}

	for(int i = 0; i < numti; i++)
	{
		int tid = 0;
		int fx = 0;
		int tx = 0;
		int fy = 0;
		int ty = 0;
		int walk = 0;

		opens >> tid;
		opens >> fx;
		opens >> tx;
		opens >> fy;
		opens >> ty;
		opens >> walk;

		int tfx = fx;
		int tfy = fy;

		bool walkarea = true;
		bool stop = false;

		if(walk == 1)
			walkarea = false;

		while(true)
		{
			for(unsigned int i = 0; i < map->tiles.size(); i++)
			{
				if(map->tiles.at(i).posx == tfx && map->tiles.at(i).posy == tfy)
				{
					map->tiles.at(i).id = tid;
					map->tiles.at(i).su = getTile(tid)->su;
					map->tiles.at(i).walk = walkarea;
					map->tiles.at(i).got = true;

					tfx += xsizetile;

					if(tfx >= tx)
					{
						tfx = fx;
						tfy += ysizetile;

						if(tfy >= ty)
						{
							stop = true;
							break;
						}
					}
				}
			}

			if(stop)
				break;
		}
	}

	opens.close();

	for(unsigned int i = 0; i < map->tiles.size(); i++)
	{
		if(!map->tiles.at(i).got)
		{
			map->tiles.at(i).su = getSurface(99);
			map->tiles.at(i).got = true;
		}
	}

	map->camShowX = 0;
	map->camShowToX = 0;
	map->camShowY = 0;
	map->camShowToY = 0;
}

void CopySkillInfo(InfoSkill *cs, Skill *ts)
{
	ts->id = cs->id;
	ts->pic = cs->pic;
	ts->level = 1;
	ts->x = cs->invX;
	ts->y = cs->invY;
	ts->takeamount = cs->takeamount;
	ts->take = cs->take;
	ts->nameSkill = cs->nameSkill;
}

void SetSkillLevel(int lvl, int id)
{
	for(unsigned int i = 0; i < skill.size(); i++)
	{
		if(skill.at(i)->id == id)
		{
			skill.at(i)->level = lvl;
			skill.at(i)->text = RenderText_Solid(cour, IntToString(lvl).c_str(), clrFgBlack);

			break;
		}
	}

	/*
	for(int i = 0; i < skillLoaded; i++)
	{
		if(skills[i].id == id)
		{
			skills[i].level = lvl;
			skills[i].text = RenderText_Solid(cour, IntToString(lvl).c_str(), clrFgBlack);

			break;
		}
	}
	*/
}

int InitFD()
{
	FD_ZERO(&f_readset);
	FD_ZERO(&f_writeset);
	FD_ZERO(&f_exset);

	if(!dc)
	{
		FD_SET(sock, &f_readset);
		FD_SET(sock, &f_exset);

		if(toserv.size() > 0)
			FD_SET(sock, &f_writeset);
	}

	return 0;
}

int HandlePackets(unsigned char *buf, int by, bool &move)
{
	unsigned char *bufr;
	int len2 = by;
	bool created = false;

	if(cp.getsize() > 0)
	{
		Fragment fr = Fragment();
		fr.buf = new unsigned char[by];
		memcpy(fr.buf, buf, by);
		fr.len = by;

		cp.frag.push_back(fr);

		cp.compilefrag();

		bufr = new unsigned char[cp.totallen];
		memcpy(bufr, cp.fullbuf, cp.totallen);
		len2 = cp.totallen;

		created = true;
	}

	else
	{
		bufr = buf;

		Fragment fr = Fragment();
		fr.buf = new unsigned char[by];
		memcpy(fr.buf, buf, by);
		fr.len = by;

		cp.frag.push_back(fr);
	}

	if(len2 < 3)
	{
		cp.waiting = true;
		cp.iwant = 13;

		if(created)
			delete[] bufr;

		return 1;
	}

	else
	{
		int lenofthis = bufr[0];
		
		if(1 + lenofthis > len2)
		{
			cp.waiting = true;
			cp.iwant = 13;

			if(created)
				delete[] bufr;

			return 1;
		}

		else
		{
			int pack = atoi(getString((char*)bufr, 1, lenofthis).c_str());
			int fullpacklen = (pack + lenofthis + 1);

			if(len2 < fullpacklen)
			{
				cp.waiting = true;
				cp.iwant = fullpacklen - len2;

				if(created)
					delete[] bufr;

				return 1;
			}

			if(len2 > fullpacklen)
			{
				int currPos = 0;

				while(len2 > currPos)
				{
					int lenofthis2 = bufr[currPos];
					int pack2 = atoi(getString((char*)bufr, (currPos + 1), lenofthis2).c_str());
					int fullpacklen2 = (pack2 + lenofthis2 + 1);

					if((currPos + fullpacklen2) > len2)
					{
						cp.eraseme();
						cp.waiting = true;

						Fragment fr2 = Fragment();
						fr2.buf = new unsigned char[len2 - currPos];
						memcpy(fr2.buf, bufr + currPos, len2 - currPos);
						fr2.len = len2 - currPos;

						cp.frag.push_back(fr2);

						cp.iwant = fullpacklen2 - (len2 - currPos);

						if(created)
							delete[] bufr;

						return 1;
					}

					if(fullpacklen2 > 2)
					{
						ProcessPacket(bufr + lenofthis2 + 1 + currPos, pack2, move);
					}

					else
						break;

					currPos += fullpacklen2;
				}
			}

			else
			{
				if(fullpacklen == len2)
					ProcessPacket(bufr + lenofthis + 1, pack, move);
			}
		}
	}

	if(created)
		delete[] bufr;

	return 0;
}

bool AttackedTarget(Bullet *b, int id, int type, bool doit)
{
	for(unsigned int i = 0; i < b->hitted.size(); i++)
	{
		if(b->hitted.at(i).id == id && b->hitted.at(i).type == type)
			return true;
	}

	if(doit)
	{
		BulletHit bh = BulletHit();

		bh.id = id;
		bh.type = type;

		b->hitted.push_back(bh);
	}

	return false;
}

void SetTrans(bool what)
{
	if(what)
	{
		SDL_SetAlpha(partyw.at(0)->window, SDL_SRCALPHA | SDL_RLEACCEL, 128);
			
		for(unsigned int p = 0; p < win.size(); p++)
			SDL_SetAlpha(win.at(p)->w, SDL_SRCALPHA | SDL_RLEACCEL, 128);

		SDL_SetAlpha(addstatwindow.at(0)->window, SDL_SRCALPHA | SDL_RLEACCEL, 128);

		SDL_SetAlpha(task->bar, SDL_SRCALPHA | SDL_RLEACCEL, 128);
		SDL_SetAlpha(task->menu, SDL_SRCALPHA | SDL_RLEACCEL, 128);

		SDL_SetAlpha(inv->inv, SDL_SRCALPHA | SDL_RLEACCEL, 128);

		SDL_SetAlpha(sk->win, SDL_SRCALPHA | SDL_RLEACCEL, 128);

		SDL_SetAlpha(option->win, SDL_SRCALPHA | SDL_RLEACCEL, 128);

		SDL_SetAlpha(sw->win, SDL_SRCALPHA | SDL_RLEACCEL, 128);

		SDL_SetAlpha(ptrade->win, SDL_SRCALPHA | SDL_RLEACCEL, 128);

		SDL_SetAlpha(tp->win, SDL_SRCALPHA | SDL_RLEACCEL, 128);

		SDL_SetAlpha(fw->win, SDL_SRCALPHA | SDL_RLEACCEL, 128);

		SDL_SetAlpha(pms->win, SDL_SRCALPHA | SDL_RLEACCEL, 128);

		SDL_SetAlpha(skillShow, SDL_SRCALPHA | SDL_RLEACCEL, 128);

		for(unsigned int lp2 = 0; lp2 < npc.size(); lp2++)
		{
			if(npc.at(lp2)->dialogWindow != NULL)
				SDL_SetAlpha(npc.at(lp2)->dialogWindow, SDL_SRCALPHA | SDL_RLEACCEL, 128);
		}

		for(unsigned int i = 0; i < iw.size(); i++)
		{
			if(iw.at(i)->win != NULL)
				SDL_SetAlpha(iw.at(i)->win, SDL_SRCALPHA | SDL_RLEACCEL, 128);
		}

		for(unsigned int i = 0; i < pi.size(); i++)
		{
			if(pi.at(i)->win != NULL)
				SDL_SetAlpha(pi.at(i)->win, SDL_SRCALPHA | SDL_RLEACCEL, 128);
		}

		for(unsigned int i = 0; i < msgs.size(); i++)
		{
			if(msgs.at(i)->win != NULL)
				SDL_SetAlpha(msgs.at(i)->win, SDL_SRCALPHA | SDL_RLEACCEL, 128);
		}
	}

	else
	{
		SDL_SetAlpha(partyw.at(0)->window, SDL_RLEACCEL, 255);
							
		for(unsigned int p = 0; p < win.size(); p++)
			SDL_SetAlpha(win.at(p)->w, SDL_RLEACCEL, 255);

		SDL_SetAlpha(addstatwindow.at(0)->window, SDL_RLEACCEL, 255);

		SDL_SetAlpha(task->bar, SDL_RLEACCEL, 255);
		SDL_SetAlpha(task->menu, SDL_RLEACCEL, 255);

		SDL_SetAlpha(inv->inv, SDL_RLEACCEL, 255);

		SDL_SetAlpha(sk->win, SDL_RLEACCEL, 255);

		SDL_SetAlpha(option->win, SDL_RLEACCEL, 255);

		SDL_SetAlpha(sw->win, SDL_RLEACCEL, 255);

		SDL_SetAlpha(ptrade->win, SDL_RLEACCEL, 255);

		SDL_SetAlpha(tp->win, SDL_RLEACCEL, 255);

		SDL_SetAlpha(fw->win, SDL_RLEACCEL, 255);

		SDL_SetAlpha(pms->win, SDL_RLEACCEL, 255);

		SDL_SetAlpha(skillShow, SDL_RLEACCEL, 255);

		for(unsigned int lp2 = 0; lp2 < npc.size(); lp2++)
		{
			if(npc.at(lp2)->dialogWindow != NULL)
				SDL_SetAlpha(npc.at(lp2)->dialogWindow, SDL_RLEACCEL, 255);
		}

		for(unsigned int i = 0; i < iw.size(); i++)
		{
			if(iw.at(i)->win != NULL)
				SDL_SetAlpha(iw.at(i)->win, SDL_RLEACCEL, 255);
		}

		for(unsigned int i = 0; i < pi.size(); i++)
		{
			if(pi.at(i)->win != NULL)
				SDL_SetAlpha(pi.at(i)->win, SDL_RLEACCEL, 255);
		}

		for(unsigned int i = 0; i < msgs.size(); i++)
		{
			if(msgs.at(i)->win != NULL)
				SDL_SetAlpha(msgs.at(i)->win, SDL_RLEACCEL, 255);
		}
	}
}
#include <SDL_TTF.h>
#include <SDL_image.h>
#include <string>

#include "Login.h"
#include "Animation.h"
#include "Misc.h"
#include "Preload.h"
#include "Music.h"
#include "Main.h"
#include "Friend.h"

using namespace std;

extern SDL_Surface *screen;

extern SDL_Surface *chatNot;
extern SDL_Surface *chatInfo;

extern vector<Character*> cv;
extern vector<Monster*> cm;
extern vector<Window*> win;
extern vector<NPC*> npc;
extern vector<Notifer*> nv;
extern vector<Bullet*> bullets;
extern vector<AddStatWindow*> addstatwindow;
extern vector<PartyWindow*> partyw;
extern vector<PartyInvite*> pi;
extern vector<Icon*> icons;
extern vector<Drop*> drops;
extern vector<Damage> dmgs;
extern vector<Animation*> anis;
extern vector<Message*> msgs;
extern vector<InputWindow*> iw;
extern vector<LoginMessage*> lmsgs;
extern vector<CharacterSelect> charsel;
extern vector<LoginInput*> linputs;
extern vector<Skill*> skill;

extern Character *me;

extern TTF_Font *cour;
extern SDL_Color clrFg;
extern SDL_Color clrFgBlack;

extern bool spawned;
extern bool chatting;
extern bool colorkey;
extern bool chatWindow;
extern bool drag;
extern bool showFps;
extern bool chatInputWindow;
extern bool chatMessage;

extern string chat;

extern Mark *mark;
extern Inventory *inv;
extern Drag dragged;
extern Taskbar *task;
extern SkillWindow *sk;
//extern Skill *skills;
extern ObjectInfoWindow *ow;
extern OptionWindow *option;
extern ShopWindow *sw;
extern PlayerTrade *ptrade;
extern TeleportWindow *tp;
extern FriendWindow *fw;
extern ChatManager *chatFunc;
extern PrivateMessage *pms;

extern float currBackX;
extern float currBackY;

extern float cameraX;
extern float cameraY;

extern int camShowX;
extern int camShowToX;
extern int camShowY;
extern int camShowToY;

extern int skillLoaded;

extern bool transWindow;

extern Animation chatIcon;

extern SDL_Surface *barFull;
extern SDL_Surface *barItem;

extern SDL_Surface *manaBar;
extern SDL_Surface *hpBar;
extern SDL_Surface *eBar;

extern SDL_Surface *markItemShop;

SDL_Surface *fpsSurface = NULL;
SDL_Surface *pingSurface = NULL;
SDL_Surface *loadingSurface = NULL;

extern SDL_Surface *loginScreen;
extern SDL_Surface *loginFirst;
extern SDL_Surface *loginChar;
extern SDL_Surface *loginRem;

extern int logplace;
extern LoginWriter write;

extern Map *map;
extern std::vector<Tile*> preRender;

extern int resX;
extern int resY;

extern int chatPlaceW;

extern int job;

extern SDL_Surface *boxFull;
extern SDL_Surface *boxEmpty;

extern SDL_Surface *skillShow;

void UpdateImage(SDL_Surface *_image, int _x, int _y)
{
	if(_image != NULL)
	{
		SDL_Rect re = {_x, _y, _image->w, _image->h};

		SDL_BlitSurface(_image, NULL, screen, &re);
	}
}

void UpdateAll()
{
	if(screen->w > msize(MAPX) || screen->h > msize(MAPY))
		SDL_FillRect(screen, NULL, 0x00000000);

	if(map != NULL)
	{
		for(unsigned int i = 0; i < preRender.size(); i++)
			UpdateImage(preRender.at(i)->su, preRender.at(i)->cX, preRender.at(i)->cY);
	}

	if(drops.size() > 0)
	{
		for(unsigned int i = 0; i < drops.size(); i++)
		{
			if(drops.at(i)->inWin)
				UpdateImage(drops.at(i)->pic, drops.at(i)->cX, drops.at(i)->cY);
		}
	}

	if(cv.size() > 0)
	{
		for(unsigned int i = 0; i < cv.size(); i++)
		{
			if(cv.at(i)->inWin)
			{
				UpdateImage(cv.at(i)->my, cv.at(i)->cX, cv.at(i)->cY);

				if(cv.at(i)->showMenu)
				{
					int cY = (cv.at(i)->cY + ((cv.at(i)->my->h / 2) - (33 / 2)));
					int cX = (cv.at(i)->cX + cv.at(i)->my->w);

					int maxX = cX + cv.at(i)->menu.at(0).btn->w;
					int maxY = cY + (i * cv.at(i)->menu.at(0).btn->h + 1);

					if(maxX > resX)
					{
						int dif = maxX - resX;
						cX -= dif;
					}

					if(maxY > resY)
					{
						int dif = maxY - resY;
						cY -= dif;
					}

					for(unsigned int y = 0; y < cv.at(i)->menu.size(); y++)
					{
						UpdateImage(cv.at(i)->menu.at(y).btn, cX, cY);
						UpdateImage(cv.at(i)->menu.at(y).tSur, (cX + 8), (cY + 9));

						cY += (cv.at(i)->menu.at(y).btn->h + 1);
					}
				}

				if(cv.at(i)->equipped && cv.at(i)->weapon != NULL)
				{
					switch(cv.at(i)->wMove)
					{
						case LEFT: UpdateImage(cv.at(i)->weapon, (cv.at(i)->cX - (cv.at(i)->weapon->w + 1)), (cv.at(i)->cY + (cv.at(i)->my->w / 2)));
							break;

						case RIGHT: UpdateImage(cv.at(i)->weapon, (cv.at(i)->cX + cv.at(i)->my->w), (cv.at(i)->cY + (cv.at(i)->my->w / 2)));
							break;
					}
				}

				if(cv.at(i)->hat != NULL)
					UpdateImage(cv.at(i)->hat, (cv.at(i)->cX + getEquipPos(PLAYX, cv.at(i)->hatid)), (cv.at(i)->cY + getEquipPos(PLAYY, cv.at(i)->hatid)));

				if(cv.at(i)->suit != NULL)
					UpdateImage(cv.at(i)->suit, (cv.at(i)->cX + getEquipPos(PLAYX, cv.at(i)->suitid)), (cv.at(i)->cY + getEquipPos(PLAYY, cv.at(i)->suitid)));

				if(cv.at(i)->belt != NULL)
					UpdateImage(cv.at(i)->belt, (cv.at(i)->cX + getEquipPos(PLAYX, cv.at(i)->beltid)), (cv.at(i)->cY + getEquipPos(PLAYY, cv.at(i)->beltid)));

				if(cv.at(i)->c != NULL)
				{
					if((unsigned)cv.at(i)->c->removeTime <= getTime(true))
					{
						SDL_FreeSurface(cv.at(i)->c->theWindow);
						SDL_FreeSurface(cv.at(i)->c->text1);

						if(cv.at(i)->c->text2 != NULL)
							SDL_FreeSurface(cv.at(i)->c->text2);

						delete cv.at(i)->c;
						cv.at(i)->c = NULL;
					}

					else
					{
						UpdateImage(cv.at(i)->c->theWindow, cv.at(i)->cX, ((cv.at(i)->cY - cv.at(i)->c->theWindow->h) - 16));
						UpdateImage(cv.at(i)->c->text1, (cv.at(i)->cX + 8), ((cv.at(i)->cY - cv.at(i)->c->theWindow->h) - 2));

						if(cv.at(i)->c->text2 != NULL)
							UpdateImage(cv.at(i)->c->text2, (cv.at(i)->cX + 8), ((cv.at(i)->cY - cv.at(i)->c->theWindow->h) + 14));
					}
				}

				int setXProc2 = cv.at(i)->cX;

				if(cv.at(i)->my->w <= cv.at(i)->nameSur.first()->w)
					setXProc2 -= ((cv.at(i)->nameSur.first()->w - cv.at(i)->my->w) / 2);

				else
					setXProc2 += ((cv.at(i)->my->w / 2) - (cv.at(i)->nameSur.first()->w / 2));

				cv.at(i)->nameSur.draw(setXProc2, (int)(cv.at(i)->cY - 16));

				if(cv.at(i)->dead)
				{
					int setXDead = cv.at(i)->cX;

					if(cv.at(i)->my->w <= cv.at(i)->deadImg->w)
						setXDead -= ((cv.at(i)->deadImg->w - cv.at(i)->my->w) / 2);
					else
						setXDead += ((cv.at(i)->my->w / 2) - (cv.at(i)->deadImg->w / 2));

					UpdateImage(cv.at(i)->deadImg, setXDead, (cv.at(i)->cY - 32));
				}

				if(cv.at(i)->showProc || cv.at(i)->inParty)
				{
					int setXProc = cv.at(i)->cX;

					if(cv.at(i)->my->w <= barFull->w)
						setXProc -= ((barFull->w - cv.at(i)->my->w) / 2);
					else
						setXProc += ((cv.at(i)->my->w / 2) - (barFull->w / 2));

					int procs = (cv.at(i)->proc / 10);
					int theY = (cv.at(i)->cY - 25);

					if(cv.at(i)->dead)
						theY -= 14;

					UpdateImage(barFull, setXProc, theY);

					if(procs > 10)
						procs = 10;

					for(int t = 0; t < procs; t++)
					{
						UpdateImage(barItem, (setXProc + 1), (theY + 1));

						setXProc += barItem->w;
					}
				}
			}
		}
	}

	UpdateImage(me->my, (int)cameraX, (int)cameraY);
	
	if(me->equipped && me->weapon != NULL)
	{
		switch(me->wMove)
		{
			case LEFT: UpdateImage(me->weapon, (int)(cameraX - (me->weapon->w + 1)), (int)(cameraY + (me->my->w / 2)));
				break;

			case RIGHT: UpdateImage(me->weapon, (int)(cameraX + me->my->w), (int)(cameraY + (me->my->w / 2)));
				break;
		}
	}

	if(me->hat != NULL)
		UpdateImage(me->hat, (int)(cameraX + getEquipPos(PLAYX, inv->eq[1]->id)), (int)(cameraY + getEquipPos(PLAYY, inv->eq[1]->id)));

	if(me->suit != NULL)
		UpdateImage(me->suit, (int)(cameraX + getEquipPos(PLAYX, inv->eq[2]->id)), (int)(cameraY + getEquipPos(PLAYY, inv->eq[2]->id)));

	if(me->belt != NULL)
		UpdateImage(me->belt, (int)(cameraX + getEquipPos(PLAYX, inv->eq[3]->id)), (int)(cameraY + getEquipPos(PLAYY, inv->eq[3]->id)));

	if(me->c != NULL)
	{
		if((unsigned)me->c->removeTime <= getTime(true))
		{
			SDL_FreeSurface(me->c->theWindow);
			SDL_FreeSurface(me->c->text1);

			if(me->c->text2 != NULL)
				SDL_FreeSurface(me->c->text2);

			delete me->c;
			me->c = NULL;
		}

		else
		{
			UpdateImage(me->c->theWindow, (int)cameraX, (int)((cameraY - me->c->theWindow->h) - 16));
			UpdateImage(me->c->text1, (int)(cameraX + 8), (int)((cameraY - me->c->theWindow->h) - 2));

			if(me->c->text2 != NULL)
				UpdateImage(me->c->text2, (int)(cameraX + 8), (int)((cameraY - me->c->theWindow->h) + 14));
		}
	}

	int setXProc = (int)cameraX;

	if(me->my->w <= me->nameSur.first()->w)
		setXProc -= ((me->nameSur.first()->w - me->my->w) / 2);

	else
		setXProc += ((me->my->w / 2) - (me->nameSur.first()->w / 2));

	me->nameSur.draw(setXProc, (int)(cameraY - 16));

	int setXProc2 = (int)cameraX;

	if(me->my->w <= barFull->w)
		setXProc2 -= ((barFull->w - me->my->w) / 2);
	else
		setXProc2 += ((me->my->w / 2) - (barFull->w / 2));

	float procs = ((((float)me->hp / (float)me->maxhp) * 100.0f) / 10.0f);
	int theY = ((int)cameraY - 25);

	if(procs > 10)
		procs = 10;

	if(me->dead)
		theY -= 14;

	UpdateImage(barFull, setXProc2, theY);

	for(int t = 0; t < (int)procs; t++)
	{
		UpdateImage(barItem, (setXProc2 + 1), (theY + 1));

		setXProc2 += barItem->w;
	}

	if(cm.size() > 0)
	{
		for(unsigned int i = 0; i < cm.size(); i++)
		{
			if(cm.at(i)->inWin)
			{
				if(cm.at(i)->fades)
				{
					if(cm.at(i)->fadep > 255)
						cm.at(i)->fadep = 255;

					else if(cm.at(i)->fadep < 0)
						cm.at(i)->fadep = 0;

					SDL_SetAlpha(cm.at(i)->m, SDL_SRCALPHA | SDL_RLEACCEL, cm.at(i)->fadep);
					UpdateImage(cm.at(i)->m, cm.at(i)->cX, cm.at(i)->cY);
					SDL_SetAlpha(cm.at(i)->m, SDL_SRCALPHA, 255);
				}

				else
				{
					UpdateImage(cm.at(i)->m, cm.at(i)->cX, cm.at(i)->cY);
				}

				int setX = cm.at(i)->cX;

				if(cm.at(i)->showProc)
				{
					if(cm.at(i)->showLvl)
					{
						if(cm.at(i)->m->w <= cm.at(i)->whenMarkName->w)
							setX -= ((cm.at(i)->whenMarkName->w - cm.at(i)->m->w) / 2);
						else
							setX += ((cm.at(i)->m->w / 2) - (cm.at(i)->whenMarkName->w / 2));

						UpdateImage(cm.at(i)->whenMarkName, setX, (cm.at(i)->cY - 16));
					}

					else
					{
						if(cm.at(i)->m->w <= cm.at(i)->nameSurface->w)
							setX -= ((cm.at(i)->nameSurface->w - cm.at(i)->m->w) / 2);
						else
							setX += ((cm.at(i)->m->w / 2) - (cm.at(i)->nameSurface->w / 2));

						UpdateImage(cm.at(i)->nameSurface, setX, (cm.at(i)->cY - 16));
					}

					int setXProc = cm.at(i)->cX;

					if(cm.at(i)->m->w <= barFull->w)
						setXProc -= ((barFull->w - cm.at(i)->m->w) / 2);
					else
						setXProc += ((cm.at(i)->m->w / 2) - (barFull->w / 2));

					int procs = (cm.at(i)->proc / 10);

					UpdateImage(barFull, setXProc, (cm.at(i)->cY - 25));

					for(int t = 0; t < procs; t++)
					{
						UpdateImage(barItem, (setXProc + 1), (cm.at(i)->cY - 24));

						setXProc += barItem->w;
					}
				}

				else
				{
					if(cm.at(i)->m->w <= cm.at(i)->nameSurface->w)
						setX -= ((cm.at(i)->nameSurface->w - cm.at(i)->m->w) / 2);
					else
						setX += ((cm.at(i)->m->w / 2) - (cm.at(i)->nameSurface->w / 2));

					UpdateImage(cm.at(i)->nameSurface, setX, (cm.at(i)->cY - 16));
				}
			}
		}
	}

	if(dmgs.size() > 0)
	{
		for(unsigned int z = 0; z < dmgs.size(); z++)
		{
			if(dmgs.at(z).dmgItem != NULL)
			{
				UpdateImage(dmgs.at(z).dmgItem, (int)dmgs.at(z).x, (int)dmgs.at(z).y);
				UpdateImage(dmgs.at(z).dmg, (int)dmgs.at(z).x, ((int)dmgs.at(z).y + 14));
			}

			else
				UpdateImage(dmgs.at(z).dmg, (int)dmgs.at(z).x, (int)dmgs.at(z).y);
		}
	}

	if(mark->enable)
	{
		UpdateImage(mark->leftdown, mark->leftdX, mark->leftdY);
		UpdateImage(mark->rightdown, mark->rightdX, mark->rightdY);
		UpdateImage(mark->leftup, mark->leftuX, mark->leftuY);
		UpdateImage(mark->rightup, mark->rightuX, mark->rightuY);
	}

	if(npc.size() > 0)
	{
		for(unsigned int i = 0; i < npc.size(); i++)
		{
			if(npc.at(i)->inWin)
			{
				UpdateImage(npc.at(i)->my, npc.at(i)->cX, npc.at(i)->cY);

				if(npc.at(i)->showMenu)
				{
					int cY = (npc.at(i)->cY + ((npc.at(i)->my->h / 2) - (33 / 2)));
					int cX = (npc.at(i)->cX + npc.at(i)->my->w);

					int maxX = cX + npc.at(i)->menu.at(0).btn->w;
					int maxY = cY + (i * npc.at(i)->menu.at(0).btn->h + 1);

					if(maxX > resX)
					{
						int dif = maxX - resX;
						cX -= dif;
					}

					if(maxY > resY)
					{
						int dif = maxY - resY;
						cY -= dif;
					}

					for(unsigned int y = 0; y < npc.at(i)->menu.size(); y++)
					{
						UpdateImage(npc.at(i)->menu.at(y).btn, cX, cY);
						UpdateImage(npc.at(i)->menu.at(y).tSur, (cX + 8), (cY + 9));

						cY += (npc.at(i)->menu.at(y).btn->h + 1);
					}
				}

				if(npc.at(i)->showDialog)
				{
					UpdateImage(npc.at(i)->dialogWindow, npc.at(i)->dialogX, npc.at(i)->dialogY);
					UpdateImage(npc.at(i)->dialogWindowName, (npc.at(i)->dialogX + 31), (npc.at(i)->dialogY + 1));
					UpdateImage(npc.at(i)->quitx, ((npc.at(i)->dialogX + npc.at(i)->dialogWindow->w) - (npc.at(i)->quitx->w + 2)), npc.at(i)->dialogY);
					
					for(unsigned int y = 0; y < npc.at(i)->npct.size(); y++)
						UpdateImage(npc.at(i)->npct.at(y).text, (npc.at(i)->dialogX + npc.at(i)->npct.at(y).x), (npc.at(i)->dialogY + npc.at(i)->npct.at(y).y));

					int x = (npc.at(i)->dialogX + 24);
					int y = (npc.at(i)->dialogY + 149);

					for(unsigned int t = 0; t < npc.at(i)->diab.size(); t++)
					{
						UpdateImage(npc.at(i)->diab.at(t).btn, x, y);
						UpdateImage(npc.at(i)->diab.at(t).tSur, (x + 11), (y + 3));

						x += (npc.at(i)->diab.at(t).btn->w + 10);
					}
				}

				else
				{
					if(npc.at(i)->c != NULL)
					{
						if((unsigned)npc.at(i)->c->removeTime <= getTime(true))
						{
							SDL_FreeSurface(npc.at(i)->c->theWindow);
							SDL_FreeSurface(npc.at(i)->c->text1);

							if(npc.at(i)->c->text2 != NULL)
								SDL_FreeSurface(npc.at(i)->c->text2);

							delete npc.at(i)->c;
							npc.at(i)->c = NULL;
						}

						else
						{
							int updateChatX = npc.at(i)->cX;
							int updateChatY = ((npc.at(i)->cY - npc.at(i)->c->theWindow->h) - 16);

							if(updateChatY < 0)
								updateChatY = 0;

							if((updateChatX + npc.at(i)->c->theWindow->w) > screen->w)
								updateChatX -= ((updateChatX + npc.at(i)->c->theWindow->w) - screen->w);

							UpdateImage(npc.at(i)->c->theWindow, updateChatX, updateChatY);
							UpdateImage(npc.at(i)->c->text1, (updateChatX + 8), (updateChatY + 14));

							if(npc.at(i)->c->text2 != NULL)
								UpdateImage(npc.at(i)->c->text2, (updateChatX + 8), (updateChatY + 30));
						}
					}
				}

				int setX = npc.at(i)->cX;

				if(npc.at(i)->my->w <= npc.at(i)->nameSurface->w)
					setX -= ((npc.at(i)->nameSurface->w - npc.at(i)->my->w) / 2);
				else
					setX += ((npc.at(i)->my->w / 2) - (npc.at(i)->nameSurface->w / 2));

				UpdateImage(npc.at(i)->nameSurface, setX, (npc.at(i)->cY - 16));
			}
		}
	}

	if(bullets.size() > 0)
	{
		for(unsigned int i = 0; i < bullets.size(); i++)
		{
			if(bullets.at(i)->inWin)
				UpdateImage(bullets.at(i)->cPic, bullets.at(i)->sX, bullets.at(i)->sY);
		}
	}

	if(anis.size() > 0)
	{
		for(unsigned int i = 0; i < anis.size(); i++)
		{
			if(anis.at(i)->inWin)
				UpdateImage(anis.at(i)->ani.at((anis.at(i)->frame - 1)), anis.at(i)->cX, anis.at(i)->cY);
		}
	}

	if(partyw.size() > 0)
	{
		for(unsigned int i = 0; i < partyw.size(); i++)
		{
			if(partyw.at(i)->enable)
			{
				UpdateImage(partyw.at(i)->window, partyw.at(i)->x, partyw.at(i)->y);
				UpdateImage(partyw.at(i)->quitx, ((partyw.at(i)->x + partyw.at(i)->window->w) - (partyw.at(i)->quitx->w + 2)), partyw.at(i)->y);

				UpdateImage(partyw.at(i)->leave, (partyw.at(i)->x + 7), (partyw.at(i)->y + 188));
				UpdateImage(partyw.at(i)->kick, (partyw.at(i)->x + 146), (partyw.at(i)->y + 188));
				UpdateImage(partyw.at(i)->invite, (partyw.at(i)->x + 284), (partyw.at(i)->y + 188));

				UpdateImage(partyw.at(i)->levelSur, (partyw.at(i)->x + 315), (partyw.at(i)->y + 21));

				if(partyw.at(i)->pm.size() > 0)
				{
					int startX = (partyw.at(i)->x + 28);
					int startXLvl = (partyw.at(i)->x + 303);
					int startY = (partyw.at(i)->y + 88);

					for(unsigned int m = 0; m < partyw.at(i)->pm.size(); m++)
					{
						UpdateImage(partyw.at(i)->pm.at(m).names, startX, startY);
						UpdateImage(partyw.at(i)->pm.at(m).lvls, startXLvl, startY);

						startY += 16;
					}
				}
			}
		}
	}

	if(win.size() > 0)
	{
		for(unsigned int i = 0; i < win.size(); i++)
		{
			if(win.at(i)->TYPE == CHATWINDOW)
			{
				if(chatting || chatWindow)
				{
					if(!win.at(i)->enable)
						win.at(i)->enable = true;
				}
			}

			if(win.at(i)->enable)
			{
				UpdateImage(win.at(i)->w, win.at(i)->x, win.at(i)->y);

				if(win.at(i)->enableX)
					UpdateImage(win.at(i)->quitx, ((win.at(i)->x + win.at(i)->w->w) - (win.at(i)->quitx->w + 2)), win.at(i)->y);

				if(win.at(i)->TYPE == HPWINDOW)
				{
					float tDone = (((float)me->hp / (float)me->maxhp) * 100.0f);

					int noCrash = (int)tDone;
					int cx = (win.at(i)->x + 59);

					for(int t = 0; t < noCrash; t++)
					{
						UpdateImage(hpBar, cx, win.at(i)->y + 6);
						//if(t == 0)
						//	UpdateImage(hpBarStart, cx, (win.at(i)->y + 25));

						//else if(t == 9)
						//	UpdateImage(hpBarStop, cx, (win.at(i)->y + 25));

						//else
						//	UpdateImage(hpBar, cx, (win.at(i)->y + 25));

						cx += 1;
					}

					if(me->maxmana > 0)
						tDone = (((float)me->mana / (float)me->maxmana) * 100.0f);
					else
						tDone = 0.0f;

					cx = (win.at(i)->x + 59);
					noCrash = (int)tDone;

					for(int t = 0; t < noCrash; t++)
					{
						UpdateImage(manaBar, cx, win.at(i)->y + 29);
						cx += 1;
					}

					if(me->maxenergy > 0)
						tDone = (((float)me->energy / (float)me->maxenergy) * 100.0f);
					else
						tDone = 0.0f;

					cx = (win.at(i)->x + 59);
					noCrash = (int)tDone;

					for(int t = 0; t < noCrash; t++)
					{
						UpdateImage(eBar, cx, win.at(i)->y + 52);
						cx += 1;
					}

					for(unsigned int r = 0; r < win.size(); r++)
					{
						if(win.at(r)->TYPE == INFOWINDOW)
						{
							UpdateImage(win.at(r)->hpSurface, ((win.at(i)->x + 59) + (50 - (win.at(r)->hpSurface->w / 2))), ((win.at(i)->y + 6) + (10 - (win.at(r)->hpSurface->h / 2))));

							break;
						}
					}

					UpdateImage(me->manaSurface, ((win.at(i)->x + 59) + (50 - (me->manaSurface->w / 2))), ((win.at(i)->y + 29) + (10 - (me->manaSurface->h / 2))));
					UpdateImage(me->energySurface, ((win.at(i)->x + 59) + (50 - (me->energySurface->w / 2))), ((win.at(i)->y + 52) + (10 - (me->energySurface->h / 2))));
				}

				else if(win.at(i)->TYPE == INFOWINDOW)
				{
					int tY = (win.at(i)->y + 6);

					UpdateImage(win.at(i)->levelSurface, (win.at(i)->x + 88), tY);
					tY += 26;

					UpdateImage(win.at(i)->expSurface, (win.at(i)->x + 88), tY);
					tY += 26;

					UpdateImage(win.at(i)->hpSurface, (win.at(i)->x + 88), tY);
				}

				else if(win.at(i)->TYPE == INVITEINPUTWINDOW || win.at(i)->TYPE == KICKINPUTWINDOW || win.at(i)->TYPE == FRIENDINPUTWINDOW)
				{
					UpdateImage(win.at(i)->inviteButton, (win.at(i)->x + 95), (win.at(i)->y + 68));
				}
			}
		}
	}

	if(option->enable)
	{
		UpdateImage(option->win, option->x, option->y);
		UpdateImage(option->quitx, ((option->x + option->win->w) - (option->quitx->w + 2)), option->y);

		int startX = (option->x + 186);
		int startY = (option->y + 62);

		for(unsigned int i = 0; i < option->op.size(); i++)
		{
			UpdateImage(option->op.at(i).name, (option->x + 25), startY);
			
			if(option->op.at(i).active)
				UpdateImage(option->buttonFull, startX, startY);
			else
				UpdateImage(option->buttonEmpty, startX, startY);

			startY += 32;
		}
	}

	if((chatMessage && pms->pm.size() > 0))
	{
	}

	else if(chatInputWindow && iw.size() > 0)
	{
	}

	else if(chatWindow || chatting)
	{
		if(!win.at(1)->enable)
			win.at(1)->enable = true;

		if(chatting)
			UpdateImage(chatNot, 0, 0);

		for(unsigned int i = 0; i < win.size(); i++)
		{
			if(win.at(i)->enable && (win.at(i)->TYPE == INVITEINPUTWINDOW || win.at(i)->TYPE == KICKINPUTWINDOW) || win.at(i)->TYPE == FRIENDINPUTWINDOW && chatWindow)
			{
				if(chat != "")
				{
					UpdateImage(chatInfo, (win.at(i)->x + 10), (win.at(i)->y + 42));
					UpdateImage(chatIcon.ani.at((chatIcon.frame - 1)), ((win.at(i)->x + 10) + chatPlaceW), (win.at(i)->y + 42));
				}

				else
					UpdateImage(chatIcon.ani.at((chatIcon.frame - 1)), ((win.at(i)->x + 10)), (win.at(i)->y + 42));	
			}

			if(win.at(i)->enable && win.at(i)->TYPE == CHATWINDOW && chatting)
			{
				if(chat != "")
				{
					UpdateImage(chatInfo, (win.at(i)->x + 8), (win.at(i)->y + 97));
					UpdateImage(chatIcon.ani.at((chatIcon.frame - 1)), (win.at(i)->x + 8 + chatPlaceW), (win.at(i)->y + 96));
				}
				
				else
					UpdateImage(chatIcon.ani.at((chatIcon.frame - 1)), (win.at(i)->x + 8), (win.at(i)->y + 96));
			}
		}
	}

	if(nv.size() > 0)
	{
		for(unsigned int i = 0; i < nv.size(); i++)
		{
			if((unsigned)nv.at(i)->sTime <= getTime(true))
				UpdateImage(nv.at(i)->msg, nv.at(i)->x, nv.at(i)->y);
		}
	}

	if(chatFunc->tps.size() > 0 && win.at(1)->enable)
	{
		int wrote = 0;
		int display = 0;

		for(unsigned int i = 0; i < chatFunc->tps.size(); i++)
		{
			if(wrote >= 5)
				break;

			if((chatFunc->startY + ((int)i * 16)) >= (win.at(1)->y + 5))
			{
				UpdateImage(chatFunc->tps.at(i).message, win.at(1)->x + 6, win.at(1)->y + 5 + display);

				wrote++;
				display += 16;
			}
		}

		if(chatFunc->goUp)
			UpdateImage(fw->noteup, win.at(1)->x + 439, win.at(1)->y + 7);

		if(chatFunc->goDown)
			UpdateImage(fw->notedown, win.at(1)->x + 439, win.at(1)->y + 75);
	}

	if(addstatwindow.size() > 0)
	{
		for(unsigned int i = 0; i < addstatwindow.size(); i++)
		{
			if(addstatwindow.at(i)->enable)
			{
				UpdateImage(addstatwindow.at(i)->window, addstatwindow.at(i)->x, addstatwindow.at(i)->y);
				UpdateImage(addstatwindow.at(i)->quitx, ((addstatwindow.at(i)->x + addstatwindow.at(i)->window->w) - addstatwindow.at(i)->quitx->w), addstatwindow.at(i)->y);

				UpdateImage(addstatwindow.at(i)->add, (addstatwindow.at(i)->x + 153), (addstatwindow.at(i)->y + 43));
				UpdateImage(addstatwindow.at(i)->add, (addstatwindow.at(i)->x + 153), (addstatwindow.at(i)->y + 75));
				UpdateImage(addstatwindow.at(i)->add, (addstatwindow.at(i)->x + 153), (addstatwindow.at(i)->y + 107));
				UpdateImage(addstatwindow.at(i)->add, (addstatwindow.at(i)->x + 153), (addstatwindow.at(i)->y + 140));

				UpdateImage(addstatwindow.at(i)->power, (addstatwindow.at(i)->x + 109), (addstatwindow.at(i)->y + 47));
				UpdateImage(addstatwindow.at(i)->hp, (addstatwindow.at(i)->x + 109), (addstatwindow.at(i)->y + 80));
				UpdateImage(addstatwindow.at(i)->agility, (addstatwindow.at(i)->x + 109), (addstatwindow.at(i)->y + 113));
				UpdateImage(addstatwindow.at(i)->intt, (addstatwindow.at(i)->x + 109), (addstatwindow.at(i)->y + 144));

				UpdateImage(addstatwindow.at(i)->pointsav, (addstatwindow.at(i)->x + 168), (addstatwindow.at(i)->y + 177));

				UpdateImage(addstatwindow.at(i)->names, (addstatwindow.at(i)->x + 84), (addstatwindow.at(i)->y + 255));
				UpdateImage(addstatwindow.at(i)->pkpoints, (addstatwindow.at(i)->x + 119), (addstatwindow.at(i)->y + 294));
				UpdateImage(addstatwindow.at(i)->servername, (addstatwindow.at(i)->x + 88), (addstatwindow.at(i)->y + 336));
			}
		}
	}

	if(pi.size() > 0)
	{
		for(unsigned int i = 0; i < pi.size(); i++)
		{
			UpdateImage(pi.at(i)->win, pi.at(i)->x, pi.at(i)->y); 
			UpdateImage(pi.at(i)->from, (pi.at(i)->x + 9), (pi.at(i)->y + 48));
			UpdateImage(pi.at(i)->yes, (pi.at(i)->x + 15), (pi.at(i)->y + 83));
			UpdateImage(pi.at(i)->no, (pi.at(i)->x + 128), (pi.at(i)->y + 83));
		}
	}

	if(msgs.size() > 0)
	{
		for(unsigned int i = 0; i < msgs.size(); i++)
		{
			UpdateImage(msgs.at(i)->win, msgs.at(i)->x, msgs.at(i)->y);
			UpdateImage(msgs.at(i)->head, ((msgs.at(i)->x + ((msgs.at(i)->win->w / 2) - (msgs.at(i)->head->w / 2)))), (msgs.at(i)->y + 9));
			UpdateImage(msgs.at(i)->text, ((msgs.at(i)->x + ((msgs.at(i)->win->w / 2) - (msgs.at(i)->text->w / 2)))), (msgs.at(i)->y + 50));

			for(unsigned int y = 0; y < msgs.at(i)->btn.size(); y++)
				UpdateImage(msgs.at(i)->btn.at(y)->sur, msgs.at(i)->btn.at(y)->x, msgs.at(i)->btn.at(y)->y);
		}
	}

	if(icons.size() > 0)
	{
		for(unsigned int i = 0; i < icons.size(); i++)
		{
			UpdateImage(icons.at(i)->icon, icons.at(i)->x, icons.at(i)->y);
			UpdateImage(icons.at(i)->name1, icons.at(i)->name1X, icons.at(i)->name1Y);
			UpdateImage(icons.at(i)->name2, icons.at(i)->name2X, icons.at(i)->name2Y);
			UpdateImage(icons.at(i)->tSurface, icons.at(i)->timeLeftX, icons.at(i)->timeLeftY);
		}
	}

	if(sk->enable)
	{
		UpdateImage(sk->win, sk->x, sk->y);
		UpdateImage(sk->quitx, (sk->x + (sk->win->w - sk->quitx->w)), sk->y);

		for(unsigned int i = 0; i < skill.size(); i++)
		{
			if((i + 1) > 6)
				break;

			int setX = ((sk->x + skill.at(i)->x + 8 - 66) + (41 - skill.at(i)->nameSkill->w / 2));
			int setY = ((sk->y + skill.at(i)->y + 2 - 44) + (8 - skill.at(i)->nameSkill->h / 2));
			
			int setPicX = ((sk->x + skill.at(i)->x + 4 - 66) + (46 - skill.at(i)->pic->w / 2));
			int setPicY = ((sk->y + skill.at(i)->y + 17 - 44) + (12 - skill.at(i)->pic->h / 2));

			UpdateImage(skillShow, sk->x + skill.at(i)->x - 66, sk->y + skill.at(i)->y - 44);
			UpdateImage(skill.at(i)->text, (sk->x + skill.at(i)->x), (sk->y + skill.at(i)->y));
			UpdateImage(skill.at(i)->pic, setPicX, setPicY);
			UpdateImage(skill.at(i)->nameSkill, setX, setY);
		}
		/*
		for(int i = 0; i < skillLoaded; i++)
			UpdateImage(skills[i].text, (sk->x + skills[i].x), (sk->y + skills[i].y));
			*/
	}

	if(inv->enable)
	{
		UpdateImage(inv->inv, inv->x, inv->y);
		UpdateImage(inv->quitx, (inv->x + (inv->inv->w - inv->quitx->w)), inv->y);
		UpdateImage(inv->golds, (inv->x + 63), (inv->y + 258));

		for(int i = 0; i < 25; i++)
		{
			if(inv->items[i] != NULL)
			{
				UpdateImage(inv->items[i]->pic, inv->items[i]->x, inv->items[i]->y);

				if(inv->items[i]->amount != NULL && inv->items[i]->maxstack > 1)
					UpdateImage(inv->items[i]->amount, (inv->items[i]->x - 5), (((inv->items[i]->y + inv->items[i]->pic->h) - inv->items[i]->amount->h) + 5));
			}
		}

		for(int i = 0; i < 4; i++)
		{
			if(inv->eq[i] != NULL)
				UpdateImage(inv->eq[i]->pic, (inv->x + getEquipPosX(inv->eq[i]->slot)), (inv->y + getEquipPosY(inv->eq[i]->slot)));
		}
	}

	if(sw->enable)
	{
		UpdateImage(sw->win, sw->x, sw->y);
		UpdateImage(sw->quitx, (sw->x + (sw->win->w - sw->quitx->w)), sw->y);

		UpdateImage(sw->shopName, (sw->x + 64 + (100 - (sw->shopName->w / 2))), (sw->y + 20));
		UpdateImage(sw->currPrice, (sw->x + 71), (sw->y + 282));
		UpdateImage(sw->currQty, (sw->x + 216), (sw->y + 282));
		UpdateImage(sw->totalPrice, (sw->x +107), (sw->y + 426));

		for(int i = 0; i < 20; i++)
		{
			if(sw->si[i].itemId != 0)
				UpdateImage(sw->si[i].pic, sw->si[i].x, sw->si[i].y);

			if(sw->cart[i].itemId != 0)
			{
				UpdateImage(sw->cart[i].pic, sw->cart[i].x, sw->cart[i].y);

				if(sw->cart[i].picStack != NULL && getItemStack(sw->cart[i].itemId) > 1)
					UpdateImage(sw->cart[i].picStack, (sw->cart[i].x - 5), (((sw->cart[i].y + sw->cart[i].pic->h) - sw->cart[i].picStack->h) + 5));
			}
		}

		UpdateImage(markItemShop, (sw->si[(sw->cMark - 1)].x - 1), (sw->si[(sw->cMark - 1)].y - 1));
	}

	if(ptrade->enable)
	{
		UpdateImage(ptrade->win, ptrade->x, ptrade->y);
		UpdateImage(ptrade->quitx, (ptrade->x + (ptrade->win->w - ptrade->quitx->w)), ptrade->y);

		UpdateImage(ptrade->tradeName1, (ptrade->x + 41), (ptrade->y + 68));
		UpdateImage(ptrade->tradeName2, (ptrade->x + 243), (ptrade->y + 68));

		UpdateImage(ptrade->status1, (ptrade->x + 84), (ptrade->y + 90));
		UpdateImage(ptrade->status2, (ptrade->x + 286), (ptrade->y + 90));

		if(ptrade->mySide == LEFT)
			UpdateImage(ptrade->ok, (ptrade->x + 62), (ptrade->y + 277));

		else
			UpdateImage(ptrade->ok, (ptrade->x + 264), (ptrade->y + 277));

		UpdateImage(ptrade->cancel, (ptrade->x + 163), (ptrade->y + 277));

		UpdateImage(ptrade->myPenya, (ptrade->x + 38), (ptrade->y + 257));
		UpdateImage(ptrade->otherPenya, (ptrade->x + 241), (ptrade->y + 257));

		for(int i = 0; i < 20; i++)
		{
			if(ptrade->si[i].itemId != 0)
			{
				UpdateImage(ptrade->si[i].pic, ptrade->si[i].x, ptrade->si[i].y);

				if(ptrade->si[i].picStack != NULL && getItemStack(ptrade->si[i].itemId) > 1)
					UpdateImage(ptrade->si[i].picStack, (ptrade->si[i].x - 5), (((ptrade->si[i].y + ptrade->si[i].pic->h) - ptrade->si[i].picStack->h) + 5));
			}

			if(ptrade->si2[i].itemId != 0)
			{
				UpdateImage(ptrade->si2[i].pic, ptrade->si2[i].x, ptrade->si2[i].y);

				if(ptrade->si2[i].picStack != NULL && getItemStack(ptrade->si2[i].itemId) > 1)
					UpdateImage(ptrade->si2[i].picStack, (ptrade->si2[i].x - 5), (((ptrade->si2[i].y + ptrade->si2[i].pic->h) - ptrade->si2[i].picStack->h) + 5));
			}
		}
	}

	if(tp->enable)
	{
		UpdateImage(tp->win, tp->x, tp->y);
		UpdateImage(tp->quitx, (tp->x + (tp->win->w - tp->quitx->w)), tp->y);

		UpdateImage(tp->npcText, (tp->x + (131 - (tp->npcText->w / 2))), (tp->y + 13));

		for(unsigned int i = 0; i < tp->tps.size(); i++)
		{
			UpdateImage(tp->tps.at(i).world, (tp->x + tp->tps.at(i).x), (tp->y + tp->tps.at(i).y));
			UpdateImage(tp->tps.at(i).price, (tp->x + 158), (tp->y + tp->tps.at(i).y));
		}
	}

	if(fw->enable)
	{
		UpdateImage(fw->win, fw->x, fw->y);
		UpdateImage(fw->quitx, (fw->x + (fw->win->w - fw->quitx->w)), fw->y);

		int wrote = 0;
		int display = 0;

		for(unsigned int i = 0; i < fw->tps.size(); i++)
		{
			if(wrote >= 13)
				break;

			if((fw->startY + ((int)i * 16)) >= (fw->y + 65))
			{
				UpdateImage(fw->tps.at(i).namep, fw->x + 15, fw->y + 65 + display);
				UpdateImage(fw->tps.at(i).levelp, fw->x + fw->win->w - 52, fw->y + 65 + display);

				wrote++;
				display += 16;
			}
		}

		wrote = 0;
		display = 0;

		for(unsigned int i = 0; i < fw->tps.size(); i++)
		{
			if(wrote >= 13)
				break;

			if((fw->startY + ((int)i * 16)) >= (fw->y + 65))
			{
				if(fw->tps.at(i).showMenu)
				{
					int cX = fw->x + 15;
					int cY = fw->y + 65 + display + 16;
					
					int maxX = cX + fw->tps.at(i).menu.at(0).btn->w;
					int maxY = cY + (fw->tps.at(i).menu.size() * fw->tps.at(i).menu.at(0).btn->h + 1);

					if(maxX > resX)
					{
						int dif = maxX - resX;
						cX -= dif;
					}

					if(maxY > resY)
					{
						int dif = maxY - resY;
						cY -= dif;
					}

					for(unsigned int y = 0; y < fw->tps.at(i).menu.size(); y++)
					{
						UpdateImage(fw->tps.at(i).menu.at(y).btn, cX, cY);
						UpdateImage(fw->tps.at(i).menu.at(y).tSur, (cX + 8), (cY + 9));

						cY += (fw->tps.at(i).menu.at(y).btn->h + 1);
					}
				}

				wrote++;
				display += 16;
			}
		}

		if(fw->goUp)
			UpdateImage(fw->noteup, fw->x + 180, fw->y + 65);

		if(fw->goDown)
			UpdateImage(fw->notedown, fw->x + 180, fw->y + 260);
	}

	if(pms->pm.size() > 0)
	{
		for(int i = pms->pm.size() - 1; i >= 0; i--)
		{
			UpdateImage(pms->win, pms->pm.at(i).x, pms->pm.at(i).y);
			UpdateImage(pms->quitx, (pms->pm.at(i).x + (pms->win->w - pms->quitx->w)), pms->pm.at(i).y);
			UpdateImage(pms->pm.at(i).tonamep, pms->pm.at(i).x + 8, pms->pm.at(i).y + 5);

			int wrote = 0;
			int display = 0;

			for(unsigned int y = 0; y < pms->pm.at(i).wrotep.size(); y++)
			{
				if(wrote >= 10)
					break;

				if((pms->pm.at(i).startY + ((int)y * 14)) >= (pms->pm.at(i).y + 30))
				{
					UpdateImage(pms->pm.at(i).wrotep.at(y), pms->pm.at(i).x + 8, pms->pm.at(i).y + 30 + display);

					wrote++;
					display += 14;
				}
			}

			if(pms->pm.at(i).text.length() > 0)
				UpdateImage(pms->pm.at(i).textp, (pms->pm.at(i).x + 8), (pms->pm.at(i).y + 184));

			if((chatMessage && pms->GetMarked() == &pms->pm.at(i)))
			{
				UpdateImage(chatIcon.ani.at((chatIcon.frame - 1)), (pms->pm.at(i).x + 8 + pms->pm.at(i).chatPlaceW), (pms->pm.at(i).y + 184));
			}

			if(pms->pm.at(i).goUp)
				UpdateImage(fw->noteup, pms->pm.at(i).x + 269, pms->pm.at(i).y + 28);

			if(pms->pm.at(i).goDown)
				UpdateImage(fw->notedown, pms->pm.at(i).x + 269, pms->pm.at(i).y + 163);
		}
	}

	if(iw.size() > 0)
	{
		for(unsigned int i = 0; i < iw.size(); i++)
		{
			UpdateImage(iw.at(i)->win, iw.at(i)->x, iw.at(i)->y);
			UpdateImage(iw.at(i)->quitx, (iw.at(i)->x + (iw.at(i)->win->w - iw.at(i)->quitx->w)), iw.at(i)->y);

			UpdateImage(iw.at(i)->desc1, (iw.at(i)->x + 14), (iw.at(i)->y + 20));
			UpdateImage(iw.at(i)->desc2, (iw.at(i)->x + 14), (iw.at(i)->y + 44));

			UpdateImage(iw.at(i)->btn1, (iw.at(i)->x + (54 - (iw.at(i)->btn1->w / 2))), (iw.at(i)->y + (96 - (iw.at(i)->btn1->h / 2))));
			UpdateImage(iw.at(i)->btn2, (iw.at(i)->x + (163 - (iw.at(i)->btn1->w / 2))), (iw.at(i)->y + (96 - (iw.at(i)->btn1->h / 2))));
			UpdateImage(iw.at(i)->btn3, (iw.at(i)->x + (193 - (iw.at(i)->btn3->w / 2))), (iw.at(i)->y + (28 - (iw.at(i)->btn3->w / 2))));

			if(iw.at(i)->btn4 != NULL && getItemStack(iw.at(i)->arg3) > 1)
				UpdateImage(iw.at(i)->btn4, ((iw.at(i)->x + (193 - (iw.at(i)->btn3->w / 2))) - 5), ((((iw.at(i)->y + (28 - (iw.at(i)->btn3->w / 2))) + iw.at(i)->btn3->h) - iw.at(i)->btn4->h) + 5));

			UpdateImage(iw.at(i)->input, (iw.at(i)->x + 14), (iw.at(i)->y + 64));
		}
	}

	if(chatInputWindow && iw.size() > 0)
	{
		if(iw.back()->inputstr.length() > 0)
			UpdateImage(chatIcon.ani.at((chatIcon.frame - 1)), ((iw.back()->x + 14) + iw.back()->input->w + 1), (iw.back()->y + 63));

		else
			UpdateImage(chatIcon.ani.at((chatIcon.frame - 1)), (iw.back()->x + 15), (iw.back()->y + 63));
	}

	if(task->enableTask)
	{
		UpdateImage(task->bar, task->x, task->y);

		for(int i = 0; i < 9; i++)
		{
			if(task->pic[i]->enable)
			{
				if(task->pic[i] != NULL)
				{
					if(task->pic[i]->cooldsec > 0)
					{
						SDL_SetAlpha(task->pic[i]->item, SDL_SRCALPHA | SDL_RLEACCEL, 128);
						UpdateImage(task->pic[i]->item, task->pic[i]->itemX, task->pic[i]->itemY);
						SDL_SetAlpha(task->pic[i]->item, SDL_SRCALPHA, 255);
					}

					else
						UpdateImage(task->pic[i]->item, task->pic[i]->itemX, task->pic[i]->itemY);

					if(task->pic[i]->type == INVENTORYITEM)
					{
						if(!task->pic[i]->eqinv)
						{
							if(task->pic[i]->stack != NULL && getItemStack(task->pic[i]->itemid) > 1)
								UpdateImage(task->pic[i]->stack, (task->pic[i]->itemX - 5), (((task->pic[i]->itemY + task->pic[i]->item->h) - task->pic[i]->stack->h) + 5));
						}
					}

					if(task->pic[i]->cooldsec > 0)
					{
						UpdateImage(task->pic[i]->coold, task->pic[i]->itemX, task->pic[i]->itemY);
					}
				}
			}
		}
	}

	if(task->enableMenu && task->enableTask)
		UpdateImage(task->menu, ((task->x + task->bar->w) - task->menu->w), (task->y - task->menu->h));

	if(ow->enable)
	{
		int nowX = ow->x;
		int nowY = (ow->y - ow->start->h - ow->stop->h - (ow->bar->h * ow->nBars));

		if((nowX + ow->start->w) > screen->w)
			nowX -= ((nowX + ow->start->w) - screen->w);

		if(nowY < 0)
			nowY = 39;

		int stopY = (nowY + ow->start->h);

		UpdateImage(ow->start, nowX, nowY);

		for(int i = 0; i < ow->nBars; i++)
		{
			UpdateImage(ow->bar, nowX, stopY);

			stopY += ow->bar->h;
		}

		UpdateImage(ow->stop, nowX, stopY);

		for(unsigned int i = 0; i < ow->text.size(); i++)
			UpdateImage(ow->text.at(i).s, (nowX + 10), (nowY + 10 + ((ow->text.at(i).i - 1) * ow->bar->h)));
	}

	if(showFps)
	{
		if(fpsSurface != NULL)
			UpdateImage(fpsSurface, 1, (screen->h - fpsSurface->h - 18));

		if(pingSurface != NULL)
			UpdateImage(pingSurface, 1, (screen->h - pingSurface->h - 1));
	}

	if(drag)
		UpdateImage(dragged.pic, dragged.x, dragged.y);

	SDL_Flip(screen);
}

void UpdateLoadingImage()
{
	SDL_FillRect(screen, NULL, 0x00000000);

	UpdateImage(loadingSurface, 1, (screen->h - loadingSurface->h - 1));

	SDL_Flip(screen);
}

void UpdateLogin()
{
	UpdateImage(loginScreen, 0, 0);

	switch(logplace)
	{
		case LOGINSCREEN:
			{
				UpdateImage(loginFirst, ((screen->w / 2) - (loginFirst->w / 2)), ((screen->h / 2) - (loginFirst->h / 2)));
				UpdateImage(loginRem, (((screen->w / 2) - (loginFirst->w / 2)) + 42), (((screen->h / 2) - (loginFirst->h / 2)) + 139));
			break;
			}

		case CHARSELECT:
			{
				int xslot = 0;
				int yslot = (((screen->h / 2) - (loginChar->h / 2)) + 35);

				UpdateImage(loginChar, ((screen->w / 2) - (loginChar->w / 2)), ((screen->h / 2) - (loginChar->h / 2)));

				for(unsigned int i = 0; i < charsel.size(); i++)
				{
					UpdateImage(charsel.at(i).deletes, ((((screen->w / 2) - (loginChar->w / 2)) + 95) + (i * 284)), (((screen->h / 2) - (loginChar->h / 2)) + 180));

					if(charsel.at(i).slot != 0)
					{
						UpdateImage(charsel.at(i).createHaha, ((((screen->w / 2) - (loginChar->w / 2)) + 95) + (i * 284)), (((screen->h / 2) - (loginChar->h / 2)) + 213));

						if(charsel.at(i).slot == 1)
							xslot = (((screen->w / 2) - (loginChar->w / 2)) + 41);

						else if(charsel.at(i).slot == 2)
							xslot = (((screen->w / 2) - (loginChar->w / 2)) + 326);

						else if(charsel.at(i).slot == 3)
							xslot = (((screen->w / 2) - (loginChar->w / 2)) + 613);

						UpdateImage(charsel.at(i).name, xslot, yslot);
						UpdateImage(charsel.at(i).level, xslot, (yslot + 31));
						UpdateImage(charsel.at(i).job, xslot, (yslot + 45));
						UpdateImage(charsel.at(i).str, xslot, (yslot + 73));
						UpdateImage(charsel.at(i).sta, xslot, (yslot + 87));
						UpdateImage(charsel.at(i).dex, xslot, (yslot + 101));
						UpdateImage(charsel.at(i).intt, xslot, (yslot + 115));
						//UpdateImage(charsel.at(i).title, xslot, (yslot + 101));
					}
				}

			break;
			}
	}

	if(linputs.size() > 0)
	{
		for(int i = (linputs.size() - 1); i >= 0; i--)
		{
			UpdateImage(linputs.at(i)->win, ((screen->w / 2) - (linputs.at(i)->win->w / 2)), ((screen->h / 2) - (linputs.at(i)->win->h / 2)));

			if(linputs.at(i)->t == CREATECHARACTER)
			{
				if(job == 1)
					UpdateImage(boxFull, ((screen->w / 2) - (linputs.at(i)->win->w / 2)) + 11, ((screen->h / 2) - (linputs.at(i)->win->h / 2)) + 98);

				else
					UpdateImage(boxEmpty, ((screen->w / 2) - (linputs.at(i)->win->w / 2)) + 11, ((screen->h / 2) - (linputs.at(i)->win->h / 2)) + 98);

				if(job == 2)
					UpdateImage(boxFull, ((screen->w / 2) - (linputs.at(i)->win->w / 2)) + 11, ((screen->h / 2) - (linputs.at(i)->win->h / 2)) + 129);

				else
					UpdateImage(boxEmpty, ((screen->w / 2) - (linputs.at(i)->win->w / 2)) + 11, ((screen->h / 2) - (linputs.at(i)->win->h / 2)) + 129);

				if(job == 3)
					UpdateImage(boxFull, ((screen->w / 2) - (linputs.at(i)->win->w / 2)) + 125, ((screen->h / 2) - (linputs.at(i)->win->h / 2)) + 98);

				else
					UpdateImage(boxEmpty, ((screen->w / 2) - (linputs.at(i)->win->w / 2)) + 125, ((screen->h / 2) - (linputs.at(i)->win->h / 2)) + 98);

				if(job == 4)
					UpdateImage(boxFull, ((screen->w / 2) - (linputs.at(i)->win->w / 2)) + 125, ((screen->h / 2) - (linputs.at(i)->win->h / 2)) + 129);

				else
					UpdateImage(boxEmpty, ((screen->w / 2) - (linputs.at(i)->win->w / 2)) + 125, ((screen->h / 2) - (linputs.at(i)->win->h / 2)) + 129);

			}
		}
	}

	if(write.enable)
	{
		int plus1x = write.GetWriterW(write.GetWriteFocus());
		int plus1y = write.GetWriterH(write.GetWriteFocus());

		UpdateImage(chatIcon.ani.at((chatIcon.frame - 1)), (1 + plus1x), plus1y);

		write.ShowStrings();
	}

	if(lmsgs.size() > 0)
	{ 
		for(int i = (lmsgs.size() - 1); i >= 0; i--)
		{
			UpdateImage(lmsgs.at(i)->win, lmsgs.at(i)->x, lmsgs.at(i)->y);
			UpdateImage(lmsgs.at(i)->names, (lmsgs.at(i)->x + ((lmsgs.at(i)->win->w / 2) - (lmsgs.at(i)->names->w / 2))), (lmsgs.at(i)->y + 22));
		}
	}

	if(msgs.size() > 0)
	{
		for(unsigned int i = 0; i < msgs.size(); i++)
		{
			UpdateImage(msgs.at(i)->win, msgs.at(i)->x, msgs.at(i)->y);
			UpdateImage(msgs.at(i)->head, ((msgs.at(i)->x + ((msgs.at(i)->win->w / 2) - (msgs.at(i)->head->w / 2)))), (msgs.at(i)->y + 9));
			UpdateImage(msgs.at(i)->text, ((msgs.at(i)->x + ((msgs.at(i)->win->w / 2) - (msgs.at(i)->text->w / 2)))), (msgs.at(i)->y + 50));

			for(unsigned int y = 0; y < msgs.at(i)->btn.size(); y++)
				UpdateImage(msgs.at(i)->btn.at(y)->sur, msgs.at(i)->btn.at(y)->x, msgs.at(i)->btn.at(y)->y);
		}
	}

	if(showFps)
	{
		if(fpsSurface != NULL)
			UpdateImage(fpsSurface, 1, (screen->h - fpsSurface->h - 18));

		if(pingSurface != NULL)
			UpdateImage(pingSurface, 1, (screen->h - pingSurface->h - 1));
	}
  
	SDL_Flip(screen);
}
#include <iostream>
#include <SDL_TTF.h>
#include <SDL_image.h>
#include <math.h>
#include <sstream>
#include <time.h>
#include <fstream>

#include "Login.h"
#include "Packet.h"
#include "Packets.h"
#include "Misc.h"
#include "Animation.h"
#include "Process.h"
#include "Preload.h"
#include "Motion.h"
#include "Music.h"
#include "Main.h"
#include "Friend.h"

using namespace std;

extern SOCKET sock;
extern sockaddr_in serv;

extern int len;
extern int eAtk;
extern int moveVel;
extern int currentChatX;
extern int currentChatY;

extern unsigned char *pac;

extern SDL_Surface *screen;
extern SDL_Surface *fpsSurface;
extern SDL_Surface *pingSurface;

extern vector<Character*> cv;
extern vector<Monster*> cm;
extern vector<AddStatWindow*> addstatwindow;
extern vector<PartyWindow*> partyw;
extern vector<NPC*> npc;
extern vector<Bullet*> bullets;
extern vector<Notifer*> nv;
extern vector<PartyInvite*> pi;
extern vector<Window*> win;
extern vector<Icon*> icons;
extern vector<Drop*> drops;
extern vector<Damage> dmgs;
extern vector<Animation*> anis;
extern vector<Message*> msgs;
extern vector<InputWindow*> iw;
extern vector<CharacterSelect> charsel;
extern vector<Skill*> skill;

extern Character *me;

extern TTF_Font *cour;
extern TTF_Font *courMedium;
extern TTF_Font *courSmall;
extern TTF_Font *courBold;
extern TTF_Font *courNotice;
extern SDL_Color clrFg;
extern SDL_Color clrFgBlack;
extern SDL_Color clrFgRed;
extern SDL_Color clrFgBlue;
extern SDL_Color clrFgGreen;
extern SDL_Color clrFgCyan;

extern bool done;
extern bool PKENABLE;
extern bool spawned;
extern bool colorkey;
extern bool showFps;
extern bool showProcent;
extern bool chatInputWindow;

extern string mLoad;

extern Mark *mark;
extern Inventory *inv;
extern Taskbar *task;
extern ShopWindow *sw;
extern PlayerTrade *ptrade;
extern TeleportWindow *tp;
extern FriendWindow *fw;
extern PrivateMessage *pms;
extern ObjectInfoWindow *ow;
extern ChatManager *chatFunc;

extern float currBackY;
extern float currBackX;

extern float cameraX;
extern float cameraY;

extern int camShowX;
extern int camShowToX;
extern int camShowY;
extern int camShowToY;

extern int FPS;

extern int skillLoaded;

extern bool kördö;

extern bool initRecv;

extern bool chatting;
extern bool chatWindow;
extern bool chatMessage;

extern Animation chatIcon;

long dropDelay = 70;

int framess = 0;
bool normalCap = false;

long nextItem = 0;

Timer fpscheck = Timer();

vector<int> programex;

extern bool botActive;
extern int botTarget;
extern int botType;
extern int currentMark;

bool dc = false;

LARGE_INTEGER pingTimes;
int pingCount = 0;

extern int lastMouseX;
extern int lastMouseY;

long attackDelay = 306;

int dieTime = 10;

Map *map = new Map();

bool sentPingPacket = false;

extern int currMoveWay;

int playerLoad = 0;

extern SFXManager *sound;
extern LoginWriter write;

extern bool login;

std::vector<Tile*> preRender;

extern bool transWindow;

bool setChatIconShow = false;

int bonusAttackSpeed = 0;

int ProcessPacket(unsigned char *buf, int len, bool &move)
{
	char *cpak = (char*)buf;

	switch(cpak[2])
	{
		case 0:
			break;

		case 0x1: case 0x2: case 0x3: case 0x6: case 0x7:
			{
				pac = new unsigned char[len];
				memcpy(pac, buf, len);

				done = true;
			break;
			}

		case 0x5: 
			{
				if(cpak[3] == 1)
				{
					SDL_FreeSurface(fpsSurface);
					fpsSurface = RenderText_Solid(cour, "FPS: 0", clrFg);

					SDL_FreeSurface(pingSurface);
					pingSurface = RenderText_Solid(cour, "Latency: 0 ms", clrFg);

					int pos = 10;

					me->job = cpak[4];
					me->level = cpak[5];
					me->map = cpak[6];
					eAtk = cpak[7];
					me->speed = cpak[8];

					if(cpak[8] == 1)
						PKENABLE = true;
					else
						PKENABLE = false;

					login = false;

					for(unsigned int i = 0; i < charsel.size(); i++)
					{
						if(charsel.at(i).dex != NULL)
							SDL_FreeSurface(charsel.at(i).dex);

						charsel.at(i).dex = NULL;

						if(charsel.at(i).name != NULL)
							SDL_FreeSurface(charsel.at(i).name);

						charsel.at(i).name = NULL;

						if(charsel.at(i).sta != NULL)
							SDL_FreeSurface(charsel.at(i).sta);

						charsel.at(i).sta = NULL;

						if(charsel.at(i).str != NULL)
							SDL_FreeSurface(charsel.at(i).str);

						charsel.at(i).str = NULL;

						if(charsel.at(i).title != NULL)
							SDL_FreeSurface(charsel.at(i).title);

						charsel.at(i).title = NULL;

						if(charsel.at(i).level != NULL)
							SDL_FreeSurface(charsel.at(i).level);

						charsel.at(i).level = NULL;

						if(charsel.at(i).createHaha != NULL)
							SDL_FreeSurface(charsel.at(i).createHaha);

						charsel.at(i).createHaha = NULL;

						if(charsel.at(i).deletes != NULL)
							SDL_FreeSurface(charsel.at(i).deletes);

						charsel.at(i).deletes = NULL;

						if(charsel.at(i).job != NULL)
							SDL_FreeSurface(charsel.at(i).job);

						charsel.at(i).job = NULL;
					}

					charsel.clear();

					me->maxhp = atoi(getString(cpak, (pos + 1), (int)cpak[pos]).c_str());
					pos += ((int)cpak[pos] + 1);

					me->hp = atoi(getString(cpak, (pos + 1), (int)cpak[pos]).c_str());
					pos += ((int)cpak[pos] + 1);

					me->power = atoi(getString(cpak, (pos + 1), (int)cpak[pos]).c_str());
					pos += ((int)cpak[pos] + 1);

					me->hpPoint = atoi(getString(cpak, (pos + 1), (int)cpak[pos]).c_str());
					pos += ((int)cpak[pos] + 1);

					me->pAv = atoi(getString(cpak, (pos + 1), (int)cpak[pos]).c_str());
					pos += ((int)cpak[pos] + 1);

					me->agility = atoi(getString(cpak, (pos + 1), (int)cpak[pos]).c_str());
					pos += ((int)cpak[pos] + 1);

					me->intt = atoi(getString(cpak, (pos + 1), (int)cpak[pos]).c_str());
					pos += ((int)cpak[pos] + 1);

					strcpy_s(me->name, getString(cpak, (pos + 1), (int)cpak[pos]).c_str());
					pos += ((int)cpak[pos] + 1);

					me->id = atoi(getString(cpak, (pos + 1), (int)cpak[pos]).c_str());
					pos += ((int)cpak[pos] + 1);

					me->x = (float)atoi(getString(cpak, (pos + 1), (int)cpak[pos]).c_str());
					pos += ((int)cpak[pos] + 1);

					me->y = (float)atoi(getString(cpak, (pos + 1), (int)cpak[pos]).c_str());
					pos += ((int)cpak[pos] + 1);

					int notuse = atoi(getString(cpak, (pos + 1), (int)cpak[pos]).c_str());
					pos += ((int)cpak[pos] + 1);

					me->gold = atoi(getString(cpak, (pos + 1), (int)cpak[pos]).c_str());
					pos += ((int)cpak[pos] + 1);

					float myExps = (float)atof(getString(cpak, (pos + 1), (int)cpak[pos]).c_str());
					pos += ((int)cpak[pos] + 2);

					me->exp = myExps;

					char goldtemp[20] = "";

					SDL_FreeSurface(inv->golds);

					_itoa_s(me->gold, goldtemp, 20, 10);

					inv->golds = RenderText_Blended(courMedium, goldtemp, clrFgBlack);

					int items = cpak[(pos - 1)];

					me->nameSur.clean();
					CreateNameSurface(&me->nameSur, cour, clrFg, me->name, 1);

					me->pixSec = (me->speed * 62);

					char tempe[20] = "";
					char full[50] = "";

					char temp2[10] = "";

					if(addstatwindow.size() > 0)
					{
						_itoa_s(me->pAv, temp2, 10, 10);
						addstatwindow.at(0)->pointsav = RenderText_Solid(cour, temp2, clrFgBlack);

						_itoa_s(me->hpPoint, temp2, 10, 10);
						addstatwindow.at(0)->hp = RenderText_Solid(cour, temp2, clrFgBlack);

						_itoa_s(me->power, temp2, 10, 10);
						addstatwindow.at(0)->power = RenderText_Solid(cour, temp2, clrFgBlack);

						_itoa_s(me->agility, temp2, 10, 10);
						addstatwindow.at(0)->agility = RenderText_Solid(cour, temp2, clrFgBlack);

						_itoa_s(me->intt, temp2, 10, 10);
						addstatwindow.at(0)->intt = RenderText_Solid(cour, temp2, clrFgBlack);
					}

					for(unsigned int i = 0; i < win.size(); i++)
					{
						if(win.at(i)->TYPE == INFOWINDOW)
						{
							SDL_FreeSurface(win.at(i)->expSurface);

							sprintf_s(tempe, 20, "%2.2f", me->exp);

							strcpy_s(full, tempe);
							strcat_s(full, "%/100%");

							win.at(i)->expSurface = RenderText_Solid(cour, full, clrFgBlack);

							SDL_FreeSurface(win.at(i)->levelSurface);

							_itoa_s(me->level, tempe, 20, 10);

							win.at(i)->levelSurface = RenderText_Solid(cour, tempe, clrFgBlack);

							SDL_FreeSurface(win.at(i)->hpSurface);

							_itoa_s(me->hp, tempe, 20, 10);
							strcpy_s(full, tempe);
							strcat_s(full, "/");
							_itoa_s(me->maxhp, tempe, 20, 10);
							strcat_s(full, tempe);

							win.at(i)->hpSurface = RenderText_Solid(cour, full, clrFgBlack);

							break;
						}
					}

					for(int y = 0; y < items; y++)
					{
						try
						{
							int slot = cpak[pos];
							pos++;

							int val = atoi(getString(cpak, (pos + 1), (int)cpak[pos]).c_str());
							pos += ((int)cpak[pos] + 1);

							int effect = atoi(getString(cpak, (pos + 1), (int)cpak[pos]).c_str());
							pos += ((int)cpak[pos] + 1);

							int sta = atoi(getString(cpak, (pos + 1), (int)cpak[pos]).c_str());
							pos += ((int)cpak[pos] + 1);

							int maxstack = atoi(getString(cpak, (pos + 1), (int)cpak[pos]).c_str());
							pos += ((int)cpak[pos] + 1);

							int idz = atoi(getString(cpak, (pos + 1), (int)cpak[pos]).c_str());
							pos += ((int)cpak[pos] + 1);

							Item *it = new Item();

							it->slot = slot;
							it->value = val;
							it->stack = sta;
							it->maxstack = maxstack;
							it->effect = effect;

							char tempborr[10] = "";

							it->pic = getIconSurface(idz);
							if(it->pic == NULL)
								Log("Error, could not get surface from memory (Item: %d).", idz);

							it->id = idz;

							_itoa_s(sta, tempborr, 10, 10);

							it->amount = RenderText_Blended(courBold, tempborr, clrFgBlue);

							inv->items[(slot - 1)] = it;

							int rad = 1;

							int cX = (inv->x + 17);
							int cY = (inv->y + 284);

							for(int l = 0; l < 25; l++)
							{
								if((slot - 1) == l)
								{
									inv->items[l]->x = cX;
									inv->items[l]->y = cY;

									break;
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

						catch(...)
						{
							Log("Exception: Couldn't access data in the *cpak memory. Additional data: [POS: %d].", pos);
						}
					}

					int eqItems = cpak[pos];
					pos++;

					for(int i = 0; i < eqItems; i++)
					{
						int slot = cpak[pos];
						pos++;

						int idEq = atoi(getString(cpak, (pos + 1), (int)cpak[pos]).c_str());
						pos += ((int)cpak[pos] + 1);

						int value = atoi(getString(cpak, (pos + 1), (int)cpak[pos]).c_str());
						pos += ((int)cpak[pos] + 1);

						Item *e = new Item();

						e->slot = slot;
						e->stack = 1;
						e->maxstack = 1;
						e->value = value;

						e->pic = getIconSurface(idEq);
						if(e->pic == NULL)
							Log("Error, could not get surface from memory (Item: %d).", idEq);

						e->id = idEq;

						e->amount = RenderText_Blended(courBold, "1", clrFgBlue);

						inv->eq[(slot - 1)] = e;

						string o = "Data/Item/";
						char buf[10];

						_itoa_s(idEq, buf, 10, 10);

						switch(slot)
						{
							case 1: e->effect = 4001;
								break;

							case 2: e->effect = 4002;
								break;

							case 3: e->effect = 4003;
								break;

							case 4: e->effect = 4004;
								break;
						}

						switch(slot)
						{
							case 1:
								{
									o += "Weapon-";
									o += buf;
									o += ".png";

									me->weapon = loadimage(o.c_str(), true, 254, 255, 255);
								break;
								}

							case 2:
								{
									o += "Hat-";
									o += buf;
									o += ".png";

									me->hat = loadimage(o.c_str(), true, 254, 255, 255);
								break;
								}

							case 3:
								{
									o += "Suit-";
									o += buf;
									o += ".png";

									me->suit = loadimage(o.c_str(), true, 254, 255, 255);
								break;
								}

							case 4:
								{
									o += "Belt-";
									o += buf;
									o += ".png";

									me->belt = loadimage(o.c_str(), true, 254, 255, 255);
								break;
								}
						}
					}

					int buffs = atoi(getString(cpak, (pos + 1), (int)cpak[pos]).c_str());
					pos += ((int)cpak[pos] + 1);

					for(unsigned int t = 0; t < icons.size(); t++)
					{
						if(icons.at(t)->tSurface != NULL)
							SDL_FreeSurface(icons.at(t)->tSurface);

						delete icons.at(t);
					}

					icons.clear();

					for(int r = 0; r < buffs; r++)
					{
						int effect = atoi(getString(cpak, (pos + 1), (int)cpak[pos]).c_str());
						pos += ((int)cpak[pos] + 1);

						int changeEffect = atoi(getString(cpak, (pos + 1), (int)cpak[pos]).c_str());
						pos += ((int)cpak[pos] + 1);

						int value = atoi(getString(cpak, (pos + 1), (int)cpak[pos]).c_str());
						pos += ((int)cpak[pos] + 1);

						int timeLeft = atoi(getString(cpak, (pos + 1), (int)cpak[pos]).c_str());
						pos += ((int)cpak[pos] + 1);
						
						Icon *ic = new Icon();

						ic->icon = loadimage("Data/Icon/Buff-Icon.png", true, 255, 255, 255);
						ic->x = (166 + ((icons.size() * 42) + 1));
						ic->y = 0;
						ic->nextRemove = (getTime(true) + 1000);
						ic->timeLeft = (float)timeLeft;

						ic->timeLeftX = (ic->x + 5);
						ic->timeLeftY = (ic->y + 28);

						ic->name1X = (ic->x + 5);
						ic->name1Y = (ic->y + 3);
						
						ic->name2X = (ic->x + 5);
						ic->name2Y = (ic->name1Y + 10);

						ic->name1 = RenderText_Blended(courSmall, getBuffNameOne(effect).c_str(), clrFgBlack);
						ic->name2 = RenderText_Blended(courSmall, getBuffNameTwo(effect).c_str(), clrFgBlack);

						char tLeft[10] = "";
						_itoa_s(timeLeft, tLeft, 10, 10);

						ic->tSurface = RenderText_Blended(courSmall, tLeft, clrFgBlack);

						ic->lvl = value;
						ic->effect = effect;
						ic->changeEffect = changeEffect;

						icons.push_back(ic);
					}

					int stru = atoi(getString(cpak, (pos + 1), (int)cpak[pos]).c_str());
					pos += ((int)cpak[pos] + 1);

					int stau = atoi(getString(cpak, (pos + 1), (int)cpak[pos]).c_str());
					pos += ((int)cpak[pos] + 1);

					int agiu = atoi(getString(cpak, (pos + 1), (int)cpak[pos]).c_str());
					pos += ((int)cpak[pos] + 1);

					int intu = atoi(getString(cpak, (pos + 1), (int)cpak[pos]).c_str());
					pos += ((int)cpak[pos] + 1);

					char tempBuff[10] = "";

					for(unsigned int b = 0; b < icons.size(); b++)
					{
						if(icons.at(b)->changeEffect == STAMINA || stau > 0)
						{
							SDL_FreeSurface(addstatwindow.at(0)->hp);

							_itoa_s(me->hpPoint, tempBuff, 10, 10);

							addstatwindow.at(0)->hp = RenderText_Solid(cour, tempBuff, clrFgBlue);
						}

						if(icons.at(b)->changeEffect == STRENGTH || stru > 0)
						{
							SDL_FreeSurface(addstatwindow.at(0)->power);

							_itoa_s(me->power, tempBuff, 10, 10);

							addstatwindow.at(0)->power = RenderText_Solid(cour, tempBuff, clrFgBlue);
						}

						if(icons.at(b)->changeEffect == AGILITY || agiu > 0)
						{
							SDL_FreeSurface(addstatwindow.at(0)->agility);

							_itoa_s(me->agility, tempBuff, 10, 10);

							addstatwindow.at(0)->agility = RenderText_Solid(cour, tempBuff, clrFgBlue);
						}

						if(icons.at(b)->changeEffect == INTT || intu > 0)
						{
							SDL_FreeSurface(addstatwindow.at(0)->intt);

							_itoa_s(me->intt, tempBuff, 10, 10);

							addstatwindow.at(0)->intt = RenderText_Solid(cour, tempBuff, clrFgBlue);
						}
					}

					int tskItems = atoi(getString(cpak, (pos + 1), (int)cpak[pos]).c_str());
					pos += ((int)cpak[pos] + 1);

					for(int i = 0; i < tskItems; i++)
					{
						int tslot = cpak[pos];
						int ttype = cpak[(pos + 1)];
						int tpslot = cpak[(pos + 2)];
						int tEqInv = cpak[(pos + 3)];

						pos += 4;

						int itemid = atoi(getString(cpak, (pos + 1), (int)cpak[pos]).c_str());
						pos += ((int)cpak[pos] + 1);

						if(ttype == INVENTORYITEM)
						{
							if(tEqInv == 1)
							{
								task->pic[(tslot - 1)]->item = inv->eq[(tpslot - 1)]->pic;
								task->pic[(tslot - 1)]->stack = NULL;
								
								task->pic[(tslot - 1)]->itemid = inv->eq[(tpslot - 1)]->id;
							}

							else
							{
								if(inv->items[(tpslot - 1)] != NULL)
								{
									char tempssItem[20];
									_itoa_s(inv->items[(tpslot - 1)]->stack, tempssItem, 20, 10);

									task->pic[(tslot - 1)]->item = inv->items[(tpslot - 1)]->pic;

									if(getItemStack(inv->items[(tpslot - 1)]->id) > 1)
										task->pic[(tslot - 1)]->stack = getInventoryStackPic(inv->items[(tpslot - 1)]->id);

									else
										task->pic[(tslot - 1)]->stack = RenderText_Blended(courBold, tempssItem, clrFgBlue);

									task->pic[(tslot - 1)]->itemid = inv->items[(tpslot - 1)]->id;
								}

								else
								{
									task->pic[(tslot - 1)]->item = getIconSurface(itemid);

									if(getItemStack(itemid) > 1)
										task->pic[(tslot - 1)]->stack = getInventoryStackPic(itemid);

									else
										task->pic[(tslot - 1)]->stack = RenderText_Blended(courBold, "1", clrFgBlue);

									task->pic[(tslot - 1)]->itemid = itemid;
								}
							}

							task->pic[(tslot - 1)]->slot = tpslot;

							task->pic[(tslot - 1)]->itemX = ((task->pic[(tslot - 1)]->x + 20) - (task->pic[(tslot - 1)]->item->w / 2));
							task->pic[(tslot - 1)]->itemY = ((task->pic[(tslot - 1)]->y + 15) - (task->pic[(tslot - 1)]->item->h / 2));

							task->pic[(tslot - 1)]->type = INVENTORYITEM;

							if(tEqInv == 1)
								task->pic[(tslot - 1)]->eqinv = true;
							else
								task->pic[(tslot - 1)]->eqinv = false;
						}

						else
						{
							task->pic[(tslot - 1)]->slot = tpslot;
							task->pic[(tslot - 1)]->type = ttype;
						}

						task->pic[(tslot - 1)]->enable = true;
					}

					int skillss = cpak[pos];
					pos += 1;

					skill.clear();

					vector<int> gsi = getSkillIds();

					for(int i = 0; i < skillss; i++)
					{
						int lvlss = cpak[pos];
						int tyid = cpak[(pos + 1)];
						pos += 2;

						for(unsigned int y = 0; y < gsi.size(); y++)
						{
							if(gsi.at(y) == tyid)
							{
								Skill *skills2 = new Skill();

								CopySkillInfo(getSkill(gsi.at(y)), skills2);

								skill.push_back(skills2);

								break;
							}
						}

						SetSkillLevel(lvlss, tyid);
					}

					for(int i = 0; i < 9; i++)
					{
						if(task->pic[i]->enable && task->pic[i]->type != INVENTORYITEM)
						{
							task->pic[i]->item = getSkill(task->pic[i]->slot)->pic;//skills[(task->pic[i]->slot - 1)].pic;
							task->pic[i]->stack = NULL;

							task->pic[i]->itemX = ((task->pic[i]->x + 20) - (task->pic[i]->item->w / 2));
							task->pic[i]->itemY = ((task->pic[i]->y + 15) - (task->pic[i]->item->h / 2));
						}
					}

					/*
					skillLoaded = skillss;
					skills = new Skill[skillss];

					vector<int> gsi = getSkillIds();

					for(unsigned int i = 0; i < gsi.size(); i++)
					{
						CopySkillInfo(getSkill(gsi.at(i)), skills[(gsi.at(i) - 1)]);
					}

					for(int i = 0; i < skillss; i++)
					{
						int lvlss = cpak[pos];
						int tyid = cpak[(pos + 1)];
						pos += 2;

						SetSkillLevel(lvlss, tyid);
					}

					for(int i = 0; i < 9; i++)
					{
						if(task->pic[i]->enable && task->pic[i]->type != INVENTORYITEM)
						{
							task->pic[i]->item = skills[(task->pic[i]->slot - 1)].pic;
							task->pic[i]->stack = NULL;

							task->pic[i]->itemX = ((task->pic[i]->x + 20) - (task->pic[i]->item->w / 2));
							task->pic[i]->itemY = ((task->pic[i]->y + 15) - (task->pic[i]->item->h / 2));
						}
					}
					*/

					dropDelay = atoi(getString(cpak, (pos + 1), (int)cpak[pos]).c_str());
					pos += ((int)cpak[pos] + 1);

					int pk = atoi(getString(cpak, (pos + 1), (int)cpak[pos]).c_str());
					pos += ((int)cpak[pos] + 1);

					string channelname = getString(cpak, (pos + 1), (int)cpak[pos]);
					pos += ((int)cpak[pos] + 1);

					dieTime = atoi(getString(cpak, (pos + 1), (int)cpak[pos]).c_str());
					pos += ((int)cpak[pos] + 1);

					attackDelay = atoi(getString(cpak, (pos + 1), (int)cpak[pos]).c_str());
					pos += ((int)cpak[pos] + 1);

					bool weapEnable = false;

					if(cpak[pos] == 1)
						weapEnable = true;

					else
						weapEnable = false;

					pos++;

					int mapXSize = atoi(getString(cpak, (pos + 1), (int)cpak[pos]).c_str());
					pos += ((int)cpak[pos] + 1);

					int mapYSize = atoi(getString(cpak, (pos + 1), (int)cpak[pos]).c_str());
					pos += ((int)cpak[pos] + 1);

					map->pkArea.clear();
					map->tiles.clear();
					map->mapId = me->map;
					map->pkEnable = PKENABLE;
					map->weaponEnable = weapEnable;
					map->xSize = mapXSize;
					map->ySize = mapYSize;

					int numberOfAreas = atoi(getString(cpak, (pos + 1), (int)cpak[pos]).c_str());
					pos += ((int)cpak[pos] + 1);

					for(int noa = 0; noa < numberOfAreas; noa++)
					{
						int minXA = atoi(getString(cpak, (pos + 1), (int)cpak[pos]).c_str());
						pos += ((int)cpak[pos] + 1);

						int maxXA = atoi(getString(cpak, (pos + 1), (int)cpak[pos]).c_str());
						pos += ((int)cpak[pos] + 1);

						int minYA = atoi(getString(cpak, (pos + 1), (int)cpak[pos]).c_str());
						pos += ((int)cpak[pos] + 1);

						int maxYA = atoi(getString(cpak, (pos + 1), (int)cpak[pos]).c_str());
						pos += ((int)cpak[pos] + 1);

						PKAREA pkArea;

						pkArea.xMin = minXA;
						pkArea.xMax = maxXA;
						pkArea.yMin = minYA;
						pkArea.yMax = maxYA;

						map->pkArea.push_back(pkArea);
					}

					LoadNewMap(me->map);

					currBackY = (cameraY - (int)me->y);
					currBackX = (cameraX - (int)me->x);

					if(currBackY > 0)
					{
						cameraY -= currBackY;
						currBackY -= currBackY;
					}

					else if((msize(MAPY) - screen->h) < (currBackY * (-1)))
					{
						int tooMuch = (int)((currBackY * (-1.0f)) - (msize(MAPY) - screen->h));

						currBackY += tooMuch;
						cameraY += tooMuch;
					}

					if(currBackX > 0)
					{
						cameraX -= currBackX;
						currBackX -= currBackX;
					}

					else if((msize(MAPX) - screen->w) < (currBackX * (-1)))
					{
						int tooMuch = (int)((currBackX * (-1.0f)) - (msize(MAPX) - screen->w));

						currBackX += tooMuch;
						cameraX += tooMuch;
					}

					camShowX = (int)(msize(MAPX) - (currBackX * (-1.0f)));
					camShowX = (msize(MAPX) - camShowX);
					camShowToX = (camShowX + screen->w);

					camShowY = (int)(msize(MAPY) - (currBackY * (-1.0f)));
					camShowY = (msize(MAPY) - camShowY);
					camShowToY = (camShowY + screen->h);

					spawned = true;

					int playersToLoad = atoi(getString(cpak, (pos + 1), (int)cpak[pos]).c_str());
					pos += ((int)cpak[pos] + 1);

					playerLoad = playersToLoad;

					char temp5[10] = "";

					if(addstatwindow.size() > 0)
					{
						_itoa_s(pk, temp5, 10, 10);
						addstatwindow.at(0)->pkpoints = RenderText_Solid(cour, temp5, clrFgBlack);

						addstatwindow.at(0)->servername = RenderText_Solid(cour, channelname.c_str(), clrFgBlack);
						addstatwindow.at(0)->names = RenderText_Solid(cour, me->name, clrFgBlack);
					}

					int friends = atoi(getString(cpak, (pos + 1), (int)cpak[pos]).c_str());
					pos += ((int)cpak[pos] + 1);

					fw->CleanNames();

					for(int i = 0; i < friends; i++)
					{
						string name = getString(cpak, (pos + 1), (int)cpak[pos]).c_str();
						pos += ((int)cpak[pos] + 1);

						int level = atoi(getString(cpak, (pos + 1), (int)cpak[pos]).c_str());
						pos += ((int)cpak[pos] + 1);

						int job = atoi(getString(cpak, (pos + 1), (int)cpak[pos]).c_str());
						pos += ((int)cpak[pos] + 1);

						int id = atoi(getString(cpak, (pos + 1), (int)cpak[pos]).c_str());
						pos += ((int)cpak[pos] + 1);

						int on = cpak[pos];
						pos++;

						bool stats = false;

						if(on == 1)
							stats = true;

						else
							stats = false;

						Friend fri = Friend();

						fri.name = name;
						fri.level = level;
						fri.job = job;
						fri.id = id;
						fri.status = stats;

						fri.namep = RenderText_Solid(courMedium, name.c_str(), clrFgBlack);
						fri.levelp = RenderText_Solid(courMedium, IntToString(level).c_str(), clrFgBlack);

						if(!fri.status)
						{
							SDL_SetAlpha(fri.namep, SDL_SRCALPHA | SDL_RLEACCEL, 128);
							SDL_SetAlpha(fri.levelp, SDL_SRCALPHA | SDL_RLEACCEL, 128);
						}

						MenuButton mRemove = MenuButton();
						MenuButton mParty = MenuButton();
						MenuButton mMessage = MenuButton();

						strcpy_s(mMessage.text, "Send message");
						strcpy_s(mParty.text, "Invite to party");
						strcpy_s(mRemove.text, "Remove friend");

						mParty.tSur = RenderText_Solid(courMedium, mParty.text, clrFgBlack);
						mParty.btn = loadimage("Data/Interface/Menu-Window.png", true, 255, 255, 255);

						mRemove.tSur = RenderText_Solid(courMedium, mRemove.text, clrFgBlack);
						mRemove.btn = loadimage("Data/Interface/Menu-Window.png", true, 255, 255, 255);

						mMessage.tSur = RenderText_Solid(courMedium, mMessage.text, clrFgBlack);
						mMessage.btn = loadimage("Data/Interface/Menu-Window.png", true, 255, 255, 255);

						fri.menu.push_back(mMessage);
						fri.menu.push_back(mParty);
						fri.menu.push_back(mRemove);

						fw->tps.push_back(fri);
					}

					fw->UpdateNames();
					fw->startY = fw->y + 65;

					for(unsigned int pm = 0; pm < pms->pm.size(); pm++)
						pms->pm.at(pm).Clean();

					pms->pm.clear();

					me->mana = atoi(getString(cpak, (pos + 1), (int)cpak[pos]).c_str());
					pos += ((int)cpak[pos] + 1);

					me->maxmana = atoi(getString(cpak, (pos + 1), (int)cpak[pos]).c_str());
					pos += ((int)cpak[pos] + 1);

					me->energy = atoi(getString(cpak, (pos + 1), (int)cpak[pos]).c_str());
					pos += ((int)cpak[pos] + 1);

					me->maxenergy = atoi(getString(cpak, (pos + 1), (int)cpak[pos]).c_str());
					pos += ((int)cpak[pos] + 1);

					me->job = cpak[pos];
					pos++;

					bonusAttackSpeed = atoi(getString(cpak, (pos + 1), (int)cpak[pos]).c_str());
					pos += ((int)cpak[pos] + 1);

					_itoa_s(me->mana, tempe, 20, 10);
					strcpy_s(full, tempe);
					strcat_s(full, "/");
					_itoa_s(me->maxmana, tempe, 20, 10);
					strcat_s(full, tempe);

					me->manaSurface = RenderText_Solid(cour, full, clrFgBlack);

					_itoa_s(me->energy, tempe, 20, 10);
					strcpy_s(full, tempe);
					strcat_s(full, "/");
					_itoa_s(me->maxenergy, tempe, 20, 10);
					strcat_s(full, tempe);

					me->energySurface = RenderText_Solid(cour, full, clrFgBlack);

					SDL_FreeSurface(me->deadImg);
					me->Load(me->job);
				}

				else if(cpak[3] == 3)
				{
					int pos = 4;

					int mobs = atoi(getString(cpak, 5, (int)cpak[4]).c_str());
					pos += ((int)cpak[pos] + 1);

					for(int i = 0; i < mobs; i++)
					{
						Monster *m = new Monster();

						m->level = cpak[pos];
						m->attack = cpak[(pos + 1)];
						m->id = cpak[(pos + 2)];

						pos += 5;

						m->x = (float)atoi(getString(cpak, (pos + 1), (int)cpak[pos]).c_str());
						pos += ((int)cpak[pos] + 1);

						m->y = (float)atoi(getString(cpak, (pos + 1), (int)cpak[pos]).c_str());
						pos += ((int)cpak[pos] + 1);

						m->mId = atoi(getString(cpak, (pos + 1), (int)cpak[pos]).c_str());
						pos += ((int)cpak[pos] + 1);

						m->followId = atoi(getString(cpak, (pos + 1), (int)cpak[pos]).c_str());
						pos += ((int)cpak[pos] + 2);

						bool after = false;

						if(cpak[(pos - 1)] == 1)
							after = true;

						int xx = atoi(getString(cpak, (pos + 1), (int)cpak[pos]).c_str());
						pos += ((int)cpak[pos] + 1);

						int yy = atoi(getString(cpak, (pos + 1), (int)cpak[pos]).c_str());
						pos += ((int)cpak[pos] + 2);

						bool aggro = false;

						if(cpak[(pos - 1)] == 1)
							aggro = true;

						int speed = atoi(getString(cpak, (pos + 1), (int)cpak[pos]).c_str());
						pos += ((int)cpak[pos] + 1);

						if(cpak[pos] == 1)
						{
							m->moving = true;
							m->moveWay = cpak[(pos + 1)];
						}

						else
							m->moving = false;

						pos += 2;

						int mProc = cpak[pos];
						pos++;

						if((m->x >= camShowX && camShowToX >= m->x) && (m->y >=camShowY && camShowToY >= m->y))
						{
							int t = screen->w;
							int t2 = screen->h;

							m->cX = (camShowToX - (int)m->x);
							m->cY = (camShowToY - (int)m->y);

							t -= m->cX;
							t2 -= m->cY;

							m->cX = t;
							m->cY = t2;

							m->inWin = true;
						}

						else
						{
							if(m->inWin)
								m->inWin = false;
						}

						if(after || m->followId != 0)
						{
							m->beforeX = (float)xx;
							m->beforeY = (float)yy;
						}

						else
						{
							m->beforeX = m->x;
							m->beforeY = m->y;
						}

						m->speed = speed;
						m->proc = mProc;
						m->fades = true;
						m->fadeway = true;

						m->Load();

						char tempssMob[20];
						string fullMonsterName = getMonsterName(m->id);

						fullMonsterName += " Lvl. ";
						_itoa_s(m->level, tempssMob, 20, 10);
						fullMonsterName += tempssMob;

						if(!aggro)
						{
							m->nameSurface = RenderText_Solid(cour, getMonsterName(m->id).c_str(), clrFg);
							m->whenMarkName = RenderText_Solid(cour, fullMonsterName.c_str(), clrFg);
						}

						else
						{
							m->nameSurface = RenderText_Solid(cour, getMonsterName(m->id).c_str(), clrFgRed);
							m->whenMarkName = RenderText_Solid(cour, fullMonsterName.c_str(), clrFgRed);
						}

						cm.push_back(m);
					}
				}

				else if(cpak[3] == 10)
				{
					int pos = 4;

					int npcs = atoi(getString(cpak, (pos + 1), (int)cpak[pos]).c_str());
					pos += ((int)cpak[pos] + 1);

					for(int i = 0; i < npcs; i++)
					{
						NPC *np = new NPC();

						np->x = atoi(getString(cpak, (pos + 1), (int)cpak[pos]).c_str());
						pos += ((int)cpak[pos] + 1);

						np->y = atoi(getString(cpak, (pos + 1), (int)cpak[pos]).c_str());
						pos += ((int)cpak[pos] + 1);

						np->id = atoi(getString(cpak, (pos + 1), (int)cpak[pos]).c_str());
						pos += ((int)cpak[pos] + 1);

						strcpy_s(np->name, getString(cpak, (pos + 1), (int)cpak[pos]).c_str());
						pos += ((int)cpak[pos] + 2);

						int cur = cpak[(pos - 1)];

						for(int y = 0; y < cur; y++)
						{
							MenuButton m = MenuButton();

							strcpy_s(m.text, getString(cpak, (pos + 1), (int)cpak[pos]).c_str());
							pos += ((int)cpak[pos] + 1);

							m.tSur = RenderText_Solid(courMedium, m.text, clrFgBlack);
							m.btn = loadimage("Data/Interface/Menu-Window.png", true, 255, 255, 255);

							np->menu.push_back(m);
						}

						np->dialogWindow = loadimage("Data/Interface/NPCDialog-Window.png", false, 0, 0, 0);
						np->quitx = loadimage("Data/Interface/Quit-Button.png", true, 255, 255, 255);
						np->dialogWindowName = RenderText_Solid(courMedium, np->name, clrFgBlack);

						if(transWindow)
							SDL_SetAlpha(np->dialogWindow, SDL_SRCALPHA | SDL_RLEACCEL, 128);

						np->dialogX = 200;
						np->dialogY = 50;

						np->showMenu = false;
						np->showDialog = false;

						if((np->x >= camShowX && camShowToX >= np->x) && (np->y >=camShowY && camShowToY >= np->y))
						{
							int t = screen->w;
							int t2 = screen->h;

							np->cX = (camShowToX - np->x);
							np->cY = (camShowToY - np->y);

							t -= np->cX;
							t2 -= np->cY;

							np->cX = t;
							np->cY = t2;

							np->inWin = true;
						}

						else
						{
							if(np->inWin)
								np->inWin = false;
						}

						np->Load();

						np->nameSurface = RenderText_Solid(cour, np->name, clrFg);

						npc.push_back(np);
					}
				}

				else
				{
					int pos = 5;

					for(int i = 0; i < cpak[4]; i++)
					{
						Character *t = new Character();

						t->level = cpak[pos];
						t->power = cpak[(pos + 1)];
						t->speed = cpak[(pos + 2)];

						if(cpak[(pos + 3)] == 1)
							t->equipped = true;
						else
							t->equipped = false;

						if(cpak[(pos + 4)] == 1)
							t->dead = true;
						else
							t->dead = false;

						t->wMove = cpak[(pos + 5)];

						pos += 6;

						strcpy_s(t->name, getString(cpak, (pos + 1), (int)cpak[pos]).c_str());
						pos += ((int)cpak[pos] + 1);

						t->id = atoi(getString(cpak, (pos + 1), (int)cpak[pos]).c_str());
						pos += ((int)cpak[pos] + 1);

						if(cpak[pos] == 1)
						{
							t->inParty = true;

							t->nameSur.clean();
							CreateNameSurface(&t->nameSur, cour, clrFgRed, t->name, 1);
						}

						else
						{
							t->inParty = false;

							t->nameSur.clean();
							CreateNameSurface(&t->nameSur, cour, clrFg, t->name, 1);
						}

						pos++;

						t->x = (float)atoi(getString(cpak, (pos + 1), (int)cpak[pos]).c_str());
						pos += ((int)cpak[pos] + 1);

						t->y = (float)atoi(getString(cpak, (pos + 1), (int)cpak[pos]).c_str());
						pos += ((int)cpak[pos] + 1);

						t->pixSec = (t->speed * 62);

						if(cpak[pos] == 1)
						{
							t->cMove = (int)cpak[(pos + 1)];
							t->moving = true;

							t->move.start(false);
						}

						else
							t->moving = false;

						pos += 2;

						int eqItems = cpak[pos];
						pos++;

						for(int i = 0; i < eqItems; i++)
						{
							int slot = cpak[pos];
							pos++;

							int idEq = atoi(getString(cpak, (pos + 1), (int)cpak[pos]).c_str());
							pos += ((int)cpak[pos] + 1);

							string o = "Data/Item/";
							char buf[10];

							_itoa_s(idEq, buf, 10, 10);

							switch(slot)
							{
								case 1:
									{
										o += "Weapon-";
										o += buf;
										o += ".png";

										t->weapon = loadimage(o.c_str(), true, 254, 255, 255);
										t->weaponid = idEq;
									break;
									}

								case 2:
									{
										o += "Hat-";
										o += buf;
										o += ".png";

										t->hat = loadimage(o.c_str(), true, 254, 255, 255);
										t->hatid = idEq;
									break;
									}

								case 3:
									{
										o += "Suit-";
										o += buf;
										o += ".png";

										t->suit = loadimage(o.c_str(), true, 254, 255, 255);
										t->suitid = idEq;
									break;
									}

								case 4:
									{
										o += "Belt-";
										o += buf;
										o += ".png";

										t->belt = loadimage(o.c_str(), true, 254, 255, 255);
										t->beltid = idEq;
									break;
									}
							}
						}

						t->proc = cpak[pos];
						pos++;

						t->follow = getBool(cpak[pos]);
						pos++;

						t->followId = atoi(getString(cpak, (pos + 1), (int)cpak[pos]).c_str());
						pos += ((int)cpak[pos] + 1);

						t->job = cpak[pos];
						pos++;

						if(t->follow)
						{
							t->moving = false;
							t->cMove = false;

							t->move.start(false);
						}

						if((t->x >= camShowX && camShowToX >= t->x) && (t->y >=camShowY && camShowToY >= t->y))
						{
							int u = screen->w;
							int u2 = screen->h;

							t->cX = (camShowToX - (int)t->x);
							t->cY = (camShowToY - (int)t->y);

							u -= t->cX;
							u2 -= t->cY;

							t->cX = u;
							t->cY = u2;

							t->inWin = true;
						}

						else
						{
							if(t->inWin)
								t->inWin = false;
						}

						MenuButton mTrade = MenuButton();
						MenuButton mFollow = MenuButton();
						MenuButton mParty = MenuButton();
						MenuButton mAddFriend = MenuButton();

						strcpy_s(mTrade.text, "Trade");
						strcpy_s(mFollow.text, "Follow");
						strcpy_s(mParty.text, "Party");
						strcpy_s(mAddFriend.text, "Add as friend");

						mTrade.tSur = RenderText_Solid(courMedium, mTrade.text, clrFgBlack);
						mTrade.btn = loadimage("Data/Interface/Menu-Window.png", true, 255, 255, 255);

						mFollow.tSur = RenderText_Solid(courMedium, mFollow.text, clrFgBlack);
						mFollow.btn = loadimage("Data/Interface/Menu-Window.png", true, 255, 255, 255);

						mParty.tSur = RenderText_Solid(courMedium, mParty.text, clrFgBlack);
						mParty.btn = loadimage("Data/Interface/Menu-Window.png", true, 255, 255, 255);

						mAddFriend.tSur = RenderText_Solid(courMedium, mAddFriend.text, clrFgBlack);
						mAddFriend.btn = loadimage("Data/Interface/Menu-Window.png", true, 255, 255, 255);

						t->menu.push_back(mTrade);
						t->menu.push_back(mFollow);
						t->menu.push_back(mParty);
						t->menu.push_back(mAddFriend);

						t->Load(t->job);

						cv.push_back(t);

						if(playerLoad > 0)
							playerLoad--;
					}
				}
			break;
			}

		case 0x8:
			{
				int pos = 5;

				int id = atoi(getString(cpak, (pos + 1), (int)cpak[pos]).c_str());
				pos += ((int)cpak[pos] + 1);

				int x = atoi(getString(cpak, (pos + 1), (int)cpak[pos]).c_str());
				pos += ((int)cpak[pos] + 1);

				int y = atoi(getString(cpak, (pos + 1), (int)cpak[pos]).c_str());

				if(cpak[4] == 1)
				{
					for(unsigned int i = 0; i < cv.size(); i++)
					{
						if(cv.at(i)->id == id)
						{
							cv.at(i)->x = (float)x;
							cv.at(i)->y = (float)y;

							cv.at(i)->cMove = cpak[3];

							cv.at(i)->moving = true;

							if(cv.at(i)->follow)
							{
								cv.at(i)->follow = false;
								cv.at(i)->followId = 0;
							}

							cv.at(i)->move.start(false);

							switch(cv.at(i)->cMove)
							{
								case LEFT:
									{
										if(cv.at(i)->wMove != LEFT)
											cv.at(i)->wMove = LEFT;
									break;
									}

								case RIGHT:
									{
										if(cv.at(i)->wMove != RIGHT)
											cv.at(i)->wMove = RIGHT;
									break;
									}
							}

							if((cv.at(i)->x >= camShowX && camShowToX >= cv.at(i)->x) && (cv.at(i)->y >=camShowY && camShowToY >= cv.at(i)->y))
							{
								int t = screen->w;
								int t2 = screen->h;

								cv.at(i)->cX = (camShowToX - (int)cv.at(i)->x);
								cv.at(i)->cY = (camShowToY - (int)cv.at(i)->y);

								t -= cv.at(i)->cX;
								t2 -= cv.at(i)->cY;

								cv.at(i)->cX = t;
								cv.at(i)->cY = t2;

								cv.at(i)->inWin = true;
							}

							else
							{
								if(cv.at(i)->inWin)
									cv.at(i)->inWin = false;
							}

							break;
						}
					}
				}

				else
				{
					for(unsigned int i = 0; i < cm.size(); i++)
					{
						if(cm.at(i)->mId == id)
						{
							cm.at(i)->x = (float)x;
							cm.at(i)->y = (float)y;
							cm.at(i)->beforeX = (float)x;
							cm.at(i)->beforeY = (float)y;

							cm.at(i)->moveWay = cpak[3];

							cm.at(i)->moving = true;

							cm.at(i)->move.start(true);

							if((cm.at(i)->x >= camShowX && camShowToX >= cm.at(i)->x) && (cm.at(i)->y >=camShowY && camShowToY >= cm.at(i)->y))
							{
								int t = screen->w;
								int t2 = screen->h;

								cm.at(i)->cX = (camShowToX - (int)cm.at(i)->x);
								cm.at(i)->cY = (camShowToY - (int)cm.at(i)->y);

								t -= cm.at(i)->cX;
								t2 -= cm.at(i)->cY;

								cm.at(i)->cX = t;
								cm.at(i)->cY = t2;

								cm.at(i)->inWin = true;
							}

							else
							{
								if(cm.at(i)->inWin)
									cm.at(i)->inWin = false;
							}

							break;
						}
					}
				}
			break;
			}

		case 0x9:
			{
				int pos = 4;

				int id = atoi(getString(cpak, (pos + 1), (int)cpak[pos]).c_str());
				pos += ((int)cpak[pos] + 1);

				int x = atoi(getString(cpak, (pos + 1), (int)cpak[pos]).c_str());
				pos += ((int)cpak[pos] + 1);

				int y = atoi(getString(cpak, (pos + 1), (int)cpak[pos]).c_str());

				if(cpak[3] == 1)
				{
					for(unsigned int i = 0; i < cv.size(); i++)
					{
						if(cv.at(i)->id == id)
						{
							cv.at(i)->moving = false;

							cv.at(i)->x = (float)x;
							cv.at(i)->y = (float)y;

							if(cv.at(i)->follow)
							{
								cv.at(i)->follow = false;
								cv.at(i)->followId = 0;
							}

							if((cv.at(i)->x >= camShowX && camShowToX >= cv.at(i)->x) && (cv.at(i)->y >=camShowY && camShowToY >= cv.at(i)->y))
							{
								int t = screen->w;
								int t2 = screen->h;

								cv.at(i)->cX = (camShowToX - (int)cv.at(i)->x);
								cv.at(i)->cY = (camShowToY - (int)cv.at(i)->y);

								t -= cv.at(i)->cX;
								t2 -= cv.at(i)->cY;

								cv.at(i)->cX = t;
								cv.at(i)->cY = t2;

								cv.at(i)->inWin = true;
							}

							else
							{
								if(cv.at(i)->inWin)
									cv.at(i)->inWin = false;
							}

							break;
						}
					}
				}

				else
				{
					for(unsigned int i = 0; i < cm.size(); i++)
					{
						if(cm.at(i)->mId == id)
						{
							cm.at(i)->moving = false;

							cm.at(i)->x = (float)x;
							cm.at(i)->y = (float)y;
							cm.at(i)->beforeX = (float)x;
							cm.at(i)->beforeY = (float)y;

							if(((cm.at(i)->x + cm.at(i)->m->w) >= camShowX && camShowToX >= cm.at(i)->x) && (cm.at(i)->y >=camShowY && camShowToY >= cm.at(i)->y))
							{
								int t = screen->w;
								int t2 = screen->h;

								cm.at(i)->cX = (camShowToX - (int)cm.at(i)->x);
								cm.at(i)->cY = (camShowToY - (int)cm.at(i)->y);

								t -= cm.at(i)->cX;
								t2 -= cm.at(i)->cY;

								cm.at(i)->cX = t;
								cm.at(i)->cY = t2;

								cm.at(i)->inWin = true;
							}

							else
							{
								if(cm.at(i)->inWin)
									cm.at(i)->inWin = false;
							}

							break;
						}
					}
				}
			break;
			}

		case 0x10:
			{
				if(cpak[3] > 0)
				{
					int id = 0;

					try
					{
						me->map = cpak[3];
					}

					catch(...)
					{
						Log("Exception caught at PROCESS ::case 0x10: %d", id);
					}

					if(cpak[4] == 1)
						PKENABLE = true;
					else
						PKENABLE = false;

					bool weapEnable = true;

					if(cpak[5] == 0)
					{
						me->equipped = false;
						weapEnable = false;
					}

					if(move)
						move = false;

					if(botActive)
						botActive = false;

					tp->Clean();
					tp->enable = false;

					sw->Clean();
					sw->enable = false;

					ptrade->Clean();
					ptrade->enable = false;

					botTarget = 0;
					botType = 0;
					currentMark = 0;

					showProcent = false;

					int pos = 6;

					int xPos = atoi(getString(cpak, (pos + 1), (int)cpak[pos]).c_str());
					pos += ((int)cpak[pos] + 1);

					int yPos = atoi(getString(cpak, (pos + 1), (int)cpak[pos]).c_str());
					pos += ((int)cpak[pos] + 1);

					int xSize = atoi(getString(cpak, (pos + 1), (int)cpak[pos]).c_str());
					pos += ((int)cpak[pos] + 1);

					int ySize = atoi(getString(cpak, (pos + 1), (int)cpak[pos]).c_str());
					pos += ((int)cpak[pos] + 1);

					me->x = (float)xPos;
					me->y = (float)yPos;

					delete map;
					map = new Map();

					map->mapId = me->map;
					map->pkEnable = PKENABLE;
					map->weaponEnable = weapEnable;
					map->xSize = xSize;
					map->ySize = ySize;

					int numberOfAreas = atoi(getString(cpak, (pos + 1), (int)cpak[pos]).c_str());
					pos += ((int)cpak[pos] + 1);

					for(int noa = 0; noa < numberOfAreas; noa++)
					{
						int minXA = atoi(getString(cpak, (pos + 1), (int)cpak[pos]).c_str());
						pos += ((int)cpak[pos] + 1);

						int maxXA = atoi(getString(cpak, (pos + 1), (int)cpak[pos]).c_str());
						pos += ((int)cpak[pos] + 1);

						int minYA = atoi(getString(cpak, (pos + 1), (int)cpak[pos]).c_str());
						pos += ((int)cpak[pos] + 1);

						int maxYA = atoi(getString(cpak, (pos + 1), (int)cpak[pos]).c_str());
						pos += ((int)cpak[pos] + 1);

						PKAREA pkArea;

						pkArea.xMin = minXA;
						pkArea.xMax = maxXA;
						pkArea.yMin = minYA;
						pkArea.yMax = maxYA;

						map->pkArea.push_back(pkArea);
					}

					int playersToLoad = atoi(getString(cpak, (pos + 1), (int)cpak[pos]).c_str());
					pos += ((int)cpak[pos] + 1);

					playerLoad = playersToLoad;

					mark->enable = false;

					LoadNewMap(cpak[3]);

					cameraY = (float)(screen->h / 2);
					cameraX = (float)(screen->w / 2);

					currBackY = (cameraY - (int)me->y);
					currBackX = (cameraX - (int)me->x);

					if(currBackY > 0)
					{
						cameraY -= currBackY;
						currBackY -= currBackY;
					}

					else if((msize(MAPY) - screen->h) < (currBackY * (-1)))
					{
						int tooMuch = (int)((currBackY * (-1.0f)) - (msize(MAPY) - screen->h));

						currBackY += tooMuch;
						cameraY += tooMuch;
					}

					if(currBackX > 0)
					{
						cameraX -= currBackX;
						currBackX -= currBackX;
					}

					else if((msize(MAPX) - screen->w) < (currBackX * (-1)))
					{
						int tooMuch = (int)((currBackX * (-1.0f)) - (msize(MAPX) - screen->w));

						currBackX += tooMuch;
						cameraX += tooMuch;
					}

					camShowX = (int)(msize(MAPX) - (currBackX * (-1.0f)));
					camShowX = (msize(MAPX) - camShowX);
					camShowToX = (camShowX + screen->w);

					camShowY = (int)(msize(MAPY) - (currBackY * (-1.0f)));
					camShowY = (msize(MAPY) - camShowY);
					camShowToY = (camShowY + screen->h);

					for(unsigned int i = 0; i < cv.size(); i++)
					{
						cv.at(i)->nameSur.clean();
						SDL_FreeSurface(cv.at(i)->deadImg);

						if(cv.at(i)->weapon != NULL)
							SDL_FreeSurface(cv.at(i)->weapon);

						if(cv.at(i)->hat != NULL)
							SDL_FreeSurface(cv.at(i)->hat);

						if(cv.at(i)->suit != NULL)
							SDL_FreeSurface(cv.at(i)->suit);

						if(cv.at(i)->belt != NULL)
							SDL_FreeSurface(cv.at(i)->belt);

						if(cv.at(i)->c != NULL)
						{
							SDL_FreeSurface(cv.at(i)->c->text1);
							SDL_FreeSurface(cv.at(i)->c->theWindow);

							if(cv.at(i)->c->text2 != NULL)
								SDL_FreeSurface(cv.at(i)->c->text2);

							delete cv.at(i)->c;
							cv.at(i)->c = NULL;
						}

						delete cv.at(i);
					}

					cv.clear();

					for(unsigned int i = 0; i < cm.size(); i++)
					{
						SDL_FreeSurface(cm.at(i)->nameSurface);
						SDL_FreeSurface(cm.at(i)->whenMarkName);

						delete cm.at(i);
					}

					cm.clear();

					for(unsigned int i = 0; i < npc.size(); i++)
					{
						SDL_FreeSurface(npc.at(i)->my);
						SDL_FreeSurface(npc.at(i)->nameSurface);

						SDL_FreeSurface(npc.at(i)->dialogWindow);
						SDL_FreeSurface(npc.at(i)->dialogWindowName);
						SDL_FreeSurface(npc.at(i)->quitx);

						if(npc.at(i)->npct.size() > 0)
						{
							for(unsigned int y = 0; y < npc.at(i)->npct.size(); y++)
								SDL_FreeSurface(npc.at(i)->npct.at(y).text);

							npc.at(i)->npct.clear();
						}

						if(npc.at(i)->diab.size() > 0)
						{
							for(unsigned int t = 0; t < npc.at(i)->diab.size(); t++)
							{
								SDL_FreeSurface(npc.at(i)->diab.at(t).btn);
								SDL_FreeSurface(npc.at(i)->diab.at(t).tSur);
							}

							npc.at(i)->diab.clear();
						}

						if(npc.at(i)->c != NULL)
						{
							SDL_FreeSurface(npc.at(i)->c->text1);
							SDL_FreeSurface(npc.at(i)->c->theWindow);

							if(npc.at(i)->c->text2 != NULL)
								SDL_FreeSurface(npc.at(i)->c->text2);

							delete npc.at(i)->c;
						}

						delete npc.at(i);
					}

					npc.clear();

					for(unsigned int i = 0; i < bullets.size(); i++)
						delete bullets.at(i);

					bullets.clear();

					for(unsigned int i = 0; i < drops.size(); i++)
						delete drops.at(i);

					drops.clear();

					for(unsigned int i = 0; i < dmgs.size(); i++)
					{
						SDL_FreeSurface(dmgs.at(i).dmg);

						if(dmgs.at(i).dmgItem != NULL)
							SDL_FreeSurface(dmgs.at(i).dmgItem);
					}

					dmgs.clear();

					for(unsigned int i = 0; i < anis.size(); i++)
					{
						if(anis.at(i)->deletePic)
						{
							for(unsigned int y = 0; y < anis.at(i)->ani.size(); y++)
								SDL_FreeSurface(anis.at(i)->ani.at(y));
						}

						delete anis.at(i);
					}

					anis.clear();
				}
			break;
			}

		case 0x11:
			{
				int pos = 3;
				int id = atoi(getString(cpak, (pos + 1), (int)cpak[pos]).c_str());
				pos += ((int)cpak[pos] + 1);

				float x = (float)atoi(getString(cpak, (pos + 1), (int)cpak[pos]).c_str());
				pos += ((int)cpak[pos] + 1);

				float y = (float)atoi(getString(cpak, (pos + 1), (int)cpak[pos]).c_str());

				if(id == me->id)
				{
					me->x = x;
					me->y = y;

					cameraY = (float)(screen->h / 2);
					cameraX = (float)(screen->w / 2);

					currBackY = (cameraY - (int)me->y);
					currBackX = (cameraX - (int)me->x);

					if(currBackY > 0)
					{
						cameraY -= currBackY;
						currBackY -= currBackY;
					}

					else if((msize(MAPY) - screen->h) < (currBackY * (-1)))
					{
						int tooMuch = (int)((currBackY * (-1.0f)) - (msize(MAPY) - screen->h));

						currBackY += tooMuch;
						cameraY += tooMuch;
					}

					if(currBackX > 0)
					{
						cameraX -= currBackX;
						currBackX -= currBackX;
					}

					else if((msize(MAPX) - screen->w) < (currBackX * (-1)))
					{
						int tooMuch = (int)((currBackX * (-1.0f)) - (msize(MAPX) - screen->w));

						currBackX += tooMuch;
						cameraX += tooMuch;
					}

					camShowX = (int)(msize(MAPX) - (currBackX * (-1.0f)));
					camShowX = (msize(MAPX) - camShowX);
					camShowToX = (camShowX + screen->w);

					camShowY = (int)(msize(MAPY) - (currBackY * (-1.0f)));
					camShowY = (msize(MAPY) - camShowY);
					camShowToY = (camShowY + screen->h);

					if(me->dead)
					{
						me->dead = false;

						me->hp = (me->maxhp / 2);
					}
				}

				else
				{
					for(unsigned int i = 0; i < cv.size(); i++)
					{
						if(id == cv.at(i)->id)
						{
							cv.at(i)->x = x;
							cv.at(i)->y = y;

							if((cv.at(i)->x >= camShowX && camShowToX >= cv.at(i)->x) && (cv.at(i)->y >=camShowY && camShowToY >= cv.at(i)->y))
							{
								int t = screen->w;
								int t2 = screen->h;

								cv.at(i)->cX = (camShowToX - (int)cv.at(i)->x);
								cv.at(i)->cY = (camShowToY - (int)cv.at(i)->y);

								t -= cv.at(i)->cX;
								t2 -= cv.at(i)->cY;

								cv.at(i)->cX = t;
								cv.at(i)->cY = t2;

								cv.at(i)->inWin = true;
							}

							else
							{
								if(cv.at(i)->inWin)
									cv.at(i)->inWin = false;
							}

							break;
						}
					}
				}			

			break;
			}

		case 0x12:
			{
				if(cpak[4] == 0)
				{
					for(unsigned int i = 0; i < cv.size(); i++)
					{
						cv.at(i)->nameSur.clean();
						SDL_FreeSurface(cv.at(i)->deadImg);

						if(cv.at(i)->weapon != NULL)
							SDL_FreeSurface(cv.at(i)->weapon);

						if(cv.at(i)->hat != NULL)
							SDL_FreeSurface(cv.at(i)->hat);

						if(cv.at(i)->suit != NULL)
							SDL_FreeSurface(cv.at(i)->suit);

						if(cv.at(i)->belt != NULL)
							SDL_FreeSurface(cv.at(i)->belt);

						if(cv.at(i)->c != NULL)
						{
							SDL_FreeSurface(cv.at(i)->c->text1);
							SDL_FreeSurface(cv.at(i)->c->theWindow);

							if(cv.at(i)->c->text2 != NULL)
								SDL_FreeSurface(cv.at(i)->c->text2);

							delete cv.at(i)->c;
							cv.at(i)->c = NULL;
						}

						for(unsigned int y = 0; y < cv.at(i)->menu.size(); y++)
						{
							SDL_FreeSurface(cv.at(i)->menu.at(y).btn);
							SDL_FreeSurface(cv.at(i)->menu.at(y).tSur);
						}

						cv.at(i)->menu.clear();

						delete cv.at(i);
					}

					cv.clear();

					for(unsigned int i = 0; i < cm.size(); i++)
					{
						SDL_FreeSurface(cm.at(i)->nameSurface);
						SDL_FreeSurface(cm.at(i)->whenMarkName);

						delete cm.at(i);
					}

					cm.clear();
				}

				else
				{
					int pos = 4;
					int type = cpak[3];
					int id = atoi(getString(cpak, (pos + 1), (int)cpak[pos]).c_str());

					if(type == 1)
					{
						for(unsigned int i = 0; i < cv.size(); i++)
						{
							if(cv.at(i)->id == id)
							{
								if(mark->marktype == 1 && mark->npcmark == id)
								{
									mark->marktype = 0;
									mark->npcmark = 0;
									mark->enable = false;
								}

								cv.at(i)->nameSur.clean();
								SDL_FreeSurface(cv.at(i)->deadImg);

								if(cv.at(i)->weapon != NULL)
									SDL_FreeSurface(cv.at(i)->weapon);

								if(cv.at(i)->hat != NULL)
									SDL_FreeSurface(cv.at(i)->hat);

								if(cv.at(i)->suit != NULL)
									SDL_FreeSurface(cv.at(i)->suit);

								if(cv.at(i)->belt != NULL)
									SDL_FreeSurface(cv.at(i)->belt);

								if(cv.at(i)->c != NULL)
								{
									SDL_FreeSurface(cv.at(i)->c->theWindow);
									SDL_FreeSurface(cv.at(i)->c->text1);

									if(cv.at(i)->c->text2 != NULL)
										SDL_FreeSurface(cv.at(i)->c->text2);

									delete cv.at(i)->c;
								}

								for(unsigned int y = 0; y < cv.at(i)->menu.size(); y++)
								{
									SDL_FreeSurface(cv.at(i)->menu.at(y).btn);
									SDL_FreeSurface(cv.at(i)->menu.at(y).tSur);
								}

								cv.at(i)->menu.clear();

								delete cv.at(i);
								cv.erase(cv.begin() + i);

								break;
							}
						}
					}

					else if(type == 2)
					{
						for(unsigned int i = 0; i < cm.size(); i++)
						{
							if(cm.at(i)->mId == id)
							{
								if(mark->marktype == 2 && mark->npcmark == id)
								{
									mark->marktype = 0;
									mark->npcmark = 0;
									mark->enable = false;
								}

								cm.at(i)->dead = true;
								cm.at(i)->fadeway = false;
								cm.at(i)->fadep = 255;
								cm.at(i)->fades = true;

								break;
							}
						}
					}
				}
			break;
			}

		case 0x13:
			{
				switch(cpak[3])
				{
					case HPUPDATE:
						{
							int pos = 5;
							int dead = cpak[4];
							int hp = atoi(getString(cpak, 6, (int)cpak[5]).c_str());
							pos += ((int)cpak[pos] + 1);
							int maxhp = atoi(getString(cpak, (pos + 1), (int)cpak[pos]).c_str());

							int finalHP = hp;
							int finalMaxHP = maxhp;

							if(maxhp != me->maxhp)
								me->maxhp = maxhp;

							if(dead == 1)
							{
								me->dead = true;

								string msg1 = "You have been killed. Reviving in ";
								string msg2 = " seconds.";

								char temp[10] = "";

								Animation *deada = new Animation();

								deada->disTime = (getTime(true) + 10000);
								deada->frame = 0;
								deada->x = (int)me->x;
								deada->y = (int)(me->y - 32);
								deada->fps = 1;

								for(int i = dieTime; i >= 0; i--)
								{
									string fullString = msg1;

									_itoa_s(i, temp, 10, 10);

									fullString += temp;
									fullString += msg2;

									SDL_Surface *msg = RenderText_Solid(cour, fullString.c_str(), clrFg);

									deada->ani.push_back(msg);
								}

								anis.push_back(deada);

								for(unsigned int i = 0; i < icons.size(); i++)
								{
									if(icons.at(i)->changeEffect == SPEED)
										me->speed -= icons.at(i)->lvl;

									else if(icons.at(i)->changeEffect == SPEEDMINUS)
										me->speed += icons.at(i)->lvl;

									else if(icons.at(i)->changeEffect == STAMINA)
									{
										char temp2[10] = "";

										me->hpPoint -= icons.at(i)->lvl;

										_itoa_s(me->hpPoint, temp2, 10, 10);

										SDL_FreeSurface(addstatwindow.at(0)->hp);
										addstatwindow.at(0)->hp = RenderText_Solid(cour, temp2, clrFgBlack);
									}

									else if(icons.at(i)->changeEffect == STRENGTH)
									{
										char temp2[10] = "";

										me->power -= icons.at(i)->lvl;

										_itoa_s(me->power, temp2, 10, 10);

										SDL_FreeSurface(addstatwindow.at(0)->power);
										addstatwindow.at(0)->power = RenderText_Solid(cour, temp2, clrFgBlack);
									}

									SDL_FreeSurface(icons.at(i)->name1);
									SDL_FreeSurface(icons.at(i)->name2);
									SDL_FreeSurface(icons.at(i)->tSurface);
									SDL_FreeSurface(icons.at(i)->icon);

									delete icons.at(i); 
								}

								icons.clear();

								for(unsigned int q = 0; q < cm.size(); q++)
								{
									if(me->id == cm.at(q)->followId)
										cm.at(q)->followId = 0;
								}

								me->hp = 0;
								finalHP = 0;
							}

							else if(hp == 0)
							{
								me->hp = 1;
								finalHP = 1;
							}

							else
							{
								me->hp = hp;
								finalHP = hp;
							}
							
							char temp[10] = "";
							char full[50] = "";

							for(unsigned int i = 0; i < win.size(); i++)
							{
								if(win.at(i)->TYPE == INFOWINDOW)
								{
									SDL_FreeSurface(win.at(i)->hpSurface);

									_itoa_s(finalHP, temp, 10, 10);

									strcpy_s(full, temp);
									strcat_s(full, "/");

									_itoa_s(finalMaxHP, temp, 10, 10);

									strcat_s(full, temp);

									win.at(i)->hpSurface = RenderText_Solid(cour, full, clrFgBlack);

									break;
								}
							}

						break;
						}

					case MANAUPDATE:
						{
							int pos = 4;
							int hp = atoi(getString(cpak, (pos + 1), (int)cpak[pos]).c_str());
							pos += ((int)cpak[pos] + 1);
							int maxhp = atoi(getString(cpak, (pos + 1), (int)cpak[pos]).c_str());

							int finalHP = hp;
							int finalMaxHP = maxhp;

							if(maxhp != me->maxmana)
								me->maxmana = maxhp;

							me->mana = hp;

							SDL_FreeSurface(me->manaSurface);

							char temp[10] = "";
							char full[50] = "";
							_itoa_s(finalHP, temp, 10, 10);

							strcpy_s(full, temp);
							strcat_s(full, "/");

							_itoa_s(finalMaxHP, temp, 10, 10);

							strcat_s(full, temp);

							me->manaSurface = RenderText_Solid(cour, full, clrFgBlack);
						break;
						}

					case ENERGYUPDATE:
						{
							int pos = 4;
							int hp = atoi(getString(cpak, (pos + 1), (int)cpak[pos]).c_str());
							pos += ((int)cpak[pos] + 1);
							int maxhp = atoi(getString(cpak, (pos + 1), (int)cpak[pos]).c_str());

							int finalHP = hp;
							int finalMaxHP = maxhp;

							if(maxhp != me->maxenergy)
								me->maxenergy = maxhp;

							me->energy = hp;

							SDL_FreeSurface(me->energySurface);

							char temp[10] = "";
							char full[50] = "";
							_itoa_s(finalHP, temp, 10, 10);

							strcpy_s(full, temp);
							strcat_s(full, "/");

							_itoa_s(finalMaxHP, temp, 10, 10);

							strcat_s(full, temp);

							me->energySurface = RenderText_Solid(cour, full, clrFgBlack);
						break;
						}

					default: Log("Unknown refresh header 0x%d.", cpak[3]);
						break;
				}
			break;
			}

		case 0x14:
			{
				int pos = 3;

				int id = atoi(getString(cpak, (pos + 1), (int)cpak[pos]).c_str());
				pos += ((int)cpak[pos] + 1);

				int x = atoi(getString(cpak, (pos + 1), (int)cpak[pos]).c_str());
				pos += ((int)cpak[pos] + 1);

				int y = atoi(getString(cpak, (pos + 1), (int)cpak[pos]).c_str());

				for(unsigned int i = 0; i < cv.size(); i++)
				{
					if(id == cv.at(i)->id)
					{
						cv.at(i)->x = (float)x;
						cv.at(i)->y = (float)y;

						if((cv.at(i)->x >= camShowX && camShowToX >= cv.at(i)->x) && (cv.at(i)->y >=camShowY && camShowToY >= cv.at(i)->y))
						{
							int t = screen->w;
							int t2 = screen->h;

							cv.at(i)->cX = (camShowToX - (int)cv.at(i)->x);
							cv.at(i)->cY = (camShowToY - (int)cv.at(i)->y);

							t -= cv.at(i)->cX;
							t2 -= cv.at(i)->cY;

							cv.at(i)->cX = t;
							cv.at(i)->cY = t2;

							cv.at(i)->inWin = true;
						}

						else
						{
							if(cv.at(i)->inWin)
								cv.at(i)->inWin = false;
						}

						break;
					}
				}

			break;		
			}

		case 0x15:
			{
				int pos = 3;

				Chat c = Chat();
				c.msg = getString(cpak, (pos + 1), (int)cpak[pos]);
				pos += ((int)cpak[pos] + 1);

				c.name = getString(cpak, (pos + 1), (int)cpak[pos]);
				pos += ((int)cpak[pos] + 1);

				bool isPro = getBool(cpak[pos]);
				pos++;

				bool isNpc = getBool(cpak[pos]);
				pos++;

				int color = cpak[pos];
				SDL_Color col = clrFgBlack;

				bool cleanText = true;

				switch(color)
				{
					case 1: col = clrFgRed;
						break;

					case 2: col = clrFgGreen;
						break;

					case 3: col = clrFgBlue;
						break;

					case 4: col = clrFg;
						break;
				}

				SDL_Surface *fullMe = RenderText_Solid(cour, c.msg.c_str(), col);

				SDL_Surface *text1 = NULL;
				SDL_Surface *text2 = NULL;

				if(fullMe->w > 210)
				{
					if(fullMe != NULL)
						SDL_FreeSurface(fullMe);

					fullMe = NULL;

					vector<string> npcString;

					string tempString = "";
					unsigned int readPos = 0;

					for(unsigned int n = 0; n <= c.msg.length(); n++)
					{
						for(unsigned int t = readPos; t < n; t++)
							tempString += c.msg[t];

						if(fullMe != NULL)
							SDL_FreeSurface(fullMe);

						fullMe = NULL;

						fullMe = RenderText_Solid(cour, tempString.c_str(), col);

						if(fullMe == NULL)
						{
						}

						else
						{
							if(fullMe->w > 210)
							{
								npcString.push_back(tempString);

								readPos = n;
							}

							if(n == c.msg.length() && readPos != n)
								npcString.push_back(tempString);
						}

						tempString = "";
					}

					if(fullMe != NULL)
						SDL_FreeSurface(fullMe);

					if(npcString.size() > 0)
					{
						text1 = RenderText_Solid(cour, npcString.at(0).c_str(), col);

						if(npcString.size() > 1)
							text2 = RenderText_Solid(cour, npcString.at(1).c_str(), col);
					}
				}

				else
				{
					text1 = fullMe;
					text2 = NULL;
				}

				string fullString = c.name;
				fullString += ": ";
				fullString += c.msg;

				if(!isNpc)
				{
					c.message = RenderText_Solid(cour, fullString.c_str(), col);
					chatFunc->AddMessage(c);
				}

				if(strcmp(c.name.c_str(), "Notice") == 0)
				{
					string noticeText = "[Notice] ";

					noticeText += c.msg;

					Notifer *notice = new Notifer();

					notice->dTime = (getTime(true) + 6000);
					notice->penya = false;
					notice->sTime = 0;
					notice->msg = RenderText_Solid(courNotice, noticeText.c_str(), clrFg);
					notice->y = (screen->h / 8);
					notice->x = ((screen->w / 2) - (notice->msg->w / 2));

					for(unsigned int i = 0; i < nv.size(); i++)
					{
						if(nv.at(i)->y == notice->y)
						{
							notice->y += 40;
						}
					}

					nv.push_back(notice);
				}

				else
				{
					ChatWindow *cw = me->c;

					if(strcmp(me->name, c.name.c_str()) == 0)
					{
						if(me->c != NULL)
						{
							SDL_FreeSurface(me->c->theWindow);
							SDL_FreeSurface(me->c->text1);

							if(me->c->text2 != NULL)
								SDL_FreeSurface(me->c->text2);

							delete me->c;
						}

						me->c = new ChatWindow();
						me->c->theWindow = loadimage("Data/Interface/PlayerChat-Window.png", true, 255, 255, 255);

						me->c->text1 = text1;
						me->c->text2 = text2;

						me->c->removeTime = getTime(true) + 6000;

						cleanText = false;
					}

					else
					{
						if(!isNpc)
						{
							for(unsigned int i = 0; i < cv.size(); i++)
							{
								if(strcmp(cv.at(i)->name, c.name.c_str()) == 0)
								{
									if(cv.at(i)->c != NULL)
									{
										SDL_FreeSurface(cv.at(i)->c->theWindow);
										SDL_FreeSurface(cv.at(i)->c->text1);

										if(cv.at(i)->c->text2 != NULL)
											SDL_FreeSurface(cv.at(i)->c->text2);

										delete cv.at(i)->c;
									}

									cv.at(i)->c = new ChatWindow();
									cv.at(i)->c->theWindow = loadimage("Data/Interface/PlayerChat-Window.png", true, 255, 255, 255);

									cv.at(i)->c->text1 = text1;
									cv.at(i)->c->text2 = text2;

									cv.at(i)->c->removeTime = getTime(true) + 6000;

									cleanText = false;

									break;
								}
							}
						}

						else
						{
							for(unsigned int i = 0; i < npc.size(); i++)
							{
								if(strcmp(npc.at(i)->name, c.name.c_str()) == 0)
								{
									if(npc.at(i)->c != NULL)
									{
										SDL_FreeSurface(npc.at(i)->c->theWindow);
										SDL_FreeSurface(npc.at(i)->c->text1);

										if(npc.at(i)->c->text2 != NULL)
											SDL_FreeSurface(npc.at(i)->c->text2);

										delete npc.at(i)->c;
									}

									npc.at(i)->c = new ChatWindow();
									npc.at(i)->c->theWindow = loadimage("Data/Interface/PlayerChat-Window.png", true, 255, 255, 255);

									npc.at(i)->c->text1 = text1;
									npc.at(i)->c->text2 = text2;

									npc.at(i)->c->removeTime = getTime(true) + 6000;

									cleanText = false;

									break;
								}
							}
						}
					}
				}

				if(cleanText)
				{
					if(text1 != NULL)
						SDL_FreeSurface(text1);

					if(text2 != NULL)
						SDL_FreeSurface(text2);
				}
			break;
			}

		case 0x16:
			{
				int pos = 5;
				int PARAM = cpak[3];
				int cWay = cpak[4];

				int cX = atoi(getString(cpak, (pos + 1), (int)cpak[pos]).c_str());
				pos += ((int)cpak[pos] + 1);

				int cY = atoi(getString(cpak, (pos + 1), (int)cpak[pos]).c_str());
				pos += ((int)cpak[pos] + 1);

				int id = atoi(getString(cpak, (pos + 1), (int)cpak[pos]).c_str());
				pos += ((int)cpak[pos] + 1);

				int target = atoi(getString(cpak, (pos + 1), (int)cpak[pos]).c_str());
				pos += ((int)cpak[pos] + 1);

				int type = cpak[pos];
				pos++;

				int igenom = atoi(getString(cpak, (pos + 1), (int)cpak[pos]).c_str());
				pos += ((int)cpak[pos] + 1);

				int bspeed = atoi(getString(cpak, (pos + 1), (int)cpak[pos]).c_str());
				pos += ((int)cpak[pos] + 1);

				int brot = atoi(getString(cpak, (pos + 1), (int)cpak[pos]).c_str());
				pos += ((int)cpak[pos] + 1);

				int bdisx = atoi(getString(cpak, (pos + 1), (int)cpak[pos]).c_str());
				pos += ((int)cpak[pos] + 1);

				Bullet *bullet = new Bullet();

				bullet->cX = (float)cX;
				bullet->cY = (float)cY;
				bullet->startY = cY;
				bullet->startX = cX;
				bullet->cWay = cWay;
				bullet->owner = id;
				bullet->param = PARAM;
				bullet->move.start(true);
				bullet->target = target;
				bullet->type = type;
				bullet->speed = bspeed;
				bullet->rot = brot;
				bullet->disX = bdisx;

				if(igenom > 0)
					bullet->igenom = true;

				if((bullet->cX >= camShowX && camShowToX >= bullet->cX) && (bullet->cY >=camShowY && camShowToY >= bullet->cY))
				{
					int t = screen->w;
					int t2 = screen->h;

					bullet->sX = (camShowToX - (int)bullet->cX);
					bullet->sY = (camShowToY - (int)bullet->cY);

					t -= bullet->sX;
					t2 -= bullet->sY;

					bullet->sX = t;
					bullet->sY = t2;

					bullet->inWin = true;
				}

				if(PARAM == 1)
					bullet->cPic = getSurface(BULLET);

				else
				{
					bullet->cPic = getSurface(PARAM);
				}

				bullets.push_back(bullet);

				sound->playSound(1, 0);

			break;
			}

		case 0x17:
			{
				if(cpak[3] == 2)
				{
					Notifer *n = new Notifer();

					n->x = 0;
					n->y = 0;

					n->msg = RenderText_Solid(cour, "Can't equip in this area.", clrFg);

					n->dTime = (getTime(true) + 2000);
					n->sTime = getTime(true);

					nv.push_back(n);
				}

				else
				{
					int status = cpak[3];
					int id = atoi(getString(cpak, 5, (int)cpak[4]).c_str());

					if(id == me->id)
					{
						Notifer *n = new Notifer();

						n->x = 0;
						n->y = 0;
						n->dTime = (getTime(true) + 2000);
						n->sTime = getTime(true);

						if(status == 1)
						{
							me->equipped = true;

							n->msg = RenderText_Solid(cour, "Equipped weapon.", clrFg);
						}

						else
						{
							me->equipped = false;

							n->msg = RenderText_Solid(cour, "Un-equipped weapon.", clrFg);
						}

						nv.push_back(n);
					}

					else
					{
						for(unsigned int i = 0; i < cv.size(); i++)
						{
							if(id == cv.at(i)->id)
							{
								if(status == 1)
									cv.at(i)->equipped = true;
								else
									cv.at(i)->equipped = false;
							}
						}
					}
				}

			break;
			}

		case 0x19:
			{
				int pos = 3;

				int level = atoi(getString(cpak, (pos + 1), (int)cpak[pos]).c_str());
				pos += ((int)cpak[pos] + 1);

				float exp = (float)atof(getString(cpak, (pos + 1), (int)cpak[pos]).c_str());
				pos += ((int)cpak[pos] + 1);

				int pAv = atoi(getString(cpak, (pos + 1), (int)cpak[pos]).c_str());

				me->exp = exp;

				char temp[20] = "";
				char full[50] = "";

				for(unsigned int i = 0; i < win.size(); i++)
				{
					if(win.at(i)->TYPE == INFOWINDOW)
					{
						SDL_FreeSurface(win.at(i)->expSurface);

						sprintf_s(temp, 20, "%2.2f", me->exp);

						strcpy_s(full, temp);
						strcat_s(full, "%/100%");

						win.at(i)->expSurface = RenderText_Solid(cour, full, clrFgBlack);

						if(level != me->level)
						{
							SDL_FreeSurface(win.at(i)->levelSurface);

							_itoa_s(level, temp, 10, 10);

							win.at(i)->levelSurface = RenderText_Solid(cour, temp, clrFgBlack);
						}

						break;
					}
				}

				if(me->pAv != pAv)
				{
					me->pAv = pAv;

					if(addstatwindow.size() > 0)
					{
						char temp[10] = "";

						_itoa_s(pAv, temp, 10, 10);
						addstatwindow.at(0)->pointsav = RenderText_Solid(cour, temp, clrFgBlack);
					}
				}

				if(level != me->level)
				{
					char temp[10] = "";

					me->level = level;

					if(partyw.size() > 0)
					{
						for(unsigned int i = 0; i < partyw.at(0)->pm.size(); i++)
						{
							if(strcmp(me->name, partyw.at(0)->pm.at(i).name) == 0)
							{
								SDL_FreeSurface(partyw.at(0)->pm.at(i).lvls);

								_itoa_s(level, temp, 10, 10);

								partyw.at(0)->pm.at(i).lvls = RenderText_Solid(cour, temp, clrFgBlack);
								partyw.at(0)->pm.at(i).level = level;

								break;
							}
						}
					}
				}
			break;
			}

		case 0x21:
			{
				int deadStatus = cpak[3];
				int id = atoi(getString(cpak, 5, (int)cpak[4]).c_str());

				for(unsigned int i = 0; i < cv.size(); i++)
				{
					if(id == cv.at(i)->id)
					{
						if(deadStatus == 1)
							cv.at(i)->dead = true;
						else
							cv.at(i)->dead = false;

						break;
					}
				}

				for(unsigned int i = 0; i < cm.size(); i++)
				{
					if(id == cm.at(i)->followId)
					{
						cm.at(i)->followId = 0;

						break;
					}
				}
			break;
			}

		case 0x22:
			{
				int pos = 3;

				int power = atoi(getString(cpak, (pos + 1), (int)cpak[pos]).c_str());
				pos += ((int)cpak[pos] + 1);

				int sta = atoi(getString(cpak, (pos + 1), (int)cpak[pos]).c_str());
				pos += ((int)cpak[pos] + 1);

				int agi = atoi(getString(cpak, (pos + 1), (int)cpak[pos]).c_str());
				pos += ((int)cpak[pos] + 1);

				int intt = atoi(getString(cpak, (pos + 1), (int)cpak[pos]).c_str());
				pos += ((int)cpak[pos] + 1);

				int pav = atoi(getString(cpak, (pos + 1), (int)cpak[pos]).c_str());
				pos += ((int)cpak[pos] + 1);

				int hp = atoi(getString(cpak, (pos + 1), (int)cpak[pos]).c_str());
				pos += ((int)cpak[pos] + 1);

				bonusAttackSpeed = atoi(getString(cpak, (pos + 1), (int)cpak[pos]).c_str());
				pos += ((int)cpak[pos] + 1);

				int powerb = atoi(getString(cpak, (pos + 1), (int)cpak[pos]).c_str());
				pos += ((int)cpak[pos] + 1);

				int stab = atoi(getString(cpak, (pos + 1), (int)cpak[pos]).c_str());
				pos += ((int)cpak[pos] + 1);

				int agib = atoi(getString(cpak, (pos + 1), (int)cpak[pos]).c_str());
				pos += ((int)cpak[pos] + 1);

				int intb = atoi(getString(cpak, (pos + 1), (int)cpak[pos]).c_str());
				pos += ((int)cpak[pos] + 1);

				me->power = power;
				me->hpPoint = sta;
				me->agility = agi;
				me->intt = intt;
				me->pAv = pav;

				if(me->maxhp != hp)
				{
					me->maxhp = hp;

					char temp[10] = "";
					char full[50] = "";

					for(unsigned int i = 0; i < win.size(); i++)
					{
						if(win.at(i)->TYPE == INFOWINDOW)
						{
							SDL_FreeSurface(win.at(i)->hpSurface);

							_itoa_s(me->hp, temp, 10, 10);

							strcpy_s(full, temp);
							strcat_s(full, "/");

							_itoa_s(me->maxhp, temp, 10, 10);

							strcat_s(full, temp);

							win.at(i)->hpSurface = RenderText_Solid(cour, full, clrFgBlack);

							break;
						}
					}
				}

				if(addstatwindow.size() > 0)
				{
					char temp2[10] = "";

					bool staBuffed = false;
					bool strBuffed = false;
					bool agiBuffed = false;
					bool intBuffed = false;

					/*
					for(unsigned int i = 0; i < icons.size(); i++)
					{
						if(icons.at(i)->changeEffect == STAMINA)
							staBuffed = true;

						else if(icons.at(i)->changeEffect == STRENGTH)
							strBuffed = true;

						else if(icons.at(i)->changeEffect == AGILITY)
							agiBuffed = true;

						else if(icons.at(i)->changeEffect == INTT)
							intBuffed = true;
					}
					*/

					if(powerb > 0)
						strBuffed = true;

					if(stab > 0)
						staBuffed = true;

					if(agib > 0)
						agiBuffed = true;

					if(intb > 0)
						intBuffed = true;

					_itoa_s(power, temp2, 10, 10);
					SDL_FreeSurface(addstatwindow.at(0)->power);
					if(strBuffed)
						addstatwindow.at(0)->power = RenderText_Solid(cour, temp2, clrFgBlue);
					else
						addstatwindow.at(0)->power = RenderText_Solid(cour, temp2, clrFgBlack);

					_itoa_s(sta, temp2, 10, 10);
					SDL_FreeSurface(addstatwindow.at(0)->hp);
					if(staBuffed)
						addstatwindow.at(0)->hp = RenderText_Solid(cour, temp2, clrFgBlue);
					else
						addstatwindow.at(0)->hp = RenderText_Solid(cour, temp2, clrFgBlack);

					_itoa_s(agi, temp2, 10, 10);
					SDL_FreeSurface(addstatwindow.at(0)->agility);
					if(agiBuffed)
						addstatwindow.at(0)->agility = RenderText_Solid(cour, temp2, clrFgBlue);
					else
						addstatwindow.at(0)->agility = RenderText_Solid(cour, temp2, clrFgBlack);

					_itoa_s(intt, temp2, 10, 10);
					SDL_FreeSurface(addstatwindow.at(0)->intt);
					if(intBuffed)
						addstatwindow.at(0)->intt = RenderText_Solid(cour, temp2, clrFgBlue);
					else
						addstatwindow.at(0)->intt = RenderText_Solid(cour, temp2, clrFgBlack);

					_itoa_s(pav, temp2, 10, 10);
					SDL_FreeSurface(addstatwindow.at(0)->pointsav);
					addstatwindow.at(0)->pointsav = RenderText_Solid(cour, temp2, clrFgBlack);
				}

			break;
			}

		case 0x23:
			{
				int pos = 3;

				int id = atoi(getString(cpak, (pos + 1), (int)cpak[pos]).c_str());
				pos += ((int)cpak[pos] + 1);

				string name = getString(cpak, (pos + 1), (int)cpak[pos]);

				if(id == me->id)
				{
					strcpy_s(me->name, name.c_str());

					me->nameSur.clean();
					CreateNameSurface(&me->nameSur, cour, clrFg, me->name, 1);
				}

				else
				{
					for(unsigned int i = 0; i < cv.size(); i++)
					{
						if(id == cv.at(i)->id)
						{
							strcpy_s(cv.at(i)->name, name.c_str());

							cv.at(i)->nameSur.clean();
							CreateNameSurface(&cv.at(i)->nameSur, cour, clrFg, cv.at(i)->name, 1);

							break;
						}
					}
				}
			break;
			}

		case 0x24:
			{
				int pos = 6;
				int names = cpak[3];
				int kick = cpak[4];
				int create = cpak[5];

				string joined = "";

				if(kick == 1)
				{
					char temp[10] = "";

					if(!me->inParty && create == 1)
					{
						me->inParty = true;

						me->nameSur.clean();
						CreateNameSurface(&me->nameSur, cour, clrFgRed, me->name, 1);

						PartyMember pm = PartyMember();

						strcpy_s(pm.name, me->name);
						pm.level = me->level;

						_itoa_s(pm.level, temp, 10, 10);

						pm.names = RenderText_Solid(cour, pm.name, clrFgBlack);
						pm.lvls = RenderText_Solid(cour, temp, clrFgBlack);

						partyw.at(0)->pm.push_back(pm);
					}

					for(int z = 0; z < names; z++)
					{
						joined = getString(cpak, (pos + 1), (int)cpak[pos]);
						pos += ((int)cpak[pos] + 1);

						int level = cpak[pos];
						pos++;

						for(unsigned int i = 0; i < cv.size(); i++)
						{
							if(strcmp(cv.at(i)->name, joined.c_str()) == 0)
							{
								cv.at(i)->inParty = true;

								cv.at(i)->nameSur.clean();
								CreateNameSurface(&cv.at(i)->nameSur, cour, clrFgRed, cv.at(i)->name, 1);

								break;
							}
						}

						PartyMember pm = PartyMember();

						strcpy_s(pm.name, joined.c_str());
						pm.level = level;

						_itoa_s(pm.level, temp, 10, 10);

						pm.names = RenderText_Solid(cour, pm.name, clrFgBlack);
						pm.lvls = RenderText_Solid(cour, temp, clrFgBlack);

						partyw.at(0)->pm.push_back(pm);
					}

					if(!me->inParty && create == 2)
					{
						me->inParty = true;

						me->nameSur.clean();
						CreateNameSurface(&me->nameSur, cour, clrFgRed, me->name, 1);

						PartyMember pm = PartyMember();

						strcpy_s(pm.name, me->name);
						pm.level = me->level;

						_itoa_s(pm.level, temp, 10, 10);

						pm.names = RenderText_Solid(cour, pm.name, clrFgBlack);
						pm.lvls = RenderText_Solid(cour, temp, clrFgBlack);

						partyw.at(0)->pm.push_back(pm);
					}
				}

				else if(kick == 2)
				{
					for(int z = 0; z < names; z++)
					{
						joined = getString(cpak, (pos + 1), (int)cpak[pos]);
						pos += ((int)cpak[pos] + 1);

						if(strcmp(me->name, joined.c_str()) == 0)
						{
							me->inParty = false;

							me->nameSur.clean();
							CreateNameSurface(&me->nameSur, cour, clrFg, me->name, 1);

							for(unsigned int i = 0; i < cv.size(); i++)
							{
								cv.at(i)->nameSur.clean();
								CreateNameSurface(&cv.at(i)->nameSur, cour, clrFg, cv.at(i)->name, 1);
							}

							if(partyw.size() > 0)
							{
								for(unsigned int n = 0; n < partyw.at(0)->pm.size(); n++)
								{
									SDL_FreeSurface(partyw.at(0)->pm.at(n).names);
									SDL_FreeSurface(partyw.at(0)->pm.at(n).lvls);
								}

								partyw.at(0)->pm.clear();
							}
						}

						else
						{
							for(unsigned int i = 0; i < cv.size(); i++)
							{
								if(strcmp(cv.at(i)->name, joined.c_str()) == 0)
								{
									cv.at(i)->inParty = false;

									cv.at(i)->nameSur.clean();
									CreateNameSurface(&cv.at(i)->nameSur, cour, clrFg, cv.at(i)->name, 1);

									break;
								}
							}

							for(unsigned int y = 0; y < partyw.at(0)->pm.size(); y++)
							{
								PartyMember *c = &partyw.at(0)->pm.at(y);

								if(strcmp(joined.c_str(), c->name) == 0)
								{
									SDL_FreeSurface(c->names);
									SDL_FreeSurface(c->lvls);

									partyw.at(0)->pm.erase(partyw.at(0)->pm.begin() + y);

									break;
								}
							}
						}
					}
				}
			break;
			}

		case 0x25:
			{
				int level = cpak[3];
				string name = getString(cpak, 5, (int)cpak[4]);

				char temp[10] = "";

				if(partyw.size() > 0)
				{
					for(unsigned int i = 0; i < partyw.at(0)->pm.size(); i++)
					{
						PartyMember *c = &partyw.at(0)->pm.at(i);

						if(strcmp(c->name, name.c_str()) == 0)
						{
							SDL_FreeSurface(c->lvls);

							_itoa_s(level, temp, 10, 10);

							c->lvls = RenderText_Solid(cour, temp, clrFgBlack);
							c->level = level;

							break;
						}
					}
				}
			break;
			}

		case 0x27:
			{
				string name = getString(cpak, 4, (int)cpak[3]);
				char full[100] = "From: ";

				strcat_s(full, name.c_str());

				PartyInvite *w = new PartyInvite();

				w->win = loadimage("Data/Interface/PartyInvite-Window.png", false, 0, 0, 0);
				w->no = loadimage("Data/Interface/No-Button.png", true, 255, 255, 255);
				w->yes = loadimage("Data/Interface/Yes-Button.png", true, 255, 255, 255);

				if(transWindow)
					SDL_SetAlpha(w->win, SDL_SRCALPHA | SDL_RLEACCEL, 128);

				w->from = RenderText_Solid(cour, full, clrFgBlack);

				strcpy_s(w->name, name.c_str());

				w->x = 398;
				w->y = 62;

				if(partyw.size() > 0)
				{
					if(partyw.at(0)->enablePress)
						partyw.at(0)->enablePress = false;
				}

				pi.push_back(w);
			break;
			}

		case 0x28:
			{
				int pos = 4;

				if(cpak[3] == 1)
				{
					bool found = false;

					int effect = atoi(getString(cpak, (pos + 1), (int)cpak[pos]).c_str());
					pos += ((int)cpak[pos] + 1);

					int changeEffect = atoi(getString(cpak, (pos + 1), (int)cpak[pos]).c_str());
					pos += ((int)cpak[pos] + 1);

					int value = atoi(getString(cpak, (pos + 1), (int)cpak[pos]).c_str());
					pos += ((int)cpak[pos] + 1);

					int timeLeft = atoi(getString(cpak, (pos + 1), (int)cpak[pos]).c_str());
					pos += ((int)cpak[pos] + 1);

					for(unsigned int t = 0; t < icons.size(); t++)
					{
						if(effect == icons.at(t)->effect)
						{
							icons.at(t)->timeLeft = (float)timeLeft;
							icons.at(t)->lvl = value;
							icons.at(t)->nextRemove = (getTime(true) + 1000);

							char buf[10] = "";
							_itoa_s(timeLeft, buf, 10, 10);

							SDL_FreeSurface(icons.at(t)->tSurface);
							icons.at(t)->tSurface = RenderText_Blended(courSmall, buf, clrFgBlack);

							found = true;

							break;
						}
					}

					if(!found)
					{
						Icon *ic = new Icon();

						ic->icon = loadimage("Data/Icon/Buff-Icon.png", true, 255, 255, 255);
						ic->x = (166 + ((icons.size() * 42)));
						ic->y = 0;
						ic->nextRemove = (getTime(true) + 1000);
						ic->timeLeft = (float)timeLeft;

						ic->timeLeftX = (ic->x + 5);
						ic->timeLeftY = (ic->y + 28);

						ic->name1X = (ic->x + 5);
						ic->name1Y = (ic->y + 3);
						
						ic->name2X = (ic->x + 5);
						ic->name2Y = (ic->name1Y + 10);

						ic->name1 = RenderText_Blended(courSmall, getBuffNameOne(effect).c_str(), clrFgBlack);
						ic->name2 = RenderText_Blended(courSmall, getBuffNameTwo(effect).c_str(), clrFgBlack);

						char tLeft[10] = "";
						_itoa_s(timeLeft, tLeft, 10, 10);

						ic->tSurface = RenderText_Blended(courSmall, tLeft, clrFgBlack);

						ic->lvl = value;
						ic->effect = effect;
						ic->changeEffect = changeEffect;

						icons.push_back(ic);
					}
				}

				else if(cpak[3] == 2)
				{
					int idBuff = atoi(getString(cpak, (pos + 1), (int)cpak[pos]).c_str());
					pos += ((int)cpak[pos] + 1);

					char temp2[10] = "";

					for(unsigned int i = 0; i < icons.size(); i++)
					{
						if(idBuff == icons.at(i)->effect)
						{
							SDL_FreeSurface(icons.at(i)->tSurface);
							SDL_FreeSurface(icons.at(i)->name1);
							SDL_FreeSurface(icons.at(i)->name2);
							SDL_FreeSurface(icons.at(i)->icon);

							for(unsigned int y = (i + 1); y < icons.size(); y++)
							{
								icons.at(y)->x -= 42;
								icons.at(y)->name1X -= 42;
								icons.at(y)->name2X -= 42;
								icons.at(y)->timeLeftX -= 42;
							}

							delete icons.at(i);
							icons.erase(icons.begin() + i);

							break;
						}
					}

					if(drag && dragged.type == DRAGBUFFS)
						drag = false;
				}
			break;
			}

		case 0x29:
			{
				int pos = 5;
				int enableDisable = cpak[3];
				int amount = atoi(getString(cpak, (pos + 1), (int)cpak[pos]).c_str());
				pos += ((int)cpak[pos] + 1);

				int id = atoi(getString(cpak, (pos + 1), (int)cpak[pos]).c_str());

				if(me->id == id)
				{
					switch(cpak[4])
					{
						case SPEED: case SPEEDMINUS:
							{
								if(amount <= 0)
									amount = 1;

								me->speed = amount;

								me->pixSec = (me->speed * 62);
							break;
							}

						case STAMINA:
							{
								pos += ((int)cpak[pos] + 1);
								int newSta = atoi(getString(cpak, (pos + 1), (int)cpak[pos]).c_str());

								if(addstatwindow.size() > 0)
								{
									char temp2[10] = "";

									_itoa_s(newSta, temp2, 10, 10);
									SDL_FreeSurface(addstatwindow.at(0)->hp);
									if(enableDisable == 2)
										addstatwindow.at(0)->hp = RenderText_Solid(cour, temp2, clrFgBlack);
									else
										addstatwindow.at(0)->hp = RenderText_Solid(cour, temp2, clrFgBlue);

									if(me->maxhp > amount)
										me->hp = amount;

									me->hpPoint = newSta;
									me->maxhp = amount;

									char temp[10] = "";
									char full[50] = "";

									for(unsigned int i = 0; i < win.size(); i++)
									{
										if(win.at(i)->TYPE == INFOWINDOW)
										{
											SDL_FreeSurface(win.at(i)->hpSurface);

											_itoa_s(me->hp, temp, 10, 10);

											strcpy_s(full, temp);
											strcat_s(full, "/");

											_itoa_s(me->maxhp, temp, 10, 10);

											strcat_s(full, temp);

											win.at(i)->hpSurface = RenderText_Solid(cour, full, clrFgBlack);

											break;
										}
									}
								}
							break;
							}

						case STRENGTH:
							{
								if(addstatwindow.size() > 0)
								{
									char temp2[10] = "";

									_itoa_s(amount, temp2, 10, 10);
									SDL_FreeSurface(addstatwindow.at(0)->power);
									if(enableDisable == 2)
										addstatwindow.at(0)->power = RenderText_Solid(cour, temp2, clrFgBlack);
									else
										addstatwindow.at(0)->power = RenderText_Solid(cour, temp2, clrFgBlue);

									me->power = amount;
								}
							break;
							}

						case AGILITY:
							{
								char temp2[10] = "";

								_itoa_s(amount, temp2, 10, 10);

								SDL_FreeSurface(addstatwindow.at(0)->agility);

								if(enableDisable == 1)
									addstatwindow.at(0)->agility = RenderText_Solid(cour, temp2, clrFgBlue);

								else
									addstatwindow.at(0)->agility = RenderText_Solid(cour, temp2, clrFgBlack);

								me->agility = amount;
							break;
							}

						case INTT:
							{
								char temp2[10] = "";

								_itoa_s(amount, temp2, 10, 10);

								SDL_FreeSurface(addstatwindow.at(0)->intt);

								if(enableDisable == 2)
									addstatwindow.at(0)->intt = RenderText_Solid(cour, temp2, clrFgBlack);

								else
									addstatwindow.at(0)->intt = RenderText_Solid(cour, temp2, clrFgBlue);

								me->intt = amount;
							break;
							}

						default: Log("Unknown attribute header: 0x%d.", cpak[4]);
							break;
					}
				}
				
				else
				{
					for(unsigned int i = 0; i < cv.size(); i++)
					{
						if(cv.at(i)->id == id)
						{
							switch(cpak[4])
							{
								case SPEED: case SPEEDMINUS:
									{
										if(amount <= 0)
											amount = 1;

										cv.at(i)->speed = amount;

										cv.at(i)->pixSec = (cv.at(i)->speed * 62);
									break;
									}
							}
						}

					}
				}
			break;
			}

		case 0x31:
			{
				int pos = 3;
				
				int id = atoi(getString(cpak, (pos + 1), (int)cpak[pos]).c_str());
				pos += ((int)cpak[pos] + 1);

				int strLen = atoi(getString(cpak, (pos + 1), (int)cpak[pos]).c_str());
				pos += ((int)cpak[pos] + 1);

				string dialog = getString(cpak, pos, strLen);
				pos += strLen;

				for(unsigned int i = 0; i < npc.size(); i++)
				{
					if(id == npc.at(i)->id)
					{
						npc.at(i)->showDialog = true;
						npc.at(i)->dialog = dialog;

						int numOfDialogButton = cpak[pos];
						pos += 1;

						if(npc.at(i)->npct.size() > 0)
						{
							for(unsigned int u = 0; u < npc.at(i)->npct.size(); u++)
								SDL_FreeSurface(npc.at(i)->npct.at(u).text);

							npc.at(i)->npct.clear();
						}

						if(npc.at(i)->diab.size() > 0)
						{
							for(unsigned int u = 0; u < npc.at(i)->diab.size(); u++)
							{
								SDL_FreeSurface(npc.at(i)->diab.at(u).btn);
								SDL_FreeSurface(npc.at(i)->diab.at(u).tSur);
							}

							npc.at(i)->diab.clear();
						}

						for(int z = 0; z < numOfDialogButton; z++)
						{
							MenuButton m = MenuButton();

							strcpy_s(m.text, getString(cpak, (pos + 1), (int)cpak[pos]).c_str());
							pos += ((int)cpak[pos] + 1);

							m.tSur = RenderText_Solid(cour, m.text, clrFgBlack);
							m.btn = loadimage("Data/Interface/Clean-Button.png", true, 255, 255, 255);

							npc.at(i)->diab.push_back(m);
						}

						int cY = 29;

						NPCText fi = NPCText();

						fi.x = 25;
						fi.y = cY;
						
						SDL_Surface *fullMe = RenderText_Solid(cour, dialog.c_str(), clrFgBlack);

						if(fullMe->w > 395)
						{
							if(fullMe != NULL)
								SDL_FreeSurface(fullMe);

							fullMe = NULL;

							vector<string> npcString;

							string tempString = "";
							unsigned int readPos = 0;

							for(unsigned int n = 0; n <= dialog.length(); n++)
							{
								for(unsigned int t = readPos; t < n; t++)
									tempString += dialog[t];

								if(fullMe != NULL)
									SDL_FreeSurface(fullMe);

								fullMe = NULL;

								fullMe = RenderText_Solid(cour, tempString.c_str(), clrFgBlack);

								if(fullMe == NULL)
								{
								}

								else
								{
									if(fullMe->w > 395)
									{
										npcString.push_back(tempString);

										readPos = n;
									}

									if(n == dialog.length() && readPos != n)
										npcString.push_back(tempString);
								}

								tempString = "";
							}

							for(unsigned int s = 0; s < npcString.size(); s++)
							{
								NPCText nt = NPCText();

								nt.x = 25;
								nt.y = cY;
								nt.text = RenderText_Solid(cour, npcString.at(s).c_str(), clrFgBlack);

								npc.at(i)->npct.push_back(nt);

								cY += 17;
							}
						}

						else
						{
							fi.text = fullMe;

							npc.at(i)->npct.push_back(fi);
						}

						break;
					}
				}
			break;
			}

		case 0x33:
			{
				if(cpak[3] == 3)
				{
					for(unsigned int i = 0; i < npc.size(); i++)
					{
						if(npc.at(i)->showMenu)
							npc.at(i)->showMenu = false;
						if(npc.at(i)->showDialog)
						{
							for(unsigned int y = 0; y < npc.at(i)->npct.size(); y++)
								SDL_FreeSurface(npc.at(i)->npct.at(y).text);

							npc.at(i)->npct.clear();

							npc.at(i)->showDialog = false;
						}
					}
				}
			break;
			}

		case 0x34:
			{
				int slot = cpak[3];

				int pos = 4;

				char temp[10] = "";

				int stack = atoi(getString(cpak, (pos + 1), (int)cpak[pos]).c_str());
				pos += ((int)cpak[pos] + 1);

				int id = atoi(getString(cpak, (pos + 1), (int)cpak[pos]).c_str());
				pos += ((int)cpak[pos] + 1);

				int val = atoi(getString(cpak, (pos + 1), (int)cpak[pos]).c_str());
				pos += ((int)cpak[pos] + 1);

				int effect = atoi(getString(cpak, (pos + 1), (int)cpak[pos]).c_str());
				pos += ((int)cpak[pos] + 1);

				bool notice = false;

				if(cpak[pos] == 1)
					notice = true;

				pos++;

				int maxstack = atoi(getString(cpak, (pos + 1), (int)cpak[pos]).c_str());
				pos += ((int)cpak[pos] + 1);

				char full[100] = "You received ";

				if(stack > 1)
				{
					_itoa_s(stack, temp, 10, 10);

					strcat_s(full, temp);
					strcat_s(full, " ");
				}

				strcat_s(full, getItemName(id).c_str());
				strcat_s(full, ".");

				if(notice)
				{
					int penyas = 0;

					for(unsigned int i = 0; i < nv.size(); i++)
					{
						if(nv.at(i)->penya)
							penyas++;
					}

					if(penyas >= 6)
					{
						for(unsigned int i = 0; i < nv.size(); i++)
						{
							SDL_FreeSurface(nv.at(i)->msg);

							delete nv.at(i);
							nv.erase(nv.begin() + i);

							for(unsigned int r = 0; r < nv.size(); r++)
							{
								if(nv.at(r)->penya)
									nv.at(r)->y -= 17;
							}

							break;
						}

						Notifer *n = new Notifer();

						n->sTime = 0;
						n->msg = RenderText_Solid(courMedium, full, clrFg);
						n->dTime = (getTime(true) + 7000);
						n->x = ((screen->w / 2) - (n->msg->w / 2));
						n->y = (((screen->h / 2) + 70) + (6 * 17));
						n->penya = true;

						nv.push_back(n);
					}

					else
					{
						Notifer *n = new Notifer();

						n->sTime = 0;
						n->msg = RenderText_Solid(courMedium, full, clrFg);
						n->dTime = (getTime(true) + 7000);
						n->x = ((screen->w / 2) - (n->msg->w / 2));
						n->y = (((screen->h / 2) + 70) + ((penyas + 1) * 17));
						n->penya = true;

						nv.push_back(n);
					}
				}

				Item *item = new Item();

				item->id = id;
				item->stack = stack;
				item->value = val;
				item->maxstack = maxstack;
				item->effect = effect;

				item->pic = getIconSurface(id);
				if(item->pic == NULL)
					Log("Error, could not get surface from memory (Item: %d).", id);

				_itoa_s(stack, temp, 10, 10);

				item->amount = RenderText_Blended(courBold, temp, clrFgBlue);

				if(inv->items[(slot - 1)] == NULL)
				{
					item->slot = slot;
					inv->items[(slot - 1)] = item;

					int rad = 1;

					int cX = (inv->x + 17);
					int cY = (inv->y + 284);

					for(int i = 0; i < 25; i++)
					{
						if((slot - 1) == i)
						{
							inv->items[i]->x = cX;
							inv->items[i]->y = cY;

							break;
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

				UpdateTaskbar(NEWITEM, id, slot);

			break;
			}

		case 0x35:
			{
				if(cpak[3] == 2)
				{
					Item *temp = inv->items[(cpak[5] - 1)];

					int slot1X = inv->items[(cpak[4] - 1)]->x;
					int slot1Y = inv->items[(cpak[4] - 1)]->y;
					int slot2X = inv->items[(cpak[5] - 1)]->x;
					int slot2Y = inv->items[(cpak[5] - 1)]->y;

					temp->x = slot1X;
					temp->y = slot1Y;
					inv->items[(cpak[4] - 1)]->x = slot2X;
					inv->items[(cpak[4] - 1)]->y = slot2Y;

					inv->items[(cpak[4] - 1)]->slot = cpak[5];
					inv->items[(cpak[5] - 1)]->slot = cpak[4];

					inv->items[(cpak[5] - 1)] = inv->items[(cpak[4] - 1)];
					inv->items[(cpak[4] - 1)] = temp;

					for(int i = 0; i < 9; i++)
					{
						if(task->pic[i]->enable)
						{
							if(task->pic[i]->slot == cpak[4] && task->pic[i]->type == INVENTORYITEM && !task->pic[i]->eqinv)
								task->pic[i]->slot = cpak[5];

							else if(task->pic[i]->slot == cpak[5] && task->pic[i]->type == INVENTORYITEM && !task->pic[i]->eqinv)
								task->pic[i]->slot = cpak[4];
						}
					}
				}

				else if(cpak[3] == 1)
				{
					inv->items[(cpak[5] - 1)] = inv->items[(cpak[4] - 1)];
					inv->items[(cpak[4] - 1)] = NULL;

					inv->items[(cpak[5] - 1)]->slot = cpak[5];

					for(int i = 0; i < 9; i++)
					{
						if(task->pic[i]->enable)
						{
							if(task->pic[i]->slot == cpak[4] && task->pic[i]->type == INVENTORYITEM && !task->pic[i]->eqinv)
								task->pic[i]->slot = cpak[5];
						}
					}

					int rad = 1;

					int cX = (inv->x + 17);
					int cY = (inv->y + 284);

					for(int i = 0; i < 25; i++)
					{
						if((cpak[5] - 1) == i)
						{
							inv->items[(cpak[5] - 1)]->x = cX;
							inv->items[(cpak[5] - 1)]->y = cY;

							break;
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

			break;
			}

		case 0x36:
			{
				int slot = cpak[3];

				SDL_FreeSurface(inv->items[(slot - 1)]->amount);

				int id = inv->items[(slot - 1)]->id;

				delete inv->items[(slot - 1)];
				inv->items[(slot - 1)] = NULL;

				UpdateTaskbar(REMOVEITEM, id, slot);
			break;
			}

		case 0x39:
			{
				if(cpak[3] == 1)
				{
					int pos = 4;

					int dropss = atoi(getString(cpak, (pos + 1), (int)cpak[pos]).c_str());
					pos += ((int)cpak[pos] + 1);

					for(int i = 0; i < dropss; i++)
					{
						int itemid = cpak[pos];
						pos++;
						int itemvalue = cpak[pos];
						pos++;

						int id = atoi(getString(cpak, (pos + 1), (int)cpak[pos]).c_str());
						pos += ((int)cpak[pos] + 1);

						int x = atoi(getString(cpak, (pos + 1), (int)cpak[pos]).c_str());
						pos += ((int)cpak[pos] + 1);

						int y = atoi(getString(cpak, (pos + 1), (int)cpak[pos]).c_str());
						pos += ((int)cpak[pos] + 1);

						Drop *drop = new Drop();

						drop->id = id;
						drop->itemid = itemid;
						drop->itemvalue = itemvalue;
						drop->x = x;
						drop->y = y;
						drop->cX = 0;
						drop->cY = 0;

						drop->pic = getIconSurface(itemid);
						if(drop->pic == NULL)
							Log("Error, could not get surface from memory (Item: %d).", itemid);

						if((drop->x >= camShowX && camShowToX >= drop->x) && (drop->y >=camShowY && camShowToY >= drop->y))
						{
							int t = screen->w;
							int t2 = screen->h;

							drop->cX = (camShowToX - drop->x);
							drop->cY = (camShowToY - drop->y);

							t -= drop->cX;
							t2 -= drop->cY;

							drop->cX = t;
							drop->cY = t2;

							if(!drop->inWin)
								drop->inWin = true;
						}

						else
						{
							if(drop->inWin)
								drop->inWin = false;
						}

						drops.push_back(drop);
					}
				}

				else if(cpak[3] == 2)
				{
					int deleteid = atoi(getString(cpak, 5, (int)cpak[4]).c_str());

					for(unsigned int i = 0; i < drops.size(); i++)
					{
						if(drops.at(i)->id == deleteid)
						{
							delete drops.at(i);
							drops.erase(drops.begin() + i);

							break;
						}
					}
				}
			break;
			}

		case 0x40:
			{
				int pos = 3;

				int gold = atoi(getString(cpak, 4, (int)cpak[3]).c_str());
				pos += ((int)cpak[pos] + 1);

				int added = atoi(getString(cpak, (pos + 1), (int)cpak[pos]).c_str());
				pos += ((int)cpak[pos] + 1);

				bool notice = false;

				if(cpak[pos] == 1)
					notice = true;

				me->gold = gold;
				inv->gold = gold;

				char temp[20] = "";
				char full[100] = "You got ";
				_itoa_s(gold, temp, 20, 10);

				SDL_FreeSurface(inv->golds);
				inv->golds = RenderText_Blended(courMedium, temp, clrFgBlack);

				_itoa_s(added, temp, 20, 10);

				strcat_s(full, temp);
				strcat_s(full, " Dinero (Total: ");

				_itoa_s(gold, temp, 20, 10);

				strcat_s(full, temp);
				strcat_s(full, " Dinero");
				strcat_s(full, ").");

				if(notice)
				{
					int penyas = 0;

					for(unsigned int i = 0; i < nv.size(); i++)
					{
						if(nv.at(i)->penya)
							penyas++;
					}

					if(penyas >= 6)
					{
						for(unsigned int i = 0; i < nv.size(); i++)
						{
							SDL_FreeSurface(nv.at(i)->msg);

							delete nv.at(i);
							nv.erase(nv.begin() + i);

							for(unsigned int r = 0; r < nv.size(); r++)
							{
								if(nv.at(r)->penya)
									nv.at(r)->y -= 17;
							}

							break;
						}

						Notifer *n = new Notifer();

						n->sTime = 0;
						n->msg = RenderText_Solid(courMedium, full, clrFg);
						n->dTime = (getTime(true) + 7000);
						n->x = ((screen->w / 2) - (n->msg->w / 2));
						n->y = (((screen->h / 2) + 70) + (6 * 17));
						n->penya = true;

						nv.push_back(n);
					}

					else
					{
						Notifer *n = new Notifer();

						n->sTime = 0;
						n->msg = RenderText_Solid(courMedium, full, clrFg);
						n->dTime = (getTime(true) + 7000);
						n->x = ((screen->w / 2) - (n->msg->w / 2));
						n->y = (((screen->h / 2) + 70) + ((penyas + 1) * 17));
						n->penya = true;

						nv.push_back(n);
					}
				}
			break;
			}

		case 0x41:
			{
				int pos = 4;
				bool notice = false;

				int slot = cpak[3];

				int newav = atoi(getString(cpak, 5, (int)cpak[4]).c_str());
				pos += ((int)cpak[4] + 1);

				int oldav = atoi(getString(cpak, (pos + 1), (int)cpak[pos]).c_str());
				pos += ((int)cpak[pos] + 1);

				if(cpak[pos] == 1)
					notice = true;

				char temp[10] = "";

				SDL_FreeSurface(inv->items[(slot - 1)]->amount);

				inv->items[(slot - 1)]->stack = newav;

				_itoa_s(newav, temp, 10, 10);

				inv->items[(slot - 1)]->amount = RenderText_Blended(courBold, temp, clrFgBlue);

				UpdateTaskbar(NEWSTACK, inv->items[(slot - 1)]->id, slot);

				char full[100] = "You received ";

				if(oldav > 1)
				{
					_itoa_s(oldav, temp, 10, 10);

					strcat_s(full, temp);
					strcat_s(full, " ");
				}

				strcat_s(full, getItemName(inv->items[(slot - 1)]->id).c_str());
				strcat_s(full, ".");

				if(notice)
				{
					int penyas = 0;

					for(unsigned int i = 0; i < nv.size(); i++)
					{
						if(nv.at(i)->penya)
							penyas++;
					}

					if(penyas >= 6)
					{
						for(unsigned int i = 0; i < nv.size(); i++)
						{
							SDL_FreeSurface(nv.at(i)->msg);

							delete nv.at(i);
							nv.erase(nv.begin() + i);

							for(unsigned int r = 0; r < nv.size(); r++)
							{
								if(nv.at(r)->penya)
									nv.at(r)->y -= 17;
							}

							break;
						}

						Notifer *n = new Notifer();

						n->sTime = 0;
						n->msg = RenderText_Solid(courMedium, full, clrFg);
						n->dTime = (getTime(true) + 7000);
						n->x = ((screen->w / 2) - (n->msg->w / 2));
						n->y = (((screen->h / 2) + 70) + (6 * 17));
						n->penya = true;

						nv.push_back(n);
					}

					else
					{
						Notifer *n = new Notifer();

						n->sTime = 0;
						n->msg = RenderText_Solid(courMedium, full, clrFg);
						n->dTime = (getTime(true) + 7000);
						n->x = ((screen->w / 2) - (n->msg->w / 2));
						n->y = (((screen->h / 2) + 70) + ((penyas + 1) * 17));
						n->penya = true;

						nv.push_back(n);
					}
				}
			break;
			}

		case 0x42:
			{
				int pos = 3;

				int dmgNum = atoi(getString(cpak, (pos + 1), (int)cpak[pos]).c_str());
				pos += ((int)cpak[pos] + 1);

				for(int d = 0; d < dmgNum; d++)
				{
					int type = cpak[pos];
					int crit = cpak[(pos + 1)];

					pos += 2;

					char temp[10] = "";

					int id = atoi(getString(cpak, (pos + 1), (int)cpak[pos]).c_str());
					pos += ((int)cpak[pos] + 1);

					int dmg2 = atoi(getString(cpak, (pos + 1), (int)cpak[pos]).c_str());
					pos += ((int)cpak[pos] + 1);

					int mobproc = cpak[pos];
					pos++;

					_itoa_s(dmg2, temp, 10, 10);

					Damage dmg = Damage();

					dmg.deletetime = (getTime(true) + 700);
					dmg.move.start(true);

					if(type == 1)
					{
						if(id == me->id)
						{
							if(crit == 2)
								dmg.dmg = RenderText_Solid(courBold, temp, clrFgGreen);

							else if(crit == 3)
								dmg.dmg = RenderText_Solid(courBold, temp, clrFgCyan);

							else
								dmg.dmg = RenderText_Solid(courBold, temp, clrFgRed);

							dmg.x = (float)((cameraX + (me->my->w / 2) - (dmg.dmg->w / 2)));
							dmg.y = (float)((cameraY + (me->my->h / 2) - (dmg.dmg->h / 2)));

							dmgs.push_back(dmg);
						}

						else
						{
							for(unsigned int i = 0; i < cv.size(); i++)
							{
								if(id == cv.at(i)->id && cv.at(i)->inWin)
								{
									if(crit == 1)
										dmg.dmg = RenderText_Solid(courBold, temp, clrFgRed);
									else if(crit == 2)
										dmg.dmg = RenderText_Solid(courBold, temp, clrFgGreen);
									else if(crit == 3)
										dmg.dmg = RenderText_Solid(courBold, temp, clrFgCyan);
									else
										dmg.dmg = RenderText_Solid(courBold, temp, clrFgBlue);

									dmg.x = (float)((cv.at(i)->cX + (cv.at(i)->my->w / 2) - (dmg.dmg->w / 2)));
									dmg.y = (float)((cv.at(i)->cY + (cv.at(i)->my->h / 2) - (dmg.dmg->h / 2)));

									dmgs.push_back(dmg);

									cv.at(i)->proc = mobproc;

									break;
								}
							}
						}
					}

					else if(type == 2)
					{
						for(unsigned int i = 0; i < cm.size(); i++)
						{
							if(id == cm.at(i)->mId && cm.at(i)->inWin)
							{
								if(crit == 1)
									dmg.dmg = RenderText_Solid(courBold, temp, clrFgRed);
								else if(crit == 2)
									dmg.dmg = RenderText_Solid(courBold, temp, clrFgGreen);
								else if(crit == 3)
									dmg.dmg = RenderText_Solid(courBold, temp, clrFgCyan);
								else
									dmg.dmg = RenderText_Solid(courBold, temp, clrFgBlue);

								dmg.x = (float)((cm.at(i)->cX + (cm.at(i)->m->w / 2) - (dmg.dmg->w / 2)));
								dmg.y = (float)((cm.at(i)->cY + (cm.at(i)->m->h / 2) - (dmg.dmg->h / 2)));

								dmgs.push_back(dmg);

								cm.at(i)->proc = mobproc;

								break;
							}
						}
					}
				}
			break;
			}

		case 0x43:
			{
				int pos = 3;

				int monsterId = atoi(getString(cpak, (pos + 1), (int)cpak[pos]).c_str());
				pos += ((int)cpak[pos] + 1);

				int playerId = atoi(getString(cpak, (pos + 1), (int)cpak[pos]).c_str());
				pos += ((int)cpak[pos] + 1);

				for(unsigned int i = 0; i < cm.size(); i++)
				{
					if(cm.at(i)->mId == monsterId)
					{
						cm.at(i)->followId = playerId;
						cm.at(i)->moving = false;

						cm.at(i)->move.start(false);

						break;
					}
				}
			break;
			}

		case 0x44:
			{
				int pos = 3;

				int mos = atoi(getString(cpak, (pos + 1), (int)cpak[pos]).c_str());
				pos += ((int)cpak[pos] + 1);

				for(int i = 0; i < mos; i++)
				{
					int mob1 = atoi(getString(cpak, (pos + 1), (int)cpak[pos]).c_str());
					pos += ((int)cpak[pos] + 1);

					for(unsigned int r = 0; r < cm.size(); r++)
					{
						if(mob1 == cm.at(r)->mId)
						{
							cm.at(r)->moving = false;
							cm.at(r)->followId = 0;

							cm.at(r)->move.start(false);

							break;
						}
					}
				}

			break;
			}

		case 0x45:
			{
				int pos = 3;

				int id = atoi(getString(cpak, (pos + 1), (int)cpak[pos]).c_str());
				pos += ((int)cpak[pos] + 1);

				long disTime = atoi(getString(cpak, (pos + 1), (int)cpak[pos]).c_str());
				pos += ((int)cpak[pos] + 1);

				int x = atoi(getString(cpak, (pos + 1), (int)cpak[pos]).c_str());
				pos += ((int)cpak[pos] + 1);

				int y = atoi(getString(cpak, (pos + 1), (int)cpak[pos]).c_str());

				Animation *struc = GetSkillAnimation(id);
				Animation *huahs = new Animation();

				*huahs = *struc;

				huahs->disTime = (getTime(true) + huahs->disTimeSec);
				huahs->frame = 0;
				huahs->x = x;
				huahs->y = y;

				anis.push_back(huahs);

			break;
			}

		case 0x46:
			{
				int pos = 3;

				int mobs = atoi(getString(cpak, (pos + 1), (int)cpak[pos]).c_str());
				pos += ((int)cpak[pos] + 1);

				for(int y = 0; y < mobs; y++)
				{
					int id = atoi(getString(cpak, (pos + 1), (int)cpak[pos]).c_str());
					pos += ((int)cpak[pos] + 1);

					for(unsigned int i = 0; i < cm.size(); i++)
					{
						if(cm.at(i)->mId == id)
						{
							cm.at(i)->dead = true;
							cm.at(i)->fadeway = false;
							cm.at(i)->fadep = 255;
							cm.at(i)->fades = true;

							break;
						}
					}
				}

			break;
			}

		case 0x47:
			{
				int pos = 3;

				int mobs = atoi(getString(cpak, (pos + 1), (int)cpak[pos]).c_str());
				pos += ((int)cpak[pos] + 1);

				for(int y = 0; y < mobs; y++)
				{
					int monId = atoi(getString(cpak, (pos + 1), (int)cpak[pos]).c_str());
					pos += ((int)cpak[pos] + 1);

					int playId = atoi(getString(cpak, (pos + 1), (int)cpak[pos]).c_str());
					pos += ((int)cpak[pos] + 1);

					int xMon = atoi(getString(cpak, (pos + 1), (int)cpak[pos]).c_str());
					pos += ((int)cpak[pos] + 1);

					int yMon = atoi(getString(cpak, (pos + 1), (int)cpak[pos]).c_str());
					pos += ((int)cpak[pos] + 1);

					for(unsigned int i = 0; i < cm.size(); i++)
					{
						if(cm.at(i)->mId == monId)
						{
							cm.at(i)->followId = playId;
							cm.at(i)->moving = false;
							cm.at(i)->x = (float)xMon;
							cm.at(i)->y = (float)yMon;
							cm.at(i)->beforeX = (float)xMon;
							cm.at(i)->beforeY = (float)yMon;

							cm.at(i)->move.start(false);

							break;
						}
					}
				}

			break;
			}

		case 0x48:
			{
				int id = atoi(getString(cpak, 4, (int)cpak[3]).c_str());

				if(task->pic[(id - 1)]->enable)
				{
					task->pic[(id - 1)]->enable = false;

					SDL_FreeSurface(task->pic[(id - 1)]->stack);
					task->pic[(id - 1)]->stack = NULL;
				}

			break;
			}

		case 0x49:
			{
				int pos = 3;

				int tslot = atoi(getString(cpak, (pos + 1), (int)cpak[pos]).c_str());
				pos += ((int)cpak[pos] + 1);

				int type = atoi(getString(cpak, (pos + 1), (int)cpak[pos]).c_str());
				pos += ((int)cpak[pos] + 1);

				int tpslot = atoi(getString(cpak, (pos + 1), (int)cpak[pos]).c_str());
				pos += ((int)cpak[pos] + 1);

				if(type == INVENTORYITEM)
				{
					char tempssItem[20];
					_itoa_s(inv->items[(tpslot - 1)]->stack, tempssItem, 20, 10);

					task->pic[(tslot - 1)]->item = inv->items[(tpslot - 1)]->pic;
					task->pic[(tslot - 1)]->slot = tpslot;

					if(getItemStack(inv->items[(tpslot - 1)]->id) > 1)
						task->pic[(tslot - 1)]->stack = getInventoryStackPic(inv->items[(tpslot - 1)]->id);

					else
						task->pic[(tslot - 1)]->stack = RenderText_Blended(courBold, tempssItem, clrFgBlue);

					task->pic[(tslot - 1)]->itemX = ((task->pic[(tslot - 1)]->x + 20) - (task->pic[(tslot - 1)]->item->w / 2));
					task->pic[(tslot - 1)]->itemY = ((task->pic[(tslot - 1)]->y + 15) - (task->pic[(tslot - 1)]->item->h / 2));

					task->pic[(tslot - 1)]->type = INVENTORYITEM;
					task->pic[(tslot - 1)]->itemid = inv->items[(tpslot - 1)]->id;
				}

				else
				{
					task->pic[(tslot - 1)]->slot = tpslot;
					task->pic[(tslot - 1)]->type = type;
				}

				task->pic[(tslot - 1)]->enable = true;
				task->pic[(tslot - 1)]->eqinv = false;

				if(task->pic[(tslot - 1)]->type != INVENTORYITEM)
				{
					task->pic[(tslot - 1)]->item = getSkill(task->pic[tslot - 1]->slot)->pic;//skills[(task->pic[(tslot - 1)]->slot - 1)].pic;
					task->pic[(tslot - 1)]->stack = NULL;

					task->pic[(tslot - 1)]->itemX = ((task->pic[(tslot - 1)]->x + 20) - (task->pic[(tslot - 1)]->item->w / 2));
					task->pic[(tslot - 1)]->itemY = ((task->pic[(tslot - 1)]->y + 15) - (task->pic[(tslot - 1)]->item->h / 2));
				}
			break;
			}

		case 0x51:
			{
				int ty = cpak[3];
				int id = cpak[4];
				int lvl = cpak[5];

				try
				{
					Skill *mySkill = getMySkill(id);

					if(mySkill == NULL)
					{
						Log("Skill was not found at 0x51: %d", id);
					}

					else
					{
						if(mySkill->level != lvl)
						{
							SDL_FreeSurface(mySkill->text);
							SetSkillLevel(lvl, id);
						}
					}

					/*
					if(skills[(id - 1)].level != lvl)
					{
						skills[(id - 1)].level = lvl;

						char temp[10] = "";
						_itoa_s(lvl, temp, 10, 10);

						SDL_FreeSurface(skills[(id - 1)].text);

						skills[(id - 1)].text = RenderText_Solid(cour, temp, clrFgBlack);
					}
					*/
				}

				catch(...)
				{
					Log("Error at ::Process() switcher 0x51.");
				}
			break;
			}

		case 0x52:
			{
				int points = atoi(getString(cpak, 4, (int)cpak[3]).c_str());
				char temp[10] = "";

				SDL_FreeSurface(addstatwindow.at(0)->pkpoints);

				_itoa_s(points, temp, 10, 10);
				
				addstatwindow.at(0)->pkpoints = RenderText_Solid(cour, temp, clrFgBlack);
			break;
			}

		case 0x54:
			{
			break;
			}

		case 0x55: PacketIsOnline(1);
			break;

		case 0x56:
			{
				int pos = 4;

				bool goldtrue = getBool(cpak[3]);

				int gold = atoi(getString(cpak, (pos + 1), (int)cpak[pos]).c_str());
				pos += ((int)cpak[pos] + 1);

				int added = atoi(getString(cpak, (pos + 1), (int)cpak[pos]).c_str());
				pos += ((int)cpak[pos] + 1);

				string name = getString(cpak, (pos + 1), (int)cpak[pos]);

				char temp[20] = "";
				char full[100] = "You ";

				if(goldtrue)
					strcat_s(full, "got ");
				else
					strcat_s(full, "received ");

				_itoa_s(gold, temp, 20, 10);

				strcat_s(full, temp);
				strcat_s(full, " ");

				if(goldtrue)
				{
					strcat_s(full, "(Total: ");
					
					_itoa_s(added, temp, 20, 10);

					strcat_s(full, temp);
					strcat_s(full, " Dinero).");
				}

				else
				{
					strcat_s(full, name.c_str());
					strcat_s(full, ".");
				}

				int penyas = 0;

				for(unsigned int i = 0; i < nv.size(); i++)
				{
					if(nv.at(i)->penya)
						penyas++;
				}

				if(penyas >= 6)
				{
					for(unsigned int i = 0; i < nv.size(); i++)
					{
						SDL_FreeSurface(nv.at(i)->msg);

						delete nv.at(i);
						nv.erase(nv.begin() + i);

						for(unsigned int r = 0; r < nv.size(); r++)
						{
							if(nv.at(r)->penya)
								nv.at(r)->y -= 17;
						}

						break;
					}

					Notifer *n = new Notifer();

					n->sTime = 0;
					n->msg = RenderText_Solid(courMedium, full, clrFg);
					n->dTime = (getTime(true) + 7000);
					n->x = ((screen->w / 2) - (n->msg->w / 2));
					n->y = (((screen->h / 2) + 70) + (6 * 17));
					n->penya = true;

					nv.push_back(n);
				}

				else
				{
					Notifer *n = new Notifer();

					n->sTime = 0;
					n->msg = RenderText_Solid(courMedium, full, clrFg);
					n->dTime = (getTime(true) + 7000);
					n->x = ((screen->w / 2) - (n->msg->w / 2));
					n->y = (((screen->h / 2) + 70) + ((penyas + 1) * 17));
					n->penya = true;

					nv.push_back(n);
				}
			break;
			}

		case 0x57:
			{
				if(cpak[3] == 1)
				{
					botActive = true;

					botType = cpak[4];
				}

				else
				{
					botActive = false;

					botType = 0;
				}

				botTarget = 0;
				currentMark = 0;

				RemoveAllMoveKeys(0);
				SetKeyDeactivate(SDLK_3);
			break;
			}

		case 0x58:
			{
				if(cpak[3] == 1)
				{
					int pos = 4;

					int slot = cpak[pos];
					pos++;

					int idEq = atoi(getString(cpak, (pos + 1), (int)cpak[pos]).c_str());
					pos += ((int)cpak[pos] + 1);

					int value = atoi(getString(cpak, (pos + 1), (int)cpak[pos]).c_str());
					pos += ((int)cpak[pos] + 1);

					Item *e = new Item();

					e->slot = slot;
					e->stack = 1;
					e->maxstack = 1;
					e->value = value;

					switch(slot)
					{
						case 1: e->effect = 4001;
							break;

						case 2: e->effect = 4002;
							break;

						case 3: e->effect = 4003;
							break;

						case 4: e->effect = 4004;
							break;
					}

					e->pic = getIconSurface(idEq);
					if(e->pic == NULL)
						Log("Error, could not get surface from memory (Item: %d).", idEq);

					e->id = idEq;

					e->amount = RenderText_Blended(courBold, "1", clrFgBlue);

					inv->eq[(slot - 1)] = e;

					string o = "Data/Item/";
					char buf[10];

					_itoa_s(idEq, buf, 10, 10);

					switch(cpak[4])
					{
						case 1:
							{
								o += "Weapon-";
								o += buf;
								o += ".png";

								me->weapon = loadimage(o.c_str(), true, 254, 255, 255);
							break;
							}

						case 2:
							{
								o += "Hat-";
								o += buf;
								o += ".png";

								me->hat = loadimage(o.c_str(), true, 254, 255, 255);
							break;
							}

						case 3:
							{
								o += "Suit-";
								o += buf;
								o += ".png";

								me->suit = loadimage(o.c_str(), true, 254, 255, 255);
							break;
							}

						case 4:
							{
								o += "Belt-";
								o += buf;
								o += ".png";

								me->belt = loadimage(o.c_str(), true, 254, 255, 255);
							break;
							}

						default: Log("Unknown equip slot: %d.", cpak[4]);
							break;
					}
				}

				else if(cpak[3] == 2)
				{
					delete inv->eq[(cpak[4] - 1)];
					inv->eq[(cpak[4] - 1)] = NULL;

					switch(cpak[4])
					{
						case 1:
							{
								if(me->weapon != NULL)
									SDL_FreeSurface(me->weapon);

								me->weapon = NULL;
							break;
							}

						case 2:
							{
								if(me->hat != NULL)
									SDL_FreeSurface(me->hat);

								me->hat = NULL;
							break;
							}

						case 3:
							{
								if(me->suit != NULL)
									SDL_FreeSurface(me->suit);

								me->suit = NULL;
							break;
							}

						case 4:
							{
								if(me->belt != NULL)
									SDL_FreeSurface(me->belt);

								me->belt = NULL;
							break;
							}

						default: Log("Unknown equip slot: %d.", cpak[4]);
							break;
					}
				}

				else if(cpak[3] == 3)
				{
					int pos = 4;
					int slot = cpak[pos];
					pos++;

					int id = atoi(getString(cpak, (pos + 1), (int)cpak[pos]).c_str());
					pos += ((int)cpak[pos] + 1);

					int cvId = atoi(getString(cpak, (pos + 1), (int)cpak[pos]).c_str());
					pos += ((int)cpak[pos] + 1);

					for(unsigned int i = 0; i < cv.size(); i++)
					{
						if(cv.at(i)->id == cvId)
						{
							string o = "Data/Item/";
							char buf[10];

							_itoa_s(id, buf, 10, 10);

							switch(slot)
							{
								case 1:
									{
										o += "Weapon-";
										o += buf;
										o += ".png";

										cv.at(i)->weapon = loadimage(o.c_str(), true, 254, 255, 255);
										cv.at(i)->weaponid = id;
									break;
									}

								case 2:
									{
										o += "Hat-";
										o += buf;
										o += ".png";

										cv.at(i)->hat = loadimage(o.c_str(), true, 254, 255, 255);
										cv.at(i)->hatid = id;
									break;
									}

								case 3:
									{
										o += "Suit-";
										o += buf;
										o += ".png";

										cv.at(i)->suit = loadimage(o.c_str(), true, 254, 255, 255);
										cv.at(i)->suitid = id;
									break;
									}

								case 4:
									{
										o += "Belt-";
										o += buf;
										o += ".png";

										cv.at(i)->belt = loadimage(o.c_str(), true, 254, 255, 255);
										cv.at(i)->beltid = id;
									break;
									}

								default: Log("Unknown equip slot: %d.", cpak[4]);
									break;
							}

							break;
						}
					}
				}

				else if(cpak[3] == 4)
				{
					int id = atoi(getString(cpak, 6, (int)cpak[5]).c_str());

					for(unsigned int i = 0; i < cv.size(); i++)
					{
						if(cv.at(i)->id == id)
						{
							switch(cpak[4])
							{
								case 1:
									{
										if(cv.at(i)->weapon != NULL)
											SDL_FreeSurface(cv.at(i)->weapon);

										cv.at(i)->weapon = NULL;
										cv.at(i)->weaponid = 0;
									break;
									}

								case 2:
									{
										if(cv.at(i)->hat != NULL)
											SDL_FreeSurface(cv.at(i)->hat);

										cv.at(i)->hat = NULL;
										cv.at(i)->hatid = 0;
									break;
									}

								case 3:
									{
										if(cv.at(i)->suit != NULL)
											SDL_FreeSurface(cv.at(i)->suit);

										cv.at(i)->suit = NULL;
										cv.at(i)->suitid = 0;
									break;
									}

								case 4:
									{
										if(cv.at(i)->belt != NULL)
											SDL_FreeSurface(cv.at(i)->belt);

										cv.at(i)->belt = NULL;
										cv.at(i)->beltid = 0;
									break;
									}

								default: Log("Unknown equip slot: %d.", cpak[4]);
									break;
							}

							break;
						}
					}
				}
			break;
			}

		case 0x59:
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

				if(spawned)
					pingSurface = RenderText_Solid(cour, pingString.c_str(), clrFg);

				else
					pingSurface = RenderText_Solid(cour, pingString.c_str(), clrFgBlack);

				sentPingPacket = false;
			break;
			}

		case 0x60:
			{
				int pos = 3;

				int mobId = atoi(getString(cpak, (pos + 1), (int)cpak[pos]).c_str());
				pos += ((int)cpak[pos] + 1);

				int param = atoi(getString(cpak, (pos + 1), (int)cpak[pos]).c_str());
				pos += ((int)cpak[pos] + 1);

				int amount = atoi(getString(cpak, (pos + 1), (int)cpak[pos]).c_str());
				pos += ((int)cpak[pos] + 1);

				for(unsigned int i = 0; i < cm.size(); i++)
				{
					if(cm.at(i)->mId == mobId)
					{
						switch(param)
						{
							case SPEEDMINUS: cm.at(i)->speed = amount;
								break;

							case HPDRAIN:
								break;

							default: Log("Unknown param at 0x60: %d.", param);
								break;
						}

						break;
					}
				}
			break;
			}

		case 0x61:
			{
				int pos = 3;

				int nMobs = atoi(getString(cpak, (pos + 1), (int)cpak[pos]).c_str());
				pos += ((int)cpak[pos] + 1);

				for(int i = 0; i < nMobs; i++)
				{
					int mobId = atoi(getString(cpak, (pos + 1), (int)cpak[pos]).c_str());
					pos += ((int)cpak[pos] + 2);

					int hpProc = cpak[(pos - 1)];

					for(unsigned int y = 0; y < cm.size(); y++)
					{
						if(cm.at(y)->mId == mobId)
						{
							cm.at(y)->proc = hpProc;

							break;
						}
					}
				}
			break;
			}

		case 0x62:
			{
				int pos = 3;

				int nMobs = atoi(getString(cpak, (pos + 1), (int)cpak[pos]).c_str());
				pos += ((int)cpak[pos] + 1);

				for(int i = 0; i < nMobs; i++)
				{
					int mobId = atoi(getString(cpak, (pos + 1), (int)cpak[pos]).c_str());
					pos += ((int)cpak[pos] + 2);

					int hpProc = cpak[(pos - 1)];

					for(unsigned int y = 0; y < cv.size(); y++)
					{
						if(cv.at(y)->id == mobId)
						{
							cv.at(y)->proc = hpProc;

							break;
						}
					}
				}
			break;
			}

		case 0x63:
			{
				int type = cpak[3];
				int taskbarslot = cpak[4];
				int oldslot = cpak[5];
				int newslot = cpak[6];

				if(type == 1)
				{
					if(task->pic[(taskbarslot - 1)]->enable)
					{
						task->pic[(taskbarslot - 1)]->slot = newslot;
						task->pic[(taskbarslot - 1)]->eqinv = true;
					}
				}

				else if(type == 2)
				{
					if(task->pic[(taskbarslot - 1)]->enable)
					{
						task->pic[(taskbarslot - 1)]->slot = newslot;
						task->pic[(taskbarslot - 1)]->eqinv = false;
					}
				}
			break;
			}

		case 0x64:
			{
				int pos = 3;

				int npcId = atoi(getString(cpak, (pos + 1), (int)cpak[pos]).c_str());
				pos += ((int)cpak[pos] + 1);

				int amountItem = atoi(getString(cpak, (pos + 1), (int)cpak[pos]).c_str());
				pos += ((int)cpak[pos] + 1);

				NPC *p = NULL;

				for(unsigned int i = 0; i < npc.size(); i++)
				{
					if(npc.at(i)->id == npcId)
					{
						p = npc.at(i);

						break;
					}
				}

				if(p != NULL)
				{
					p->showShop = true;
					sw->Clean();
					sw->enable = true;
					sw->npcId = p->id;
					sw->cMark = 1;

					string shopName = p->name;
					
					shopName += "'s Shop";

					sw->shopName = RenderText_Solid(cour, shopName.c_str(), clrFgBlack);
					sw->currQty = RenderText_Solid(cour, "1", clrFgBlack);
					sw->totalPrice = RenderText_Solid(cour, "0", clrFgBlack);

					for(int i = 0; i < amountItem; i++)
					{
						int itemId = atoi(getString(cpak, (pos + 1), (int)cpak[pos]).c_str());
						pos += ((int)cpak[pos] + 1);

						if(i < 20)
						{
							sw->si[i].itemId = itemId;
							sw->si[i].itemQty = 1;
							sw->si[i].pic = getIconSurface(itemId);
							sw->si[i].itemPrice = getItemPrice(itemId);
						}
					}

					if(sw->si[(sw->cMark - 1)].itemId != 0)
					{
						char buf[10] = "";

						_itoa_s(sw->si[(sw->cMark - 1)].itemPrice, buf, 10, 10);

						sw->currPrice = RenderText_Solid(cour, buf, clrFgBlack);
					}

					else
						sw->currPrice = RenderText_Solid(cour, "0", clrFgBlack);
				}

			break;
			}

		case 0x65:
			{
				int pos = 3;

				int itemId = atoi(getString(cpak, (pos + 1), (int)cpak[pos]).c_str());
				pos += ((int)cpak[pos] + 1);

				int itemQty = atoi(getString(cpak, (pos + 1), (int)cpak[pos]).c_str());

				if(sw->enable)
				{
					for(int i = 0; i < 20; i++)
					{
						if(sw->cart[i].itemId == 0)
						{
							char tempborr[10] = "";

							_itoa_s(itemQty, tempborr, 10, 10);

							sw->cart[i].itemId = itemId;
							sw->cart[i].itemPrice = getItemPrice(itemId);
							sw->cart[i].itemQty = itemQty;
							sw->cart[i].pic = getIconSurface(itemId);
							sw->cart[i].picStack = RenderText_Blended(courBold, tempborr, clrFgBlue);

							if(sw->totalPrice != NULL)
								SDL_FreeSurface(sw->totalPrice);

							char buf[10] = "";
							int fullPrice = 0;

							for(int u = 0; u < 20; u++)
							{
								if(sw->cart[u].itemId != 0)
									fullPrice += (sw->cart[u].itemQty * sw->cart[u].itemPrice);
							}

							_itoa_s(fullPrice, buf, 10, 10);

							sw->totalPrice = RenderText_Solid(cour, buf, clrFgBlack);

							break;
						}
					}
				}
			break;
			}

		case 0x69:
			{
				int itemId = atoi(getString(cpak, 4, (int)cpak[3]).c_str());

				if(sw->enable && sw->cart[(itemId - 1)].itemId != 0)
				{
					sw->cart[(itemId - 1)] = ShopItem();
					sw->SetupPos();

					for(int i = itemId; i < 20; i++)
					{
						if(sw->cart[i].itemId != 0)
						{
							sw->cart[(i - 1)].itemId = sw->cart[i].itemId;
							sw->cart[(i - 1)].itemPrice = sw->cart[i].itemPrice;
							sw->cart[(i - 1)].itemQty = sw->cart[i].itemQty;
							sw->cart[(i - 1)].pic = sw->cart[i].pic;
							sw->cart[(i - 1)].picStack = sw->cart[i].picStack;

							sw->cart[i] = ShopItem();
							sw->SetupPos();
						}
					}

					if(sw->totalPrice != NULL)
						SDL_FreeSurface(sw->totalPrice);

					char buf[10] = "";
					int fullPrice = 0;

					for(int u = 0; u < 20; u++)
					{
						if(sw->cart[u].itemId != 0)
							fullPrice += (sw->cart[u].itemQty * sw->cart[u].itemPrice);
					}

					_itoa_s(fullPrice, buf, 10, 10);

					sw->totalPrice = RenderText_Solid(cour, buf, clrFgBlack);
				}
			break;
			}

		case 0x71:
			{
				int pos = 4;

				bool followOrNot = getBool(cpak[3]);

				int followerId = atoi(getString(cpak, (pos + 1), (int)cpak[pos]).c_str());
				pos += ((int)cpak[pos] + 1);

				int targetId = atoi(getString(cpak, (pos + 1), (int)cpak[pos]).c_str());
				pos += ((int)cpak[pos] + 1);

				int xFol = atoi(getString(cpak, (pos + 1), (int)cpak[pos]).c_str());
				pos += ((int)cpak[pos] + 1);

				int yFol = atoi(getString(cpak, (pos + 1), (int)cpak[pos]).c_str());
				pos += ((int)cpak[pos] + 1);

				int xTar = atoi(getString(cpak, (pos + 1), (int)cpak[pos]).c_str());
				pos += ((int)cpak[pos] + 1);

				int yTar = atoi(getString(cpak, (pos + 1), (int)cpak[pos]).c_str());
				pos += ((int)cpak[pos] + 1);

				bool exist = false;

				if(followOrNot)
				{
					if(targetId == me->id)
						exist = true;

					else
					{
						for(unsigned int i = 0; i < cv.size(); i++)
						{
							if(cv.at(i)->id == targetId)
							{
								cv.at(i)->x = (float)xTar;
								cv.at(i)->y = (float)yTar;

								if((cv.at(i)->x >= camShowX && camShowToX >= cv.at(i)->x) && (cv.at(i)->y >=camShowY && camShowToY >= cv.at(i)->y))
								{
									int t = screen->w;
									int t2 = screen->h;

									cv.at(i)->cX = (camShowToX - (int)cv.at(i)->x);
									cv.at(i)->cY = (camShowToY - (int)cv.at(i)->y);

									t -= cv.at(i)->cX;
									t2 -= cv.at(i)->cY;

									cv.at(i)->cX = t;
									cv.at(i)->cY = t2;

									cv.at(i)->inWin = true;
								}

								else
								{
									if(cv.at(i)->inWin)
										cv.at(i)->inWin = false;
								}

								exist = true;

								break;
							}
						}
					}
				}

				if(exist || !followOrNot)
				{
					if(followerId == me->id)
					{
						if(followOrNot)
						{
							me->followId = targetId;
							me->follow = true;

							move = false;
							currMoveWay = 0;

							me->move.start(false);
						}

						else
						{
							me->followId = 0;
							me->follow = false;
						}
					}

					else
					{
						for(unsigned int i = 0; i < cv.size(); i++)
						{
							if(cv.at(i)->id == followerId)
							{
								if(followOrNot)
								{
									cv.at(i)->followId = targetId;
									cv.at(i)->follow = true;

									cv.at(i)->moving = false;

									cv.at(i)->x = (float)xFol;
									cv.at(i)->y = (float)yFol;

									if((cv.at(i)->x >= camShowX && camShowToX >= cv.at(i)->x) && (cv.at(i)->y >=camShowY && camShowToY >= cv.at(i)->y))
									{
										int t = screen->w;
										int t2 = screen->h;

										cv.at(i)->cX = (camShowToX - (int)cv.at(i)->x);
										cv.at(i)->cY = (camShowToY - (int)cv.at(i)->y);

										t -= cv.at(i)->cX;
										t2 -= cv.at(i)->cY;

										cv.at(i)->cX = t;
										cv.at(i)->cY = t2;

										cv.at(i)->inWin = true;
									}

									else
									{
										if(cv.at(i)->inWin)
											cv.at(i)->inWin = false;
									}

									cv.at(i)->move.start(false);
								}

								else
								{
									cv.at(i)->followId = 0;
									cv.at(i)->follow = false;
								}

								break;
							}
						}
					}
				}
			break;
			}

		case 0x72:
			{
				int pos = 3;

				int playid = atoi(getString(cpak, (pos + 1), (int)cpak[pos]).c_str());
				pos += ((int)cpak[pos] + 1);

				string name = getString(cpak, (pos + 1), (int)cpak[pos]);

				char full[100] = "From: ";

				strcat_s(full, name.c_str());

				PartyInvite *w = new PartyInvite();

				w->win = loadimage("Data/Interface/TradeInvite-Window.png", false, 0, 0, 0);
				w->no = loadimage("Data/Interface/No-Button.png", true, 255, 255, 255);
				w->yes = loadimage("Data/Interface/Yes-Button.png", true, 255, 255, 255);

				if(transWindow)
					SDL_SetAlpha(w->win, SDL_SRCALPHA | SDL_RLEACCEL, 128);

				w->from = RenderText_Solid(cour, full, clrFgBlack);

				strcpy_s(w->name, name.c_str());

				w->nameid = playid;
				w->type = TRADE;

				w->x = 398;
				w->y = 62;

				pi.push_back(w);
			break;
			}

		case 0x73:
			{
				ptrade->Clean();
				ptrade->enable = false;
			break;
			}

		case 0x74:
			{
				for(unsigned int i = 0; i < npc.size(); i++)
				{
					if(npc.at(i)->showShop)
					{
						npc.at(i)->showShop = false;

						sw->Clean();
						sw->enable = false;
					}
				}

				int pos = 3;

				int playId1 = atoi(getString(cpak, (pos + 1), (int)cpak[pos]).c_str());
				pos += ((int)cpak[pos] + 1);

				string name1 = getString(cpak, (pos + 1), (int)cpak[pos]);
				pos += ((int)cpak[pos] + 1);

				int playId2 = atoi(getString(cpak, (pos + 1), (int)cpak[pos]).c_str());
				pos += ((int)cpak[pos] + 1);

				string name2 = getString(cpak, (pos + 1), (int)cpak[pos]);
				pos += ((int)cpak[pos] + 1);

				int whatside = RIGHT;

				ptrade->Clean();

				if(playId1 == me->id)
					whatside = LEFT;

				ptrade->side1 = playId1;
				ptrade->side2 = playId2;

				ptrade->enable = true;
				ptrade->mySide = whatside;

				ptrade->tradeName1 = RenderText_Solid(cour, name1.c_str(), clrFgBlack);
				ptrade->tradeName2 = RenderText_Solid(cour, name2.c_str(), clrFgBlack);

				ptrade->status1 = RenderText_Solid(cour, "Waiting", clrFgBlack);
				ptrade->status2 = RenderText_Solid(cour, "Waiting", clrFgBlack);

				ptrade->myPenya = RenderText_Blended(courMedium, "Dinero: 0", clrFgBlack);
				ptrade->otherPenya = RenderText_Blended(courMedium, "Dinero: 0", clrFgBlack);
			break;
			}

		case 0x75:
			{
				int pos = 3;

				int myid = atoi(getString(cpak, (pos + 1), (int)cpak[pos]).c_str());
				pos += ((int)cpak[pos] + 1);

				int tradeplace = cpak[pos];
				pos += 1;

				int itemid = atoi(getString(cpak, (pos + 1), (int)cpak[pos]).c_str());
				pos += ((int)cpak[pos] + 1);

				int itemstack = atoi(getString(cpak, (pos + 1), (int)cpak[pos]).c_str());
				pos += ((int)cpak[pos] + 1);

				char tempborr[10] = "";

				_itoa_s(itemstack, tempborr, 10, 10);

				if(tradeplace <= 20)
				{
					if(myid == ptrade->side1)
					{
						ptrade->si[(tradeplace - 1)].itemId = itemid;
						ptrade->si[(tradeplace - 1)].itemQty = itemstack;
						ptrade->si[(tradeplace - 1)].pic = getIconSurface(itemid);
						ptrade->si[(tradeplace - 1)].picStack = RenderText_Blended(courBold, tempborr, clrFgBlue);
					}
					
					else
					{
						ptrade->si2[(tradeplace - 1)].itemId = itemid;
						ptrade->si2[(tradeplace - 1)].itemQty = itemstack;
						ptrade->si2[(tradeplace - 1)].pic = getIconSurface(itemid);
						ptrade->si2[(tradeplace - 1)].picStack = RenderText_Blended(courBold, tempborr, clrFgBlue);
					}
				}
			break;
			}

		case 0x76:
			{
				int pos = 3;

				int playId = atoi(getString(cpak, (pos + 1), (int)cpak[pos]).c_str());
				pos += ((int)cpak[pos] + 1);

				int stat = cpak[pos];

				if(playId == ptrade->side1)
				{
					if(ptrade->status1 != NULL)
						SDL_FreeSurface(ptrade->status1);

					switch(stat)
					{
						case 1: ptrade->status1 = RenderText_Solid(cour, "Done", clrFgBlack);
							break;

						case 3: ptrade->status1 = RenderText_Solid(cour, "Adding item", clrFgBlack);
							break;

						case 4: ptrade->status1 = RenderText_Solid(cour, "Waiting", clrFgBlack);
							break;
					}
				}

				else
				{
					if(ptrade->status2 != NULL)
						SDL_FreeSurface(ptrade->status2);

					switch(stat)
					{
						case 1: ptrade->status2 = RenderText_Solid(cour, "Done", clrFgBlack);
							break;

						case 3: ptrade->status2 = RenderText_Solid(cour, "Adding item", clrFgBlack);
							break;

						case 4: ptrade->status2 = RenderText_Solid(cour, "Waiting", clrFgBlack);
							break;
					}
				}
			break;
			}

		case 0x77:
			{
				int pos = 3;

				int playId = atoi(getString(cpak, (pos + 1), (int)cpak[pos]).c_str());
				pos += ((int)cpak[pos] + 1);

				int pen = atoi(getString(cpak, (pos + 1), (int)cpak[pos]).c_str());
				pos += ((int)cpak[pos] + 1);

				char temp[10];
				_itoa_s(pen, temp, 10, 10);

				string fullPenyaString = "Dinero: ";

				fullPenyaString += temp;

				if(playId == ptrade->side1)
				{
					if(ptrade->myPenya != NULL)
						SDL_FreeSurface(ptrade->myPenya);

					ptrade->myPenya = RenderText_Blended(courMedium, fullPenyaString.c_str(), clrFgBlack);
					ptrade->penyaMe = pen;
				}

				else if(playId == ptrade->side2)
				{
					if(ptrade->otherPenya != NULL)
						SDL_FreeSurface(ptrade->otherPenya);

					ptrade->otherPenya = RenderText_Blended(courMedium, fullPenyaString.c_str(), clrFgBlack);
					ptrade->penyaOther = pen;
				}
			break;
			}

		case 0x78:
			{
				tp->enable = true;
				tp->Clean();

				int pos = 3;
				
				int id = atoi(getString(cpak, (pos + 1), (int)cpak[pos]).c_str());
				pos += ((int)cpak[pos] + 1);

				int num = atoi(getString(cpak, (pos + 1), (int)cpak[pos]).c_str());
				pos += ((int)cpak[pos] + 1);

				tp->npcId = id;

				if(getNPCById(id) != NULL)
				{
					char temp[20];
					string telenpc = getNPCById(id)->name;

					telenpc += "'s Teleportation";

					tp->npcText = RenderText_Solid(cour, telenpc.c_str(), clrFgBlack);

					for(int i = 0; i < num; i++)
					{
						string name = getString(cpak, (pos + 1), (int)cpak[pos]);
						pos += ((int)cpak[pos] + 1);

						int price = atoi(getString(cpak, (pos + 1), (int)cpak[pos]).c_str());
						pos += ((int)cpak[pos] + 1);

						TeleportObject to = TeleportObject();

						to.target = name;
						to.cost = price;

						_itoa_s(price, temp, 20, 10);

						to.world = RenderText_Solid(cour, name.c_str(), clrFgBlack);
						to.price = RenderText_Solid(cour, temp, clrFgBlack);

						to.x = 10;
						to.y = (66 + (i * 20));

						tp->tps.push_back(to);
					}
				}

			break;
			}

		case 0x79:
			{
				int id = atoi(getString(cpak, 4, (int)cpak[3]).c_str());

				partyw.at(0)->level = id;

				if(partyw.at(0)->levelSur != NULL)
					SDL_FreeSurface(partyw.at(0)->levelSur);

				partyw.at(0)->levelSur = RenderText_Solid(cour, IntToString(id).c_str(), clrFgBlack);
			break;
			}

		case 0x0A:
			{
				int doMobs = cpak[3];
				int doChar = cpak[4];

				int pos = 5;

				int id = atoi(getString(cpak, 6, (int)cpak[5]).c_str());
				pos += ((int)cpak[pos] + 1);

				int x = atoi(getString(cpak, pos + 1, (int)cpak[pos]).c_str());
				pos += ((int)cpak[pos] + 1);

				int y = atoi(getString(cpak, pos + 1, (int)cpak[pos]).c_str());
				pos += ((int)cpak[pos] + 1);

				for(unsigned int i = 0; i < cm.size(); i++)
				{
					if(!cm.at(i)->fades)
					{
						Bullet *bullet = new Bullet();

						bullet->cX = (float)x;
						bullet->cY = (float)y;
						bullet->startY = x;
						bullet->startX = y;
						bullet->cWay = 0;
						bullet->owner = id;
						bullet->param = 5;
						bullet->move.start(true);
						bullet->target = cm.at(i)->mId;

						if((bullet->cX >= camShowX && camShowToX >= bullet->cX) && (bullet->cY >=camShowY && camShowToY >= bullet->cY))
						{
							int t = screen->w;
							int t2 = screen->h;

							bullet->sX = (camShowToX - (int)bullet->cX);
							bullet->sY = (camShowToY - (int)bullet->cY);

							t -= bullet->sX;
							t2 -= bullet->sY;

							bullet->sX = t;
							bullet->sY = t2;

							bullet->inWin = true;
						}

						else
							bullet->inWin = false;

						bullet->cPic = getSurface(5);

						bullets.push_back(bullet);
					}
				}
			break;
			}

		case 0x0B:
			{
				task->pic[cpak[3]]->cooldsec = atoi(getString(cpak, 5, (int)cpak[4]).c_str());
				task->pic[cpak[3]]->coolDown = GetTickCount() + 100;
				task->pic[cpak[3]]->coold = RenderText_Solid(cour, FloatToString(((float)task->pic[cpak[3]]->cooldsec / 1000.0f)).c_str(), clrFgBlack);
			break;
			}

		case 0x0C:
			{
				int pos = 3;

				int playid = atoi(getString(cpak, (pos + 1), (int)cpak[pos]).c_str());
				pos += ((int)cpak[pos] + 1);

				string name = getString(cpak, (pos + 1), (int)cpak[pos]);

				char full[100] = "From: ";

				strcat_s(full, name.c_str());

				PartyInvite *w = new PartyInvite();

				w->win = loadimage("Data/Interface/FriendInvite-Window.png", false, 0, 0, 0);
				w->no = loadimage("Data/Interface/No-Button.png", true, 255, 255, 255);
				w->yes = loadimage("Data/Interface/Yes-Button.png", true, 255, 255, 255);

				if(transWindow)
					SDL_SetAlpha(w->win, SDL_SRCALPHA | SDL_RLEACCEL, 128);

				w->from = RenderText_Solid(cour, full, clrFgBlack);

				strcpy_s(w->name, name.c_str());

				w->nameid = playid;
				w->type = FRIEND;

				w->x = 398;
				w->y = 62;

				pi.push_back(w);
			break;
			}

		case 0x0D:
			{
				int pos = 4;

				if(cpak[3] == 1)
				{
					string name = getString(cpak, (pos + 1), (int)cpak[pos]).c_str();
					pos += ((int)cpak[pos] + 1);

					int level = atoi(getString(cpak, (pos + 1), (int)cpak[pos]).c_str());
					pos += ((int)cpak[pos] + 1);

					int job = atoi(getString(cpak, (pos + 1), (int)cpak[pos]).c_str());
					pos += ((int)cpak[pos] + 1);

					int id = atoi(getString(cpak, (pos + 1), (int)cpak[pos]).c_str());
					pos += ((int)cpak[pos] + 1);

					bool onoff = getBool(cpak[pos]);
					pos++;

					Friend fri = Friend();

					fri.name = name;
					fri.level = level;
					fri.job = job;
					fri.id = id;
					fri.status = onoff;

					fri.namep = RenderText_Solid(courMedium, fri.name.c_str(), clrFgBlack);
					fri.levelp = RenderText_Solid(courMedium, IntToString(level).c_str(), clrFgBlack);

					if(!fri.status)
					{
						SDL_SetAlpha(fri.namep, SDL_SRCALPHA | SDL_RLEACCEL, 128);
						SDL_SetAlpha(fri.levelp, SDL_SRCALPHA | SDL_RLEACCEL, 128);
					}

					MenuButton mRemove = MenuButton();
					MenuButton mParty = MenuButton();
					MenuButton mMessage = MenuButton();

					strcpy_s(mMessage.text, "Send message");
					strcpy_s(mParty.text, "Invite to party");
					strcpy_s(mRemove.text, "Remove friend");

					mParty.tSur = RenderText_Solid(courMedium, mParty.text, clrFgBlack);
					mParty.btn = loadimage("Data/Interface/Menu-Window.png", true, 255, 255, 255);

					mRemove.tSur = RenderText_Solid(courMedium, mRemove.text, clrFgBlack);
					mRemove.btn = loadimage("Data/Interface/Menu-Window.png", true, 255, 255, 255);

					mMessage.tSur = RenderText_Solid(courMedium, mMessage.text, clrFgBlack);
					mMessage.btn = loadimage("Data/Interface/Menu-Window.png", true, 255, 255, 255);

					fri.menu.push_back(mMessage);
					fri.menu.push_back(mParty);
					fri.menu.push_back(mRemove);

					fw->tps.push_back(fri);
					fw->UpdateNames();
					fw->startY = fw->y + 65;
				}

				else if(cpak[3] == 2)
				{
					string name = getString(cpak, (pos + 1), (int)cpak[pos]).c_str();
					pos += ((int)cpak[pos] + 1);

					for(unsigned int i = 0; i < fw->tps.size(); i++)
					{
						if(strcmp(fw->tps.at(i).name.c_str(), name.c_str()) == 0)
						{
							SDL_FreeSurface(fw->tps.at(i).namep);
							SDL_FreeSurface(fw->tps.at(i).levelp);

							for(unsigned int y = 0; y < fw->tps.at(i).menu.size(); y++)
							{
								SDL_FreeSurface(fw->tps.at(i).menu.at(y).tSur);
								SDL_FreeSurface(fw->tps.at(i).menu.at(y).btn);
							}

							fw->tps.erase(fw->tps.begin() + i);

							break;
						}
					}

					fw->UpdateNames();
					fw->startY = fw->y + 65;
				}
			break;
			}

		case 0x0E:
			{
				int pos = 3;

				string name = getString(cpak, (pos + 1), (int)cpak[pos]).c_str();
				pos += ((int)cpak[pos] + 1);

				int level = atoi(getString(cpak, (pos + 1), (int)cpak[pos]).c_str());
				pos += ((int)cpak[pos] + 1);

				int job = atoi(getString(cpak, (pos + 1), (int)cpak[pos]).c_str());
				pos += ((int)cpak[pos] + 1);

				int id = atoi(getString(cpak, (pos + 1), (int)cpak[pos]).c_str());
				pos += ((int)cpak[pos] + 1);

				bool onoff = getBool(cpak[pos]);

				for(unsigned int i = 0; i < fw->tps.size(); i++)
				{
					if(fw->tps.at(i).name == name)
					{
						if(onoff)
						{
							SDL_SetAlpha(fw->tps.at(i).namep, SDL_RLEACCEL, 255);
							SDL_SetAlpha(fw->tps.at(i).levelp, SDL_RLEACCEL, 255);
						}

						else
						{
							SDL_SetAlpha(fw->tps.at(i).namep, SDL_SRCALPHA | SDL_RLEACCEL, 128);
							SDL_SetAlpha(fw->tps.at(i).levelp, SDL_SRCALPHA | SDL_RLEACCEL, 128);
						}

						if(fw->tps.at(i).level != level)
						{
							SDL_FreeSurface(fw->tps.at(i).levelp);

							fw->tps.at(i).levelp = RenderText_Solid(courMedium, IntToString(level).c_str(), clrFgBlack);
						}

						fw->tps.at(i).status = onoff;
						fw->tps.at(i).level = level;
						fw->tps.at(i).job = job;
						fw->tps.at(i).id = id;
					}
				}

				fw->UpdateNames();
				fw->startY = fw->y + 65;
			break;
			}

		case 0x1A:
			{
				int pos = 3;

				string namefrom = getString(cpak, (pos + 1), (int)cpak[pos]);
				pos += ((int)cpak[pos] + 1);

				string wrote = getString(cpak, (pos + 1), (int)cpak[pos]);
				pos += ((int)cpak[pos] + 1);

				string msg = getString(cpak, (pos + 1), (int)cpak[pos]);
				pos += ((int)cpak[pos] + 1);

				string fullmsg = wrote;
				fullmsg += " says: ";
				fullmsg += msg;

				string newString = fullmsg;

				SDL_Surface *fullMe = RenderText_Solid(courMedium, fullmsg.c_str(), clrFgBlack);

				if(fullMe->w > 270)
				{
					if(fullMe != NULL)
						SDL_FreeSurface(fullMe);

					fullMe = NULL;

					vector<string> npcString;

					string tempString = "";
					unsigned int readPos = 0;

					for(unsigned int n = 0; n <= fullmsg.length(); n++)
					{
						for(unsigned int t = readPos; t < n; t++)
							tempString += fullmsg[t];

						if(fullMe != NULL)
							SDL_FreeSurface(fullMe);

						fullMe = NULL;

						fullMe = RenderText_Solid(courMedium, tempString.c_str(), clrFgBlack);

						if(fullMe == NULL)
						{
						}

						else
						{
							if(fullMe->w > 270)
							{
								npcString.push_back(tempString);

								readPos = n;
							}

							if(n == fullmsg.length() && readPos != n)
								npcString.push_back(tempString);
						}

						tempString = "";
					}

					if(fullMe != NULL)
						SDL_FreeSurface(fullMe);

					if(npcString.size() > 0)
						newString = npcString.at(0);
				}

				bool found = false;

				for(unsigned int i = 0; i < pms->pm.size(); i++)
				{
					if(pms->pm.at(i).toname == namefrom)
					{
						found = true;
						break;
					}
				}

				if(!found)
					pms->AddWindow(namefrom.c_str());

				pms->AddMessage(namefrom.c_str(), newString.c_str());

				for(unsigned int i = 0; i < pms->pm.size(); i++)
				{
					if(pms->pm.at(i).toname == namefrom)
					{
						pms->pm.at(i).SetLast();
						break;
					}
				}
			break;
			}

		default:
			{
				Log("Unknown packet command, 0x%02X len: %d.", buf[2], len);
				Log("Packet: ");

				for(int i = 0; i < len; i++)
					Log("%02X ", buf[i]);
			break;
			}
	}

	return 0;
}

void RenderThread(bool &move)
{
	if(programex.size() > 0)
	{
		switch(programex.at(0))
		{
			case DISCONNECTED:
				{
					dc = true;
					
					CreateSystemMessage("Information", "Disconnected from server.");
				break;
				}

			case DISCONNECTED2:
				{
					dc = true;

					CreateSystemMessage("Infomation", "Server socket error.");
				break;
				}
		}

		programex.erase(programex.begin());
	}

	if(map != NULL)
	{
		if(map->camShowX != camShowX || map->camShowToX != camShowToX || map->camShowY != camShowY || map->camShowToY != camShowToY)
		{
			preRender.clear();

			for(unsigned int i = 0; i < map->tiles.size(); i++)
			{
				if(((map->tiles.at(i).posx + map->tiles.at(i).su->w) >= camShowX && camShowToX >= map->tiles.at(i).posx) && ((map->tiles.at(i).posy + map->tiles.at(i).su->h) >= camShowY && camShowToY >= map->tiles.at(i).posy))
				{
					int t = screen->w;
					int t2 = screen->h;

					map->tiles.at(i).cX = (camShowToX - map->tiles.at(i).posx);
					map->tiles.at(i).cY = (camShowToY - map->tiles.at(i).posy);

					t -= map->tiles.at(i).cX;
					t2 -= map->tiles.at(i).cY;

					map->tiles.at(i).cX = t;
					map->tiles.at(i).cY = t2;

					if(!map->tiles.at(i).inWin)
						map->tiles.at(i).inWin = true;

					preRender.push_back(&map->tiles.at(i));
				}

				else
				{
					if(map->tiles.at(i).inWin)
						map->tiles.at(i).inWin = false;
				}
			}

			map->camShowX = camShowX;
			map->camShowToX = camShowToX;
			map->camShowY = camShowY;
			map->camShowToY = camShowToY;
		}
	}

	if(me->follow)
	{
		Character *targetP = getPlayerById(me->followId);

		if(targetP == NULL)
		{
			if(playerLoad <= 0)
			{
				me->follow = false;
				me->followId = 0;
			}
		}

		else
		{
			if(NotInsideMePK(targetP, me))
			{
				float Vel = ((float)me->pixSec * ((float)me->move.get_ticks(false) / 1000.0f));

				float pX = targetP->x;
				float pY = targetP->y;

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
				{
					cX = (pX - me->x);
				}

				if(pY < me->y)
				{
					yOver = true;

					cY = (me->y - pY);
				}

				else
				{
					cY = (pY - me->y);
				}

				if(cX == 0)
				{
					if(cY != 0)
					{
						if(yOver)
							MeMove(UP, Vel);
						else
							MeMove(DOWN, Vel);
					}
				}

				else if(cY == 0)
				{
					if(cX != 0)
					{
						if(xOver)
							MeMove(LEFT, Vel);
						else
							MeMove(RIGHT, Vel);
					}
				}

				else
				{
					if(cX > cY)
					{
						if(xOver)
							MeMove(LEFT, Vel);
						else
							MeMove(RIGHT, Vel);
					}

					else if(cY > cX)
					{
						if(yOver)
							MeMove(UP, Vel);
						else
							MeMove(DOWN, Vel);
					}

					else
					{
						if(xOver)
							MeMove(LEFT, Vel);
						else
							MeMove(RIGHT, Vel);
					}
				}
			}

			me->move.start(false);
		}
	}

	if(cv.size() > 0)
	{
		for(unsigned int i = 0; i < cv.size(); i++)
		{
			if(cv.at(i)->enableTimer)
			{
				if((unsigned)cv.at(i)->timerProc <= getTime(true))
				{
					cv.at(i)->showProc = false;
					cv.at(i)->enableTimer = false;
					cv.at(i)->procentType = 0;

					showProcent = false;
				}
			}

			if(cv.at(i)->follow)
			{
				Character *targetPlayer = getPlayerById(cv.at(i)->followId);

				if(targetPlayer == NULL)
				{
					if(playerLoad <= 0)
					{
						cv.at(i)->followId = 0;
						cv.at(i)->follow = false;
					}
				}

				else
				{
					if(NotInsideMePK(targetPlayer, cv.at(i)))
					{
						if(cv.at(i)->showMenu)
							cv.at(i)->showMenu = false;

						MoveToPlayer(cv.at(i), targetPlayer);
					}

					cv.at(i)->move.start(false);
				}
			}

			else if(cv.at(i)->moving)
			{
				if(cv.at(i)->showMenu)
					cv.at(i)->showMenu = false;

				float xTemp = ((float)cv.at(i)->pixSec * ((float)cv.at(i)->move.get_ticks(false) / 1000.0f));

				Character *p = cv.at(i);

				if(p->cMove == UP)
				{
					if((p->y - xTemp) < 26 && (int)p->y != 26 && (int)p->y > 26)
						xTemp = (p->y - 26.0f);

					if((p->y - xTemp) >= 26)
						p->y -= xTemp;
				}

				else if(p->cMove == DOWN)
				{
					if((p->y + xTemp) > (msize(MAPY) - p->my->h) && (int)p->y != (msize(MAPY) - p->my->h) && (int)p->y < (msize(MAPY) - p->my->h))
						xTemp = ((msize(MAPY) - p->my->h) - p->y);

					if((p->y + xTemp) <= (msize(MAPY) - p->my->h))
						p->y += xTemp;
				}

				else if(p->cMove == LEFT)
				{
					if((p->x - xTemp) < 0 && (int)p->x != 0 && (int)p->x > 0)
						xTemp = p->x;

					if((p->x - xTemp) >= 0)
						p->x -= xTemp;
				}

				else if(p->cMove == RIGHT)
				{
					if((p->x + xTemp) > (msize(MAPX) - p->my->w) && (int)p->x != (msize(MAPX) - p->my->w) && (int)p->x < (msize(MAPX) - p->my->w))
						xTemp = ((msize(MAPX) - p->my->w) - p->x);

					if((p->x + xTemp) <= (msize(MAPX) - p->my->w))
						p->x += xTemp;
				}

				cv.at(i)->move.start(false);
			}

			if(((cv.at(i)->x + cv.at(i)->my->w) >= camShowX && camShowToX >= cv.at(i)->x) && ((cv.at(i)->y + cv.at(i)->my->h) >=camShowY && camShowToY >= cv.at(i)->y))
			{
				int t = screen->w;
				int t2 = screen->h;

				cv.at(i)->cX = (camShowToX - (int)cv.at(i)->x);
				cv.at(i)->cY = (camShowToY - (int)cv.at(i)->y);

				t -= cv.at(i)->cX;
				t2 -= cv.at(i)->cY;

				cv.at(i)->cX = t;
				cv.at(i)->cY = t2;

				if(!cv.at(i)->inWin)
				{
					cv.at(i)->inWin = true;

					if(mark->enable && mark->npcmark == cv.at(i)->id && mark->marktype == 1)
						UpdateMarkPlayer(cv.at(i));
				}

				if(mark->enable && mark->npcmark == cv.at(i)->id && mark->marktype == 1)
					UpdateMarkPlayer(cv.at(i));
			}

			else
			{
				if(cv.at(i)->inWin)
				{
					cv.at(i)->inWin = false;
				}

				if(mark->enable && mark->npcmark == cv.at(i)->id && mark->marktype == 1)
				{
					mark->enable = false;
					mark->marktype = 0;
					mark->npcmark = 0;

					for(unsigned int ip = 0; ip < cv.size(); ip++)
					{
						if(cv.at(ip)->showMenu)
							cv.at(ip)->showMenu = false;
					}
				}
			}
		}
	}

	if(npc.size() > 0)
	{
		for(unsigned int i = 0; i < npc.size(); i++)
		{
			if(((npc.at(i)->x + npc.at(i)->my->w) >= camShowX && camShowToX >= npc.at(i)->x) && ((npc.at(i)->y + npc.at(i)->my->h) >=camShowY && camShowToY >= npc.at(i)->y))
			{
				int t = screen->w;
				int t2 = screen->h;

				npc.at(i)->cX = (camShowToX - npc.at(i)->x);
				npc.at(i)->cY = (camShowToY - npc.at(i)->y);

				t -= npc.at(i)->cX;
				t2 -= npc.at(i)->cY;

				npc.at(i)->cX = t;
				npc.at(i)->cY = t2;

				if(!npc.at(i)->inWin)
				{
					npc.at(i)->inWin = true;

					if(mark->enable && mark->npcmark == npc.at(i)->id && mark->marktype == 3)
						UpdateMark(npc.at(i));
				}
			}

			else
			{
				if(npc.at(i)->inWin)
				{
					npc.at(i)->inWin = false;
				}

				if(mark->enable && mark->npcmark == npc.at(i)->id && mark->marktype == 3)
				{
					mark->enable = false;
					mark->marktype = 0;
					mark->npcmark = 0;

					for(unsigned int ip = 0; ip < npc.size(); ip++)
					{
						if(npc.at(ip)->showMenu)
							npc.at(ip)->showMenu = false;
						if(npc.at(ip)->showDialog)
						{
							for(unsigned int y = 0; y < npc.at(ip)->npct.size(); y++)
								SDL_FreeSurface(npc.at(ip)->npct.at(y).text);

							npc.at(ip)->npct.clear();

							npc.at(ip)->showDialog = false;
						}
					}
				}
			}
		}
	}

	if(cm.size() > 0)
	{
		for(unsigned int i = 0; i < cm.size(); i++)
		{
			if(cm.at(i)->fades)
			{
				if((cm.at(i)->fadep >= 255 && cm.at(i)->fadeway) || (cm.at(i)->fadep <= 0 && !cm.at(i)->fadeway))
				{
					cm.at(i)->fades = false;

					if(!cm.at(i)->fadeway)
					{
						SDL_FreeSurface(cm.at(i)->nameSurface);
						SDL_FreeSurface(cm.at(i)->whenMarkName);
						
						delete cm.at(i);
						cm.erase(cm.begin() + i);
						
						continue;
					}
				}

				if(cm.at(i)->fades)
				{
					if((unsigned)cm.at(i)->fadetime <= getTime(true))
					{
						if(cm.at(i)->fadeway)
							cm.at(i)->fadep += 10;

						else
							cm.at(i)->fadep -= 10;

						if(cm.at(i)->fadetime == 0)
							cm.at(i)->fadetime = getTime(true);

						cm.at(i)->fadetime += 2;
						cm.at(i)->showProc = false;
					}
				}
			}

			if(cm.at(i)->fades && !cm.at(i)->fadeway)
				continue;

			if(cm.at(i)->enableTimer)
			{
				if((unsigned)cm.at(i)->timerProc <= getTime(true))
				{
					cm.at(i)->showProc = false;
					cm.at(i)->enableTimer = false;
					cm.at(i)->procentType = 0;

					showProcent = false;
				}
			}

			if(cm.at(i)->moving && cm.at(i)->followId == 0)
			{
				float xTemp = ((float)cm.at(i)->speed * ((float)cm.at(i)->move.get_ticks(true) / 1000.0f));

				switch(cm.at(i)->moveWay)
				{
					case UP: cm.at(i)->y -= xTemp;
						break;

					case DOWN: cm.at(i)->y += xTemp;
						break;

					case LEFT: cm.at(i)->x -= xTemp;
						break;

					case RIGHT: cm.at(i)->x += xTemp;
						break;
				}		

				cm.at(i)->move.start(true);

				cm.at(i)->beforeX = cm.at(i)->x;
				cm.at(i)->beforeY = cm.at(i)->y;
			}

			else if(cm.at(i)->followId != 0)
			{
				if(cm.at(i)->followId == me->id)
				{
					if(NotInsideMe(cm.at(i), me))
					{
						float pX = me->x;
						float pY = me->y;

						float cX = 0;
						float cY = 0;

						float side = (((float)cm.at(i)->speed / 2.0f) * ((float)cm.at(i)->move.get_ticks(true) / 1000.0f));
						float nor = ((float)cm.at(i)->speed * ((float)cm.at(i)->move.get_ticks(true) / 1000.0f));

						bool yOver = false;
						bool xOver = false;

						if(pX < cm.at(i)->x)
						{
							xOver = true;

							cX = (cm.at(i)->x - (float)pX);
						}

						else
						{
							cX = ((float)pX - cm.at(i)->x);
						}

						if(pY < cm.at(i)->y)
						{
							yOver = true;

							cY = (cm.at(i)->y - (float)pY);
						}

						else
						{
							cY = ((float)pY - cm.at(i)->y);
						}

						if(cX == 0)
						{
							if(cY != 0)
							{
								if(yOver)
									cm.at(i)->y -= nor;
								else
									cm.at(i)->y += nor;
							}
						}

						else if(cY == 0)
						{
							if(cX != 0)
							{
								if(xOver)
									cm.at(i)->x -= nor;
								else
									cm.at(i)->x += nor;
							}
						}

						else
						{
							if(cX > cY)
							{
								if(xOver)
									cm.at(i)->x -= nor;
								else
									cm.at(i)->x += nor;
							}

							else if(cY > cX)
							{
								if(yOver)
									cm.at(i)->y -= nor;
								else
									cm.at(i)->y += nor;
							}

							else
							{
								if(xOver)
									cm.at(i)->x -= side;
								else
									cm.at(i)->x += side;

								if(yOver)
									cm.at(i)->y -= side;
								else
									cm.at(i)->y += side;
							}
						}
					}

					cm.at(i)->move.start(true);
				}

				else
				{
					for(unsigned int z = 0; z < cv.size(); z++)
					{
						if(cv.at(z)->id == cm.at(i)->followId)
						{
							if(NotInsideMe(cm.at(i), cv.at(z)))
							{
								float pX = cv.at(z)->x;
								float pY = cv.at(z)->y;

								float cX = 0;
								float cY = 0;

								float side = (((float)cm.at(i)->speed / 2.0f) * ((float)cm.at(i)->move.get_ticks(true) / 1000.0f));
								float nor = ((float)cm.at(i)->speed * ((float)cm.at(i)->move.get_ticks(true) / 1000.0f));

								bool yOver = false;
								bool xOver = false;

								if(pX < cm.at(i)->x)
								{
									xOver = true;

									cX = (cm.at(i)->x - (float)pX);
								}

								else
								{
									cX = ((float)pX - cm.at(i)->x);
								}

								if(pY < cm.at(i)->y)
								{
									yOver = true;

									cY = (cm.at(i)->y - (float)pY);
								}

								else
								{
									cY = ((float)pY - cm.at(i)->y);
								}

								if(cX == 0)
								{
									if(cY != 0)
									{
										if(yOver)
											cm.at(i)->y -= nor;
										else
											cm.at(i)->y += nor;
									}
								}

								else if(cY == 0)
								{
									if(cX != 0)
									{
										if(xOver)
											cm.at(i)->x -= nor;
										else
											cm.at(i)->x += nor;
									}
								}

								else
								{
									if(cX > cY)
									{
										if(xOver)
											cm.at(i)->x -= nor;
										else
											cm.at(i)->x += nor;
									}

									else if(cY > cX)
									{
										if(yOver)
											cm.at(i)->y -= nor;
										else
											cm.at(i)->y += nor;
									}

									else
									{
										if(xOver)
											cm.at(i)->x -= side;
										else
											cm.at(i)->x += side;

										if(yOver)
											cm.at(i)->y -= side;
										else
											cm.at(i)->y += side;
									}
								}
							}

							cm.at(i)->move.start(true);

							break;
						}
					}
				}
			}

			else
			{
				if(cm.at(i)->beforeX != cm.at(i)->x || cm.at(i)->beforeY != cm.at(i)->y)
				{
					float nor = ((float)cm.at(i)->speed * ((float)cm.at(i)->move.get_ticks(true) / 1000.0f));

					float changeX = 0;
					float changeY = 0;

					if(cm.at(i)->beforeX > cm.at(i)->x)
						changeX = (cm.at(i)->beforeX - cm.at(i)->x);

					else if(cm.at(i)->beforeX < cm.at(i)->x)
						changeX = (cm.at(i)->x - cm.at(i)->beforeX);

					if(cm.at(i)->beforeY > cm.at(i)->y)
						changeY = (cm.at(i)->beforeY - cm.at(i)->y);

					else if(cm.at(i)->beforeY < cm.at(i)->y)
						changeY = (cm.at(i)->y - cm.at(i)->beforeY);

					if(changeX <= nor && changeY <= nor)
					{
						cm.at(i)->x = cm.at(i)->beforeX;
						cm.at(i)->y = cm.at(i)->beforeY;
					}

					else
					{	
						if(cm.at(i)->beforeX > cm.at(i)->x)
							cm.at(i)->x += nor;

						else if(cm.at(i)->beforeX < cm.at(i)->x)
							cm.at(i)->x -= nor;

						if(cm.at(i)->beforeY > cm.at(i)->y)
							cm.at(i)->y += nor;

						else if(cm.at(i)->beforeY < cm.at(i)->y)
							cm.at(i)->y -= nor;
					}

					cm.at(i)->move.start(true);
				}
			}

			if(((cm.at(i)->x + cm.at(i)->m->w) >= camShowX && camShowToX >= cm.at(i)->x) && ((cm.at(i)->y + cm.at(i)->m->h) >=camShowY && camShowToY >= cm.at(i)->y))
			{
				int t = screen->w;
				int t2 = screen->h;

				cm.at(i)->cX = (camShowToX - (int)cm.at(i)->x);
				cm.at(i)->cY = (camShowToY - (int)cm.at(i)->y);

				t -= cm.at(i)->cX;
				t2 -= cm.at(i)->cY;

				cm.at(i)->cX = t;
				cm.at(i)->cY = t2;

				if(!cm.at(i)->inWin)
				{
					cm.at(i)->inWin = true;
				}
			}

			else
			{
				if(cm.at(i)->inWin)
				{
					cm.at(i)->inWin = false;
				}
			}
		}
	}

	if(nv.size() > 0)
	{
		for(unsigned int i = 0; i < nv.size(); i++)
		{
			if((unsigned)nv.at(i)->dTime <= getTime(true))
			{
				bool penya = nv.at(i)->penya;

				SDL_FreeSurface(nv.at(i)->msg);

				delete nv.at(i);
				nv.erase(nv.begin() + i);

				if(penya)
				{
					for(unsigned int r = i; r < nv.size(); r++)
					{
						if(nv.at(r)->penya)
							nv.at(r)->y -= 17;
					}
				}
			}
		}
	}

	if(bullets.size() > 0)
	{
		for(unsigned int i = 0; i < bullets.size(); i++)
		{
			float xTemp = (bullets.at(i)->speed * ((float)bullets.at(i)->move.get_ticks(true) / 1000.0f));

			bool res = false;

			if(bullets.at(i)->param == 6)
			{
				float xTemp2 = (450 * ((float)bullets.at(i)->move.get_ticks(true) / 1000.0f));

				res = BotBullet(xTemp2, bullets.at(i), false);
				bullets.at(i)->cWay = 0;
			}

			else if(bullets.at(i)->param == 5)
			{
				float xTemp2 = (600 * ((float)bullets.at(i)->move.get_ticks(true) / 1000.0f));

				res = BotBullet(xTemp2, bullets.at(i), false);
				bullets.at(i)->cWay = 0;
			}

			else
			{
				switch(bullets.at(i)->cWay)
				{
					case LEFT: bullets.at(i)->cX -= xTemp;
						break;

					case RIGHT: bullets.at(i)->cX += xTemp;
						break;
				}
			}

			bullets.at(i)->move.start(true);

			if(((bullets.at(i)->cX + bullets.at(i)->cPic->w) >= camShowX && camShowToX >= bullets.at(i)->cX) && ((bullets.at(i)->cY + bullets.at(i)->cPic->h) >=camShowY && camShowToY >= bullets.at(i)->cY))
			{
				int t = screen->w;
				int t2 = screen->h;

				bullets.at(i)->sX = (camShowToX - (int)bullets.at(i)->cX);
				bullets.at(i)->sY = (camShowToY - (int)bullets.at(i)->cY);

				t -= bullets.at(i)->sX;
				t2 -= bullets.at(i)->sY;

				bullets.at(i)->sX = t;
				bullets.at(i)->sY = t2;

				if(!bullets.at(i)->inWin)
				{
					bullets.at(i)->inWin = true;
				}
			}

			else
			{
				if(bullets.at(i)->inWin)
				{
					bullets.at(i)->inWin = false;
				}
			}

			if(res)
			{
				delete bullets.at(i);
				bullets.erase(bullets.begin() + i);

				continue;
			}

			if(bullets.at(i)->cX < 0 || bullets.at(i)->cX > msize(MAPX))
			{
				delete bullets.at(i);
				bullets.erase(bullets.begin() + i);
				
				continue;
			}

			else if(bullets.at(i)->cWay == LEFT && (bullets.at(i)->cX <= (bullets.at(i)->startX - bullets.at(i)->disX)))
			{
				delete bullets.at(i);
				bullets.erase(bullets.begin() + i);

				continue;
			}

			else if(bullets.at(i)->cWay == RIGHT && (bullets.at(i)->cX >= (bullets.at(i)->startX + bullets.at(i)->disX)))
			{
				delete bullets.at(i);
				bullets.erase(bullets.begin() + i);

				continue;
			}

			bool found = false;

			for(int z = (cm.size() - 1); z >= 0; z--)
			{
				if(((bullets.at(i)->cX - cm.at(z)->m->w) <= cm.at(z)->x && (bullets.at(i)->cX + bullets.at(i)->cPic->w) >= cm.at(z)->x && ((bullets.at(i)->cY - cm.at(z)->m->h) <= cm.at(z)->y && (bullets.at(i)->cY + bullets.at(i)->cPic->h) >= cm.at(z)->y)))
				{
					if(!cm.at(z)->dead)
					{
						if(bullets.at(i)->target != 0)
						{
							if(bullets.at(i)->target != cm.at(z)->mId)
								continue;

							if(bullets.at(i)->type != 2)
							{
								if(bullets.at(i)->type != 1)
									continue;
							}
						}

						if(bullets.at(i)->owner == me->id && !AttackedTarget(bullets.at(i), cm.at(z)->mId, 2, false))
						{
							PacketGameAttack(cm.at(z)->mId, 1, bullets.at(i)->param, bullets.at(i)->hits);

							CheckMonsterInfo(cm.at(z)->cX, cm.at(z)->cY, false, 1, cm.at(z), NULL, false);
						}

						if(!bullets.at(i)->igenom || bullets.at(i)->param > 1)
						{
							delete bullets.at(i);
							bullets.erase(bullets.begin() + i);

							found = true;
							break;
						}

						else
						{
							if(!AttackedTarget(bullets.at(i), cm.at(z)->mId, 2, true))
								bullets.at(i)->hits++;
						}
					}
				}
			}

			if(PKENABLE && !found)
			{
				for(int z2 = (cv.size() - 1); z2 >= 0; z2--)
				{
					if(((bullets.at(i)->cX - cv.at(z2)->my->w) <= cv.at(z2)->x && (bullets.at(i)->cX + bullets.at(i)->cPic->w) >= cv.at(z2)->x && ((bullets.at(i)->cY - cv.at(z2)->my->h) <= cv.at(z2)->y && (bullets.at(i)->cY + bullets.at(i)->cPic->h) >= cv.at(z2)->y)))
					{
						if(bullets.at(i)->target != 0)
						{
							if(bullets.at(i)->target != cv.at(z2)->id)
								continue;

							if(bullets.at(i)->type != 2)
							{
								if(bullets.at(i)->type != 0)
									continue;
							}
						}

						if(bullets.at(i)->owner == me->id && !AttackedTarget(bullets.at(i), cv.at(z2)->id, 1, false))
						{
							PacketGameAttack(cv.at(z2)->id, 0, bullets.at(i)->param, bullets.at(i)->hits);

							CheckMonsterInfo(cv.at(z2)->cX, cv.at(z2)->cY, false, 1, NULL, cv.at(z2), false);
						}

						if(bullets.at(i)->owner != cv.at(z2)->id)
						{
							if(!bullets.at(i)->igenom || bullets.at(i)->param > 1)
							{
								delete bullets.at(i);
								bullets.erase(bullets.begin() + i);

								found = true;
								break;
							}

							else
							{
								if(!AttackedTarget(bullets.at(i), cv.at(z2)->id, 1, true))
									bullets.at(i)->hits++;
							}
						}
					}
				}

				if(!found)
				{
					if(bullets.at(i)->owner != me->id)
					{
						if(((bullets.at(i)->cX - me->my->w) <= me->x && (bullets.at(i)->cX + bullets.at(i)->cPic->w) >= me->x && ((bullets.at(i)->cY - me->my->h) <= me->y && (bullets.at(i)->cY + bullets.at(i)->cPic->h) >= me->y)))
						{
							if(!bullets.at(i)->igenom || bullets.at(i)->param > 1)
							{
								delete bullets.at(i);
								bullets.erase(bullets.begin() + i);
							}

							else
								bullets.at(i)->hits++;
						}
					}
				}
			}
		}
	}

	if(icons.size() > 0)
	{
		for(unsigned int i = 0; i < icons.size(); i++)
		{
			if((unsigned)icons.at(i)->nextRemove <= getTime(true))
			{
				if((icons.at(i)->timeLeft - 1) <= 0 && icons.at(i)->timeLeft != 0)
				{
					icons.at(i)->timeLeft = 0;

					char temp[10] = "";
					_itoa_s((int)icons.at(i)->timeLeft, temp, 10, 10);

					SDL_FreeSurface(icons.at(i)->tSurface);

					icons.at(i)->tSurface = RenderText_Blended(courSmall, temp, clrFgBlack);
				}

				else
				{
					icons.at(i)->timeLeft -= 1;

					SDL_FreeSurface(icons.at(i)->tSurface);

					char temp[10] = "";
					_itoa_s((int)icons.at(i)->timeLeft, temp, 10, 10);

					icons.at(i)->tSurface = RenderText_Blended(courSmall, temp, clrFgBlack);
				}

				icons.at(i)->nextRemove += 1000;
			}
		}
	}

	if(drops.size() > 0)
	{
		for(int i = (drops.size() - 1); i >= 0; i--)
		{
			if(((drops.at(i)->x + drops.at(i)->pic->w) >= camShowX && camShowToX >= drops.at(i)->x) && ((drops.at(i)->y + drops.at(i)->pic->h) >=camShowY && camShowToY >= drops.at(i)->y))
			{
				int t = screen->w;
				int t2 = screen->h;

				drops.at(i)->cX = (camShowToX - drops.at(i)->x);
				drops.at(i)->cY = (camShowToY - drops.at(i)->y);

				t -= drops.at(i)->cX;
				t2 -= drops.at(i)->cY;

				drops.at(i)->cX = t;
				drops.at(i)->cY = t2;

				if(!drops.at(i)->inWin)
				{
					drops.at(i)->inWin = true;
				}
			}

			else
			{
				if(drops.at(i)->inWin)
				{
					drops.at(i)->inWin = false;
				}
			}

			if(drops.at(i)->inWin)
			{
				if(((drops.at(i)->x - me->my->w) <= me->x && (drops.at(i)->x + drops.at(i)->pic->w) >= me->x && ((drops.at(i)->y - me->my->h) <= me->y && (drops.at(i)->y + drops.at(i)->pic->h) >= me->y)))
				{
					if((unsigned)drops.at(i)->dropNext <= getTime(true))
					{
						if((unsigned)nextItem <= getTime(true))
						{
							bool fus = fullInventory();
							bool nospaceforme = fullInventoryItem(drops.at(i)->itemid);

							if(fus)
							{
								if(drops.at(i)->itemid == 2)
								{
									PacketPickUpDrop(drops.at(i)->id);

									nextItem = (getTime(true) + dropDelay);
									drops.at(i)->dropNext = (getTime(true) + 2000);
								}
							}

							else
							{
								if(!nospaceforme)
								{
									PacketPickUpDrop(drops.at(i)->id);

									nextItem = (getTime(true) + dropDelay);
									drops.at(i)->dropNext = (getTime(true) + 2000);
								}
							}
						}
					}
				}
			}
		}
	}

	if(dmgs.size() > 0)
	{
		for(unsigned int z = 0; z < dmgs.size(); z++)
		{
			if((unsigned)dmgs.at(z).deletetime <= getTime(true))
			{
				SDL_FreeSurface(dmgs.at(z).dmg);

				if(dmgs.at(z).dmgItem != NULL)
					SDL_FreeSurface(dmgs.at(z).dmgItem);

				dmgs.erase(dmgs.begin() + z);
			}

			else
			{
				float xTemp = (75 * ((float)dmgs.at(z).move.get_ticks(true) / 1000.0f));

				dmgs.at(z).y -= xTemp;

				dmgs.at(z).move.start(true);
			}
		}
	}

	if(anis.size() > 0)
	{
		for(unsigned int i = 0; i < anis.size(); i++)
		{
			if((unsigned)anis.at(i)->disTime <= getTime(true))
			{
				if(anis.at(i)->deletePic)
				{
					for(unsigned int a = 0; a < anis.at(i)->ani.size(); a++)
						SDL_FreeSurface(anis.at(i)->ani.at(a));
				}

				delete anis.at(i);
				anis.erase((anis.begin() + i));
			}

			else
			{
				if((unsigned)anis.at(i)->nextChange <= getTime(true))
				{
					if(anis.at(i)->frame == anis.at(i)->ani.size())
						anis.at(i)->frame = 1;
					else
						anis.at(i)->frame++;

					anis.at(i)->nextChange = (getTime(true) + (1000 / anis.at(i)->fps));
				}

				if(((anis.at(i)->x + anis.at(i)->ani.at((anis.at(i)->frame - 1))->w) >= camShowX && camShowToX >= anis.at(i)->x) && ((anis.at(i)->y + anis.at(i)->ani.at((anis.at(i)->frame - 1))->h) >=camShowY && camShowToY >= anis.at(i)->y))
				{
					int t = screen->w;
					int t2 = screen->h;

					anis.at(i)->cX = (camShowToX - anis.at(i)->x);
					anis.at(i)->cY = (camShowToY - anis.at(i)->y);

					t -= anis.at(i)->cX;
					t2 -= anis.at(i)->cY;

					anis.at(i)->cX = t;
					anis.at(i)->cY = t2;

					if(!anis.at(i)->inWin)
						anis.at(i)->inWin = true;
				}

				else
				{
					if(anis.at(i)->inWin)
						anis.at(i)->inWin = false;
				}
			}
		}
	}

	for(unsigned int t = 0; t < 9; t++)
	{
		if(task->pic[t]->enable)
		{
			if(task->pic[t]->cooldsec > 0)
			{
				if((unsigned)task->pic[t]->coolDown < GetTickCount())
				{
					task->pic[t]->cooldsec -= 100;
					task->pic[t]->coolDown += 100;

					if(task->pic[t]->coold != NULL)
						SDL_FreeSurface(task->pic[t]->coold);

					task->pic[t]->coold = RenderText_Solid(cour, FloatToString(((float)task->pic[t]->cooldsec / 1000.0f)).c_str(), clrFgBlack);
				}
			}

			else
			{
				if(task->pic[t]->coolDown != 0)
				{
					task->pic[t]->coolDown = 0;
				}
			}
		}
	}

	if(chatting || chatWindow || (chatInputWindow && iw.size() > 0) || write.enable || (chatMessage && pms->pm.size() > 0))
	{
		if((unsigned)chatIcon.nextChange <= getTime(true))
		{
			if(chatIcon.frame == chatIcon.ani.size())
				chatIcon.frame = 1;
			else
				chatIcon.frame++;

			chatIcon.nextChange = (getTime(true) + (1000 / chatIcon.fps));
		}

		else if(setChatIconShow)
		{
			chatIcon.frame = 1;
			chatIcon.nextChange = (getTime(true) + (1000 / chatIcon.fps));
			setChatIconShow = false;
		}
	}

	bool test = ObjectInfoWindowFunction(lastMouseX, lastMouseY, lastMouseX, lastMouseY);

	if(test)
		ow->enable = true;

	else
		ow->enable = false;

	if(spawned)
		UpdateAll();
	else if(login)
		UpdateLogin();
	else
		UpdateLoadingImage();

	if(showFps)
		framess++;

	if(showFps)
	{
		if(fpscheck.get_ticks(true) >= 1000)
		{
			string fpsString = "FPS: ";
			char buf[10] = "";

			_itoa_s(framess, buf, 10, 10);

			fpsString += buf;

			if(fpsSurface != NULL)
				SDL_FreeSurface(fpsSurface);

			if(spawned)
				fpsSurface = RenderText_Solid(cour, fpsString.c_str(), clrFg);

			else
				fpsSurface = RenderText_Solid(cour, fpsString.c_str(), clrFgBlack);

			framess = 0;
			fpscheck.start(true);

			if(pingCount >= 1)
			{
				if(!sentPingPacket)
				{
					PacketSendPing();
					QueryPerformanceCounter(&pingTimes);

					sentPingPacket = true;
				}

				pingCount = 0;
			}

			else
				pingCount++;
		}
	}
}
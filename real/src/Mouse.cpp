#include <iostream>
#include <vector>
#include <SDL_ttf.h>
#include <string>

#include "Mouse.h"
#include "Packets.h"
#include "Login.h"
#include "Game.h"
#include "Misc.h"
#include "Process.h"
#include "Motion.h"
#include "Preload.h"
#include "Main.h"
#include "Music.h"
#include "Friend.h"

using namespace std;

extern bool keys[365];

extern SDL_Surface *screen;

extern vector<PPAK> pp;

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
extern vector<Message*> msgs;
extern vector<InputWindow*> iw;
extern vector<LoginMessage*> lmsgs;
extern vector<CharacterSelect> charsel;
extern vector<LoginInput*> linputs;
extern vector<Skill*> skill;

extern Character *me;

extern TTF_Font *cour;
extern TTF_Font *courMedium;
extern TTF_Font *courSmall;
extern TTF_Font *courBold;
extern SDL_Color clrFg;
extern SDL_Color clrFgBlack;
extern SDL_Color clrFgRed;
extern SDL_Color clrFgBlue;
extern SDL_Color clrFgGreen;

extern bool done;
extern bool PKENABLE;
extern bool spawned;
extern bool colorkey;
extern bool showFps;

extern string mLoad;

extern Mark *mark;
extern Inventory *inv;
extern Taskbar *task;
extern OptionWindow *option;
extern ShopWindow *sw;
extern PlayerTrade *ptrade;
extern TeleportWindow *tp;
extern FriendWindow *fw;
extern ObjectInfoWindow *ow;
extern ChatManager *chatFunc;
extern PrivateMessage *pms;

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
extern bool initProcess;

extern bool chatting;
extern bool chatWindow;

extern string chat;

extern long nextInput;

extern int chatTries;

extern SDL_Surface *chatNot;
extern SDL_Surface *chatInfo;

extern long nextEnter;
extern long nextChatDelete;
extern long nextShot;
extern long nextEquipWeapon;
extern long nextWindowChange;
extern long nextSpace;
extern long nextMapChange;

extern bool transWindow;

extern SkillWindow *sk;

extern bool drag;

extern Drag dragged;

int currMoveWay = 0;

extern bool botActive;

extern long attackDelay;

bool chatInputWindow = false;
bool chatMessage = false;

int lastMouseX = 0;
int lastMouseY = 0;

extern bool noSound;
extern bool noMusic;

extern MusicManager *music;

extern int logplace;

extern bool writeLogin;
extern int writeLoginX;
extern int writeLoginY;
extern int writeLoginX2;
extern int writeLoginY2;

extern SDL_Surface *loginFirst;
extern SDL_Surface *loginChar;
extern SDL_Surface *loginRem;

extern bool wantPacket;

LoginWriter write = LoginWriter();

extern int lastSent;

extern bool login;

extern int isEvent;

extern bool remLogin;
extern char loginName[40];

int isMasked = 0;

int BuffSize = 0;

extern int resX;
extern int resY;

int chatPlace = 0;
int chatPlaceW = 0;

extern bool setChatIconShow;

int job = 1;

extern SDL_Surface *fpsSurface;
extern SDL_Surface *pingSurface;

bool HandleMouseLogin(SDL_Event event)
{
	if(event.button.button == SDL_BUTTON_LEFT)
	{
		int x = event.button.x;
		int y = event.button.y;
		int x2 = ((screen->w / 2) - (loginFirst->w / 2));
		int y2 = ((screen->h / 2) - (loginFirst->h / 2));
		int x3 = ((screen->w / 2) - (loginChar->w / 2));
		int y3 = ((screen->h / 2) - (loginChar->h / 2));

		if(msgs.size() > 0)
		{
			for(unsigned int i = 0; i < msgs.size(); i++)
			{
				for(unsigned int o = 0; o < msgs.at(i)->btn.size(); o++)
				{
					Button *b = msgs.at(i)->btn.at(o);

					if(OverButton(b->x, b->y, b->sur->w, b->sur->h, x, y))
					{
						switch(b->effect)
						{
							case DISCONNECTED: return true;
								break;
						}

						return false;
					}
				}
			}
		}

		if(lmsgs.size() > 0)
		{
			for(unsigned int i = 0; i < lmsgs.size(); i++)
			{
				if(OverButton((lmsgs.at(i)->x + 94), (lmsgs.at(i)->y + 59), 81, 20, x, y))
				{
					lmsgs.at(i)->Clean();
					delete lmsgs.at(i);
					lmsgs.erase(lmsgs.begin() + i);

					return false;
				}
			}

			return false;
		}

		if(linputs.size() > 0)
		{
			for(unsigned int i = 0; i < linputs.size(); i++)
			{
				int placeX = 77;
				int placeY = 102;

				if(linputs.at(i)->t == CREATECHARACTER)
					placeY = 166;

				if(OverButton(((screen->w / 2) - (linputs.at(i)->win->w / 2)) + 11, ((screen->h / 2) - (linputs.at(i)->win->h / 2)) + 98, 15, 15, x, y))
					job = 1;

				if(OverButton(((screen->w / 2) - (linputs.at(i)->win->w / 2)) + 11, ((screen->h / 2) - (linputs.at(i)->win->h / 2)) + 129, 15, 15, x, y))
					job = 2;

				if(OverButton(((screen->w / 2) - (linputs.at(i)->win->w / 2)) + 125, ((screen->h / 2) - (linputs.at(i)->win->h / 2)) + 98, 15, 15, x, y))
					job = 3;

				if(OverButton(((screen->w / 2) - (linputs.at(i)->win->w / 2)) + 125, ((screen->h / 2) - (linputs.at(i)->win->h / 2)) + 129, 15, 15, x, y))
					job = 4;

				if(OverButton((((screen->w / 2) - (linputs.at(i)->win->w / 2)) + placeX), (((screen->h / 2) - (linputs.at(i)->win->h / 2)) + placeY), 84, 22, x, y))
				{
					switch(linputs.at(i)->t)
					{
						case CREATECHARACTER:
							{
								if(strlen(write.GetText(1)) <= 0)
									LMessage(6, "Please input a name.");

								else if(strlen(write.GetText(1)) >= 16)
									LMessage(6, "Character name is too long.");

								else
								{
									PacketCharacterChange(1, (char*)write.GetText(1), NULL, linputs.at(0)->i, job);
									wantPacket = true;
									lastSent = 0x7;
								}

								write.StopWrite();
								job = 1;
							break;
							}

						case DELETECHARACTER:
							{
								if(strlen(write.GetText(1)) <= 0)
									LMessage(6, "Please input a password.");

								else
								{
									PacketCharacterChange(2, (char*)charsel.at((linputs.at(0)->i - 1)).name2.c_str(), (char*)write.GetText(1), 0, 0);
									wantPacket = true;
									lastSent = 0x8;
								}

								write.StopWrite();
							break;
							}
					}

					if(linputs.at(i)->win != NULL)
						SDL_FreeSurface(linputs.at(i)->win);

					delete linputs.at(i);

					linputs.erase(linputs.begin() + i);

					return false;
				}
			}

			return false;
		}

		switch(logplace)
		{
			case LOGINSCREEN:
				{
					if(OverButton((x2 + 40), (y2 + 139), loginRem->w, loginRem->h, x, y))
					{
						remLogin = !remLogin;
						SDL_FreeSurface(loginRem);

						if(remLogin)
						{
							loginRem = loadimage("Data/Interface/Option-Full.png", false, 0, 0, 0);

							AddConfigOption(0, NULL, 1, 0, ONEINT, "RemLogin:");
						}

						else
						{
							loginRem = loadimage("Data/Interface/Option-Empty.png", false, 0, 0, 0);

							RemoveConfigOption("Login:");
							AddConfigOption(0, NULL, 0, 0, ONEINT, "RemLogin:");
						}

						return false;
					}

					if(OverButton((x2 + 40), (y2 + 47), 257, 28, x, y))
					{
						write.ChangeFocus(2);

						return false;
					}

					if(OverButton((x2 + 40), (y2 + 97), 257, 28, x, y))
					{
						write.ChangeFocus(1);

						return false;
					}

					if(OverButton((x2 + 324), (y2 + 35), 121, 38, x, y))
					{
						if(strlen(write.GetText(1)) <= 0)
							LMessage(5, "Please input a username.");

						else if(strlen(write.GetText(2)) <= 0)
							LMessage(5, "Please input a password.");

						else
						{
							PacketLogin((char*)write.GetText(1), (char*)write.GetText(2), 1);

							wantPacket = true;
						}

						return false;
					}

					if(OverButton((x2 + 324), (y2 + 73), 121, 38, x, y))
					{
						if(strlen(write.GetText(1)) <= 0)
							LMessage(5, "Please input a username.");

						else if(strlen(write.GetText(2)) <= 0)
							LMessage(5, "Please input a password.");

						else
						{
							PacketLogin((char*)write.GetText(1), (char*)write.GetText(2), 2);

							wantPacket = true;
						}

						return false;
					}

					if(OverButton((x2 + 324), (y2 + 111), 121, 38, x, y))
						return true;

				break;
				}

			case CHARSELECT:
				{
					for(unsigned int i = 0; i < charsel.size(); i++)
					{
						if(OverButton(((((screen->w / 2) - (loginChar->w / 2)) + 95) + (i * 284)), (((screen->h / 2) - (loginChar->h / 2)) + 180), 84, 22, x, y))
						{
							if(charsel.at(i).slot == 0)
							{
								CreateLoginInput(CREATECHARACTER, (i + 1));

								write.StartWrite(true, false, (((screen->w / 2) - (linputs.back()->win->w / 2)) + 23), (((screen->h / 2) - (linputs.back()->win->h / 2)) + 63), 0, 0, 1);

								return false;
							}

							else
							{
								CreateLoginInput(DELETECHARACTER, (i + 1));

								isMasked = DELETECHARACTER;
								write.StartWrite(true, false, (((screen->w / 2) - (linputs.back()->win->w / 2)) + 23), (((screen->h / 2) - (linputs.back()->win->h / 2)) + 63), 0, 0, 1);

								return false;
							}
						}

						if(OverButton(((((screen->w / 2) - (loginChar->w / 2)) + 95) + (i * 284)), (((screen->h / 2) - (loginChar->h / 2)) + 213), 84, 22, x, y))
						{
							PacketMainSpawn(charsel.at(i).slot, me);

							return false;
						}
					}

					if(OverButton((x3 + 12), (y3 + 260), 84, 22, x, y))
					{
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
						}

						charsel.clear();

						Logout();
						logplace = LOGINSCREEN;
						write.StartWrite(true, true, (((screen->w / 2) - (loginFirst->w / 2)) + 44), (((screen->h / 2) - (loginFirst->h / 2)) + 52), (((screen->w / 2) - (loginFirst->w / 2)) + 44), (((screen->h / 2) - (loginFirst->h / 2)) + 102), 1);
						write.ar1s = loginName;
						write.NewSurface(1);

						return false;
					}
				break;
				}
		}

	}

	return false;
}

bool HandleMouse(SDL_Event event, bool &altf, bool &move)
{
	if(event.button.button == SDL_BUTTON_LEFT)
	{
		int x = event.button.x;
		int y = event.button.y;

		lastMouseX = event.button.x;
		lastMouseY = event.button.y;

		if(task->enableTask && task->enablePress)
		{
			bool overAll = OverButton(task->x, task->y, task->bar->w, task->bar->h, x, y);

			if(OverButton((task->bar->w - 106), (task->y + 9), 83, 21, x, y))
			{
				if(task->enableMenu)
					task->enableMenu = false;
				else
					task->enableMenu = true;

				return false;
			}

			if(task->enableMenu)
			{
				task->enableMenu = false;

				if(OverButton(((task->x + task->bar->w) - task->menu->w), (task->y - task->menu->h), 105, 42, x, y))
				{
					option->enable = true;

					return false;
				}

				if(OverButton(((task->x + task->bar->w) - task->menu->w), ((task->y - task->menu->h) + 84), 105, 42, x, y))
				{
					altf = true;

					return true;
				}

				if(OverButton(((task->x + task->bar->w) - task->menu->w), ((task->y - task->menu->h) + 42), 105, 42, x, y))
				{
					SmallClean();
					login = true;
					logplace = LOGINSCREEN;
					write.StartWrite(true, true, (((screen->w / 2) - (loginFirst->w / 2)) + 44), (((screen->h / 2) - (loginFirst->h / 2)) + 52), (((screen->w / 2) - (loginFirst->w / 2)) + 44), (((screen->h / 2) - (loginFirst->h / 2)) + 102), 1);
					write.ar1s = loginName;
					write.NewSurface(1);

					SDL_FreeSurface(fpsSurface);
					fpsSurface = RenderText_Solid(cour, "FPS: 0", clrFgBlack);

					SDL_FreeSurface(pingSurface);
					pingSurface = RenderText_Solid(cour, "Latency: 0 ms", clrFgBlack);

					return false;
				}

				if(OverButton((task->x + (task->bar->w - task->menu->w)), (task->y - task->menu->h), task->menu->w, task->menu->h, x, y))
					return false;
			}

			if(overAll)
				return false;
		}

		else
		{
			if(task->enableMenu)
				task->enableMenu = false;
		}

		if(iw.size() > 0)
		{
			for(int i = (iw.size() - 1); i >= 0; i--)
			{
				if(OverButton((iw.at(i)->x + (54 - (iw.at(i)->btn1->w / 2))), (iw.at(i)->y + (96 - (iw.at(i)->btn1->h / 2))), iw.at(i)->btn1->w, iw.at(i)->btn1->h, x, y))
				{
					if(iw.at(i)->inputstr.length() > 0)
					{
						if(atoi(iw.at(i)->inputstr.c_str()) <= iw.at(i)->arg2)
						{
							switch(iw.at(i)->type)
							{
								case INVENTORYREMOVEITEM: PacketInventoryRemoveItem(iw.at(i)->arg1, atoi(iw.at(i)->inputstr.c_str()), iw.at(i)->arg3);
									break;

								case INVENTORYDROPITEM: PacketDropItem(iw.at(i)->arg1, atoi(iw.at(i)->inputstr.c_str()), iw.at(i)->arg3);
									break;

								case NPCTRADESELL: PacketShopSell(iw.at(i)->arg1, atoi(iw.at(i)->inputstr.c_str()), iw.at(i)->arg3);
									break;

								case PLAYERTRADEADD: PacketAddTradeItem(iw.at(i)->arg1, atoi(iw.at(i)->inputstr.c_str()), iw.at(i)->arg3);
									break;
							}
						}
					}

					iw.at(i)->Clean(i);

					delete iw.at(i);
					iw.erase((iw.begin() + i));

					if(iw.size() == 0)
						chatInputWindow = false;

					return false;
				}

				if(OverButton((iw.at(i)->x + (163 - (iw.at(i)->btn2->w / 2))), (iw.at(i)->y + (96 - (iw.at(i)->btn2->h / 2))), iw.at(i)->btn2->w, iw.at(i)->btn2->h, x, y) || OverButton((iw.at(i)->x + (iw.at(i)->win->w - iw.at(i)->quitx->w)), iw.at(i)->y, iw.at(i)->quitx->w, iw.at(i)->quitx->h, x, y))
				{
					iw.at(i)->Clean(i);

					delete iw.at(i);
					iw.erase((iw.begin() + i));

					if(iw.size() == 0)
						chatInputWindow = false;

					return false;
				}

				if(OverButton(iw.at(i)->x, iw.at(i)->y, iw.at(i)->win->w, iw.at(i)->win->w, x, y))
				{
					StartDragWindow(DRAGINPUTWINDOW, x, y, &iw.at(i)->x, &iw.at(i)->y, i, NULL, &iw.at(i)->win->w, &iw.at(i)->win->h);

					return false;
				}
			}
		}

		if(pms->pm.size() > 0)
		{
			for(int i = pms->pm.size() - 1; i >= 0; i--)
			{
				if(OverButton(pms->pm.at(i).x, pms->pm.at(i).y, pms->win->w, pms->win->h, x, y))
				{
					pms->marked = pms->pm.at(i).toname;

					if(!chatting && !chatWindow)
					{
						if(!chatMessage)
						{
							chatMessage = true;

							SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);
						}
					}
				}

				else
				{
					if(chatMessage)
					{
						chatMessage = false;

						if(!chatting && !chatWindow)
							SDL_EnableKeyRepeat(0, SDL_DEFAULT_REPEAT_INTERVAL);
					}
				}

				if(OverButton((pms->pm.at(i).x + (pms->win->w - pms->quitx->w)), pms->pm.at(i).y, pms->quitx->w, pms->quitx->h, x, y))
				{
					if(!chatting && !chatWindow)
					{
						if(chatMessage)
						{
							chatMessage = false;

							SDL_EnableKeyRepeat(0, SDL_DEFAULT_REPEAT_INTERVAL);
						}
					}

					pms->RemoveWindow(pms->pm.at(i).toname.c_str());

					return false;
				}

				if(OverButton(pms->pm.at(i).x + 225, pms->pm.at(i).y + 181, 55, 21, x, y))
				{
					PMessage *pm = pms->GetMarked();
					
					SDL_EnableKeyRepeat(0, SDL_DEFAULT_REPEAT_INTERVAL);

					if(pm->text.length() > 0)
					{
						PacketSendPM(pm->toname.c_str(), pm->text.c_str());

						pm->text = "";

						if(pm->textp != NULL)
							SDL_FreeSurface(pm->textp);

						pm->textp = NULL;

						pm->chatPlaceW = 0;
						pm->chatPlace = 0;
					}

					return false;
				}

				if(OverButton(pms->pm.at(i).x + 7, pms->pm.at(i).y + 27, 272, 146, x, y))
				{
					pms->pm.at(i).StartDrag(x, y);

					return false;
				}

				if(OverButton(pms->pm.at(i).x, pms->pm.at(i).y, pms->win->w, pms->win->h, x, y))
				{
					dragged.name = pms->pm.at(i).toname;
					StartDragWindow(DRAGPMSWINDOW, x, y, &pms->pm.at(i).x, &pms->pm.at(i).y, i, NULL, &pms->win->w, &pms->win->h);

					return false;
				}
			}
		}

		if(fw->enable)
		{
			bool hit = false;
			bool clickedFriend = false;
			int clickedI = -1;

			int wrote = 0;
			int display = 0;

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

						for(unsigned int y2 = 0; y2 < fw->tps.at(i).menu.size(); y2++)
						{
							if(OverButton(cX, cY, fw->tps.at(i).menu.at(y2).btn->w, fw->tps.at(i).menu.at(y2).btn->h, x, y))
							{
								PacketFriendMenu(fw->tps.at(i).name.c_str(), fw->tps.at(i).menu.at(y2).text);

								hit = true;
								
								break;
							}

							else

							cY += (fw->tps.at(i).menu.at(y2).btn->h + 1);
						}

						break;
					}

					else
					{
						if(OverButton(fw->x + 15, fw->y + 65 + display, fw->tps.at(i).namep->w, 16, x, y))
						{
							fw->tps.at(i).showMenu = true;
							clickedFriend = true;
							clickedI = i;

							hit = true;

							break;
						}
					}

					wrote++;
					display += 16;
				}
			}

			if(clickedI != -1)
			{
				for(unsigned int i = 0; i < fw->tps.size(); i++)
				{
					if(clickedI != i)
						fw->tps.at(i).showMenu = false;
				}
			}

			if(!clickedFriend)
			{
				for(unsigned int i = 0; i < fw->tps.size(); i++)
				{
					if(fw->tps.at(i).showMenu)
					{
						fw->tps.at(i).showMenu = false;
					}
				}
			}

			if(hit)
				return false;

			if(OverButton(fw->x + 16, fw->y + 282, 75, 12, x, y))
			{
				if(!chatting && !chatWindow)
				{
					Window *w = new Window();

					w->w = loadimage("Data/Interface/InviteParty-Window.png", false, 0, 0, 0);
					w->quitx = loadimage("Data/Interface/Quit-Button.png", true, 255, 255, 255);
					w->inviteButton = loadimage("Data/Interface/AddFriend-Button.png", true, 255, 255, 255);
					w->TYPE = FRIENDINPUTWINDOW;

					if(transWindow)
						SDL_SetAlpha(w->w, SDL_SRCALPHA | SDL_RLEACCEL, 128);

					w->x = (screen->w / 2 - w->w->w / 2);
					w->y = (screen->h / 2 - w->w->h / 2);

					w->enable = true;
					w->enableX = true;

					win.push_back(w);

					chatWindow = true;
				}

				return false;
			}

			if(OverButton((fw->x + (fw->win->w - fw->quitx->w)), fw->y, fw->quitx->w, fw->quitx->h, x, y))
			{
				fw->enable = false;

				return false;
			}

			if(OverButton(fw->x + 9, fw->y + 60, 186, 213, x, y))
			{
				fw->StartDrag(x, y);

				return false;
			}

			if(OverButton(fw->x, fw->y, fw->win->w, fw->win->h, x, y))
			{
				StartDragWindow(DRAGFRIENDWINDOW, x, y, &fw->x, &fw->y, 0, &fw->enable, &fw->win->w, &fw->win->h);

				return false;
			}
		}

		if(tp->enable)
		{
			for(unsigned int i = 0; i < tp->tps.size(); i++)
			{
				if(OverButton((tp->x + tp->tps.at(i).x), (tp->y + tp->tps.at(i).y), 250, 17, x, y))
				{
					PacketNPCTeleport(tp->npcId, (i + 1));

					tp->Clean();
					tp->enable = false;

					return false;
				}
			}

			if(OverButton((tp->x + (tp->win->w - tp->quitx->w)), tp->y, tp->quitx->w, tp->quitx->h, x, y))
			{
				tp->Clean();
				tp->enable = false;

				return false;
			}

			if(OverButton(tp->x, tp->y, tp->win->w, tp->win->h, x, y))
			{
				StartDragWindow(DRAGTELEPORTWINDOW, x, y, &tp->x, &tp->y, 0, &tp->enable, &tp->win->w, &tp->win->h);

				return false;
			}
		}

		if(ptrade->enable)
		{
			if(ptrade->side1 == me->id)
			{
				if(OverButton((ptrade->x + 62), (ptrade->y + 277), ptrade->ok->w, ptrade->ok->h, x, y))
				{
					PacketDoneTrade();

					return false;
				}
			}

			else if(ptrade->side2 == me->id)
			{
				if(OverButton((ptrade->x + 264), (ptrade->y + 277), ptrade->ok->w, ptrade->ok->h, x, y))
				{
					PacketDoneTrade();

					return false;
				}
			}

			if(OverButton((ptrade->x + 163), (ptrade->y + 277), ptrade->cancel->w, ptrade->cancel->h, x, y) || OverButton((ptrade->x + (ptrade->win->w - ptrade->quitx->w)), ptrade->y, ptrade->quitx->w, ptrade->quitx->h, x, y))
			{
				PacketCancelTrade();

				return false;
			}

			if(OverButton(ptrade->x, ptrade->y, ptrade->win->w, ptrade->win->h, x, y))
			{
				StartDragWindow(DRAGTRADEWINDOW, x, y, &ptrade->x, &ptrade->y, 0, &ptrade->enable, &ptrade->win->w, &ptrade->win->h);

				return false;
			}
		}

		bool clicked = false;

		for(unsigned int i = 0; i < npc.size(); i++)
		{
			if(npc.at(i)->showShop)
			{
				bool hit = false;

				if(OverButton(sw->x, sw->y, sw->win->w, sw->win->h, x, y) || (OverButton(inv->x, inv->y, inv->inv->w, inv->inv->h, x, y) && inv->enable))
				{
					if(OverButton((sw->x + 26), (sw->y + 52), 277, 227, x, y))
					{
						for(int n = 0; n < 20; n++)
						{
							if(sw->si[n].itemId != 0)
							{
								if(OverButton(sw->si[n].x, sw->si[n].y, sw->si[n].pic->w, sw->si[n].pic->h, x, y))
								{
									if(sw->cMark == (n + 1))
									{
										hit = true;

										break;
									}

									sw->cMark = (n + 1);

									char buf[10] = "";

									_itoa_s((sw->si[n].itemPrice * sw->si[n].itemQty), buf, 10, 10);

									if(sw->currPrice != NULL)
										SDL_FreeSurface(sw->currPrice);

									sw->currPrice = RenderText_Solid(cour, buf, clrFgBlack);

									_itoa_s(sw->si[n].itemQty, buf, 10, 10);

									if(sw->currQty != NULL)
										SDL_FreeSurface(sw->currQty);

									sw->currQty = RenderText_Solid(cour, buf, clrFgBlack);

									hit = true;

									break;
								}
							}
						}

						if(!hit)
						{
							if(sw->currPrice != NULL)
								SDL_FreeSurface(sw->currPrice);

							sw->currPrice = RenderText_Solid(cour, "0", clrFgBlack);

							if(sw->currQty != NULL)
								SDL_FreeSurface(sw->currQty);

							sw->currQty = RenderText_Solid(cour, "1", clrFgBlack);

							sw->cMark = 0;
						}
					}

					if(!hit)
					{
						if(OverButton((sw->x + 269), (sw->y + 283), 11, 11, x, y))
						{
							if(sw->cMark > 0)
							{
								if(sw->si[(sw->cMark - 1)].itemId != 0)
								{
									if(getItemStack(sw->si[(sw->cMark - 1)].itemId) > sw->si[(sw->cMark - 1)].itemQty)
									{
										sw->si[(sw->cMark - 1)].itemQty++;

										char buf[10] = "";

										_itoa_s(sw->si[(sw->cMark - 1)].itemQty, buf, 10, 10);

										if(sw->currQty != NULL)
											SDL_FreeSurface(sw->currQty);

										sw->currQty = RenderText_Solid(cour, buf, clrFgBlack);

										_itoa_s((sw->si[(sw->cMark - 1)].itemPrice * sw->si[(sw->cMark - 1)].itemQty), buf, 10, 10);

										if(sw->currPrice != NULL)
											SDL_FreeSurface(sw->currPrice);

										sw->currPrice = RenderText_Solid(cour, buf, clrFgBlack);

										hit = true;
									}
								}
							}
						}

						else if(OverButton((sw->x + 282), (sw->y + 283), 11, 11, x, y))
						{
							if(sw->cMark > 0)
							{
								if(sw->si[(sw->cMark - 1)].itemId != 0)
								{
									if(sw->si[(sw->cMark - 1)].itemQty > 1)
									{
										sw->si[(sw->cMark - 1)].itemQty--;

										char buf[10] = "";

										_itoa_s(sw->si[(sw->cMark - 1)].itemQty, buf, 10, 10);

										if(sw->currQty != NULL)
											SDL_FreeSurface(sw->currQty);

										sw->currQty = RenderText_Solid(cour, buf, clrFgBlack);

										_itoa_s((sw->si[(sw->cMark - 1)].itemPrice * sw->si[(sw->cMark - 1)].itemQty), buf, 10, 10);

										if(sw->currPrice != NULL)
											SDL_FreeSurface(sw->currPrice);

										sw->currPrice = RenderText_Solid(cour, buf, clrFgBlack);

										hit = true;
									}
								}
							}
						}

						else if(OverButton((sw->x + 245), (sw->y + 283), 24, 11, x, y))
						{
							if(sw->cMark > 0)
							{
								if(sw->si[(sw->cMark - 1)].itemId != 0)
								{
									sw->si[(sw->cMark - 1)].itemQty = getItemStack(sw->si[(sw->cMark - 1)].itemId);

									char buf[10] = "";

									_itoa_s(sw->si[(sw->cMark - 1)].itemQty, buf, 10, 10);

									if(sw->currQty != NULL)
										SDL_FreeSurface(sw->currQty);

									sw->currQty = RenderText_Solid(cour, buf, clrFgBlack);

									_itoa_s((sw->si[(sw->cMark - 1)].itemPrice * sw->si[(sw->cMark - 1)].itemQty), buf, 10, 10);

									if(sw->currPrice != NULL)
										SDL_FreeSurface(sw->currPrice);

									sw->currPrice = RenderText_Solid(cour, buf, clrFgBlack);

									hit = true;
								}
							}
						}
					}

					if(!hit)
					{
						if(OverButton((sw->x + 123), (sw->y + 315), 82, 25, x, y))
						{
							if(sw->cMark != 0)
							{
								if(sw->si[(sw->cMark - 1)].itemId != 0)
								{
									int taken = 0;

									for(int op = 0; op < 20; op++)
									{
										if(sw->cart[op].itemId != 0)
											taken++;
									}

									if(taken < 16)
										PacketShopAdd(sw->cMark, sw->si[(sw->cMark - 1)].itemQty, sw->npcId);

									hit = true;
								}
							}
						}
					}

					if(!hit)
					{
						if(OverButton((sw->x + 221), (sw->y + 315), 82, 25, x, y))
						{
							vector<int> itemId;
							vector<int> itemQty;

							for(int i = 0; i < 20; i++)
							{
								if(sw->cart[i].itemId != 0)
								{
									itemId.push_back(sw->cart[i].itemId);
									itemQty.push_back(sw->cart[i].itemQty);
								}
							}

							if(itemId.size() > 0)
								PacketShopBuy(itemId, itemQty, sw->npcId);

							hit = true;
						}
					}

					clicked = true;
				}

				if(OverButton(((sw->x + sw->win->w) - sw->quitx->w), sw->y, sw->quitx->w, sw->quitx->h, x, y))
				{
					clicked = false;
					hit = true;
				}

				else
				{
					if(!hit)
					{
						if(OverButton(sw->x, sw->y, sw->win->w, sw->win->h, x, y))
						{
							StartDragWindow(DRAGNPCSHOPWINDOW, x, y, &sw->x, &sw->y, 0, &sw->enable, &sw->win->w, &sw->win->h);

							clicked = true;
							hit = true;
						}
					}
				}

				if(!clicked && (mark->enable && mark->marktype == 3))
				{
					mark->enable = false;
					mark->marktype = 0;
					mark->npcmark = 0;

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

						if(npc.at(i)->showShop)
						{
							npc.at(i)->showShop = false;

							sw->Clean();
							sw->enable = false;
						}
					}

					for(unsigned int i = 0; i < cv.size(); i++)
					{
						if(cv.at(i)->showMenu)
							cv.at(i)->showMenu = false;
					}
				}

				if(hit)
					return false;

				break;
			}
		}

		if(inv->enable)
		{
			if(!drag)
			{
				for(int i = 0; i < 25; i++)
				{
					if(inv->items[i] != NULL)
					{
						if(OverButton(inv->items[i]->x, inv->items[i]->y, inv->items[i]->pic->w, inv->items[i]->pic->h, x, y))
						{
							dragged.type = INVENTORYITEM;
							dragged.id = inv->items[i]->id;
							dragged.x = inv->items[i]->x;
							dragged.y = inv->items[i]->y;
							dragged.pic = inv->items[i]->pic;
							dragged.slot = (i + 1);
							dragged.qty = inv->items[i]->stack;

							drag = true;

							return false;
						}
					}
				}

				if(OverButton((inv->x + 18), (inv->y + 256), 187, 16, x, y))
				{
					dragged.type = INVENTORYITEM;
					dragged.id = 2;
					dragged.pic = getIconSurface(2);
					dragged.x = (x - (dragged.pic->w / 2));
					dragged.y = (y - (dragged.pic->h / 2));
					dragged.slot = 0;
					dragged.qty = me->gold;

					drag = true;

					return false;
				}
			}

			if(OverButton(((inv->x + inv->inv->w) - inv->quitx->w), inv->y, inv->quitx->w, inv->quitx->h, x, y))
			{
				inv->enable = false;

				return false;
			}

			if(OverButton(inv->x, inv->y, inv->inv->w, inv->inv->h, x, y))
			{
				StartDragWindow(DRAGINVENTORYWINDOW, x, y, &inv->x, &inv->y, 0, &inv->enable, &inv->inv->w, &inv->inv->h);

				return false;
			}
		}

		if(sk->enable)
		{
			if(!drag)
			{
				for(unsigned int i = 0; i < skill.size(); i++)
				{
					if(OverButton(((sk->x + skill.at(i)->x) - 67), ((sk->y + skill.at(i)->y) - 44), 100, 64, x, y))
					{
						dragged.type = SKILL;
						dragged.pic = skill.at(i)->pic;
						dragged.id = skill.at(i)->id;
						dragged.x = (x - (dragged.pic->w / 2));
						dragged.y = (y - (dragged.pic->h / 2));
						dragged.slot = (i + 1);

						drag = true;

						return false;
					}
				}

				/*
				for(int i = 0; i < skillLoaded; i++)
				{
					if(OverButton(((sk->x + skills[i].x) - 67), ((sk->y + skills[i].y) - 44), 100, 64, x, y))
					{
						dragged.type = SKILL;
						dragged.pic = skills[i].pic;
						dragged.id = skills[i].id;
						dragged.x = (x - (dragged.pic->w / 2));
						dragged.y = (y - (dragged.pic->h / 2));
						dragged.slot = (i + 1);

						drag = true;

						return false;
					}
				}
				*/
			}

			if(OverButton((sk->x + (sk->win->w - sk->quitx->w)), sk->y, sk->quitx->w, sk->quitx->h, x, y))
			{
				sk->enable = false;

				return false;
			}

			if(OverButton(sk->x, sk->y, sk->win->w, sk->win->h, x, y))
			{
				StartDragWindow(DRAGSKILLWINDOW, x, y, &sk->x, &sk->y, 0, &sk->enable, &sk->win->w, &sk->win->h);

				return false;
			}
		}

		if(icons.size() > 0)
		{
			if(OverButton(icons.at(0)->x, icons.at(0)->y, (icons.at(0)->icon->w + ((icons.size() - 1) * icons.at(0)->icon->w)), icons.at(0)->icon->h, x, y))
			{
				BuffSize = (icons.at(0)->icon->w + ((icons.size() - 1) * icons.at(0)->icon->w));

				StartDragWindow(DRAGBUFFS, x, y, &icons.at(0)->x, &icons.at(0)->y, 0, NULL, &BuffSize, &icons.at(0)->icon->h);

				return false;
			}
		}

		if(msgs.size() > 0)
		{
			for(unsigned int i = 0; i < msgs.size(); i++)
			{
				for(unsigned int o = 0; o < msgs.at(i)->btn.size(); o++)
				{
					Button *b = msgs.at(i)->btn.at(o);

					if(OverButton(b->x, b->y, b->sur->w, b->sur->h, x, y))
					{
						switch(b->effect)
						{
							case DISCONNECTED:
								{
									altf = true;
									return true;
								break;
								}
						}

						return false;
					}
				}

				if(OverButton(msgs.at(i)->x, msgs.at(i)->y, msgs.at(i)->win->w, msgs.at(i)->win->h, x, y))
				{
					StartDragWindow(DRAGMSGWINDOW, x, y, &msgs.at(i)->x, &msgs.at(i)->y, i, NULL, &msgs.at(i)->win->w, &msgs.at(i)->win->h);

					return false;
				}
			}
		}

		if(pi.size() > 0)
		{
			for(int i = (pi.size() - 1); i >= 0; i--)
			{
				PartyInvite *l = pi.at(i);

				if(OverButton((l->x + 15), (l->y + 83), l->yes->w, l->yes->h, x, y))
				{
					if(l->type == PARTY)
					{
						PacketAcceptParty(1, pi.at(0)->name);
						partyw.at(0)->enablePress = true;
					}

					else if(l->type == TRADE)
						PacketAcceptTradeInvite(1, l->nameid);

					else if(l->type == FRIEND)
						PacketAcceptFriendInvite(1, l->nameid);

					if(i == dragged.id && dragged.type == DRAGPARTINVITE)
						drag = false;

					SDL_FreeSurface(l->win);
					SDL_FreeSurface(l->from);
					SDL_FreeSurface(l->yes);
					SDL_FreeSurface(l->no);

					delete pi.at(i);
					pi.erase(pi.begin() + i);

					return false;
				}

				else if(OverButton((l->x + 128), (l->y + 83), l->no->w, l->no->h, x, y))
				{
					if(l->type == PARTY)
					{
						PacketAcceptParty(2, pi.at(0)->name);
						partyw.at(0)->enablePress = true;
					}

					else if(l->type == TRADE)
						PacketAcceptTradeInvite(2, l->nameid);

					else if(l->type == FRIEND)
						PacketAcceptFriendInvite(2, l->nameid);

					if(i == dragged.id && dragged.type == DRAGPARTINVITE)
						drag = false;

					SDL_FreeSurface(l->win);
					SDL_FreeSurface(l->from);
					SDL_FreeSurface(l->yes);
					SDL_FreeSurface(l->no);

					delete pi.at(i);
					pi.erase(pi.begin() + i);

					return false;
				}

				else if(OverButton(l->x, l->y, l->win->w, l->win->h, x, y))
				{
					StartDragWindow(DRAGPARTINVITE, x, y, &l->x, &l->y, i, NULL, &l->win->w, &l->win->h);

					return false;
				}
			}
		}

		if(addstatwindow.size() > 0)
		{
			if(addstatwindow.at(0)->enable)
			{
				AddStatWindow *l = addstatwindow.at(0);

				if(me->pAv > 0)
				{
					if(OverButton((l->x + 153), (l->y + 43), l->add->w, l->add->h, x, y))
					{
						PacketGameAddStat(STR);

						return false;
					}

					else if(OverButton((l->x + 153), (l->y + 75), l->add->w, l->add->h, x, y))
					{
						PacketGameAddStat(STA);

						return false;
					}

					else if(OverButton((l->x + 153), (l->y + 107), l->add->w, l->add->h, x, y))
					{
						PacketGameAddStat(22);

						return false;
					}

					else if(OverButton((l->x + 153), (l->y + 140), l->add->w, l->add->h, x, y))
					{
						PacketGameAddStat(23);

						return false;
					}
				}

				if(OverButton(((l->x + l->window->w) - l->quitx->w), l->y, l->quitx->w, l->quitx->h, x, y))
				{
					l->enable = false;

					return false;
				}

				if(OverButton(l->x, l->y, l->window->w, l->window->h, x, y))
				{
					StartDragWindow(DRAGADDSTATWINDOW, x, y, &l->x, &l->y, 0, &l->enable, &l->window->w, &l->window->h);

					return false;
				}
			}
		}

		if(option->enable)
		{
			if(OverButton(((option->x + option->win->w) - (option->quitx->w + 2)), option->y, option->quitx->w, option->quitx->h, x, y))
			{
				option->enable = false;

				return false;
			}

			int startX = (option->x + 186);
			int startY = (option->y + 62);

			for(unsigned int i = 0; i < option->op.size(); i++)
			{
				if(OverButton(startX, startY, option->buttonFull->w, option->buttonFull->h, x, y))
				{
					if(option->op.at(i).active)
					{
						if(option->op.at(i).effect == TRANS)
						{
							SetTrans(false);

							option->op.at(i).active = false;
							transWindow = false;

							AddConfigOption(TRANS, NULL, 0, 0, ONEINT, "Transparency:");
						}

						else if(option->op.at(i).effect == SOUND)
						{
							noSound = true;
							option->op.at(i).active = false;

							if(Mix_Playing(-1) != 0)
								Mix_Pause(-1);

							AddConfigOption(SOUND, NULL, 0, 0, ONEINT, "Sound:");
						}

						else if(option->op.at(i).effect == MUSIC)
						{
							noMusic = true;
							option->op.at(i).active = false;

							if(Mix_PlayingMusic() == 1)
								Mix_PauseMusic();

							AddConfigOption(MUSIC, NULL, 0, 0, ONEINT, "Music:");
						}
					}

					else
					{
						if(option->op.at(i).effect == TRANS)
						{
							SetTrans(true);

							option->op.at(i).active = true;
							transWindow = true;

							AddConfigOption(TRANS, NULL, 1, 0, ONEINT, "Transparency:");
						}

						else if(option->op.at(i).effect == SOUND)
						{
							noSound = false;
							option->op.at(i).active = true;

							if(Mix_Paused(-1) != 0)
								Mix_Resume(-1);

							AddConfigOption(SOUND, NULL, 1, 0, ONEINT, "Sound:");
						}

						else if(option->op.at(i).effect == MUSIC)
						{
							noMusic = false;
							option->op.at(i).active = true;

							if(Mix_PausedMusic() == 1)
								Mix_ResumeMusic();

							else
								music->playCurrent();

							AddConfigOption(MUSIC, NULL, 1, 0, ONEINT, "Music:");
						}
					}

					return false;
				}

				startY += 32;
			}

			if(OverButton(option->x, option->y, option->win->w, option->win->h, x, y))
			{
				StartDragWindow(DRAGOPTIONWINDOW, x, y, &option->x, &option->y, 0, &option->enable, &option->win->w, &option->win->h);

				return false;
			}
		}

		if(win.size() > 0)
		{
			for(unsigned int i = 0; i < win.size(); i++)
			{
				if(win.at(i)->enable)
				{
					if(win.at(i)->enableX)
					{
						if(win.at(i)->TYPE == INVITEINPUTWINDOW)
						{
							if(OverButton((win.at(i)->x + 95), (win.at(i)->y + 68), win.at(i)->inviteButton->w, win.at(i)->inviteButton->h, x, y))
							{
								if(partyw.size() > 0)
								{
									chatting = false;
									chatWindow = false;

									if(chat != "")
									{
										PacketInviteToParty((char*)chat.c_str());

										chat = "";
									}

									partyw.at(0)->enablePress = true;

									SDL_FreeSurface(win.at(i)->inviteButton);
									SDL_FreeSurface(win.at(i)->quitx);
									SDL_FreeSurface(win.at(i)->w);

									if(i == dragged.id && dragged.type == DRAGWINDOW)
										drag = false;

									delete win.at(i);
									win.erase(win.begin() + i);

									return false;
								}
							}
						}

						else if(win.at(i)->TYPE == FRIENDINPUTWINDOW)
						{
							if(OverButton((win.at(i)->x + 95), (win.at(i)->y + 68), win.at(i)->inviteButton->w, win.at(i)->inviteButton->h, x, y))
							{
								if(partyw.size() > 0)
								{
									chatting = false;
									chatWindow = false;

									if(chat != "")
									{
										PacketChangeFriend(1, chat.c_str());

										chat = "";
									}

									SDL_FreeSurface(win.at(i)->inviteButton);
									SDL_FreeSurface(win.at(i)->quitx);
									SDL_FreeSurface(win.at(i)->w);

									if(i == dragged.id && dragged.type == DRAGWINDOW)
										drag = false;

									delete win.at(i);
									win.erase(win.begin() + i);

									return false;
								}
							}
						}

						else if(win.at(i)->TYPE == KICKINPUTWINDOW)
						{
							if(OverButton((win.at(i)->x + 95), (win.at(i)->y + 68), win.at(i)->inviteButton->w, win.at(i)->inviteButton->h, x, y))
							{
								if(partyw.size() > 0)
								{
									chatting = false;
									chatWindow = false;

									if(chat != "")
									{
										PacketKickFromParty((char*)chat.c_str());

										chat = "";
									}

									partyw.at(0)->enablePress = true;

									SDL_FreeSurface(win.at(i)->inviteButton);
									SDL_FreeSurface(win.at(i)->quitx);
									SDL_FreeSurface(win.at(i)->w);

									if(i == dragged.id && dragged.type == DRAGWINDOW)
										drag = false;

									delete win.at(i);
									win.erase(win.begin() + i);

									return false;
								}
							}
						}

						if(OverButton(((win.at(i)->x + win.at(i)->w->w) - (win.at(i)->quitx->w + 2)), win.at(i)->y, win.at(i)->quitx->w, win.at(i)->quitx->h, x, y))
						{
							if(win.at(i)->TYPE == INVITEINPUTWINDOW || win.at(i)->TYPE == KICKINPUTWINDOW || win.at(i)->TYPE == FRIENDINPUTWINDOW)
							{
								if(partyw.size() > 0)
								{
									if(win.at(i)->TYPE != FRIENDINPUTWINDOW)
										partyw.at(0)->enablePress = true;

									SDL_FreeSurface(win.at(i)->inviteButton);
									SDL_FreeSurface(win.at(i)->quitx);
									SDL_FreeSurface(win.at(i)->w);

									if(i == dragged.id && dragged.type == DRAGWINDOW)
										drag = false;

									delete win.at(i);
									win.erase(win.begin() + i);
								}

								chatting = false;
								chatWindow = false;

								chat = "";
							}

							else
								win.at(i)->enable = false;

							return false;
						}
					}

					if(win.at(i)->TYPE == CHATWINDOW)
					{
						if(OverButton(win.at(i)->x + 5, win.at(i)->y + 6, 444, 79, x, y))
						{
							chatFunc->StartDrag(x, y);

							return false;
						}
					}

					if(OverButton(win.at(i)->x, win.at(i)->y, win.at(i)->w->w, win.at(i)->w->h, x, y))
					{
						StartDragWindow(DRAGWINDOW, x, y, &win.at(i)->x, &win.at(i)->y, i, NULL, &win.at(i)->w->w, &win.at(i)->w->h);

						return false;
					}
				}
			}
		}

		if(partyw.size() > 0)
		{
			if(partyw.at(0)->enable && partyw.at(0)->enablePress)
			{
				PartyWindow *l = partyw.at(0);

				if(OverButton((l->x + 7), (l->y + 188), l->leave->w, l->leave->h, x, y))
				{
					PacketLeaveParty();

					return false;
				}

				else if(OverButton((l->x + 146), (l->y + 188), l->kick->w, l->kick->h, x, y))
				{
					if(!chatting && !chatWindow)
					{
						Window *w = new Window();

						w->w = loadimage("Data/Interface/InviteParty-Window.png", false, 0, 0, 0);
						w->quitx = loadimage("Data/Interface/Quit-Button.png", true, 255, 255, 255);
						w->inviteButton = loadimage("Data/Interface/KickParty-Button.png", true, 255, 255, 255);
						w->TYPE = KICKINPUTWINDOW;

						if(transWindow)
							SDL_SetAlpha(w->w, SDL_SRCALPHA | SDL_RLEACCEL, 128);

						w->x = (l->x + 42);
						w->y = (l->y + 53);

						w->enable = true;
						w->enableX = true;

						win.push_back(w);

						l->enablePress = false;
						chatWindow = true;
					}

					return false;
				}

				else if(OverButton((l->x + 284), (l->y + 188), l->invite->w, l->invite->h, x, y))
				{
					if(!chatting && !chatWindow)
					{
						Window *w = new Window();

						w->w = loadimage("Data/Interface/InviteParty-Window.png", false, 0, 0, 0);
						w->quitx = loadimage("Data/Interface/Quit-Button.png", true, 255, 255, 255);
						w->inviteButton = loadimage("Data/Interface/InviteParty-Button.png", true, 255, 255, 255);
						w->TYPE = INVITEINPUTWINDOW;

						if(transWindow)
							SDL_SetAlpha(w->w, SDL_SRCALPHA | SDL_RLEACCEL, 128);

						w->x = (l->x + 42);
						w->y = (l->y + 53);

						w->enable = true;
						w->enableX = true;

						win.push_back(w);

						l->enablePress = false;
						chatWindow = true;
					}

					return false;
				}

				else if(OverButton((l->x + (l->window->w - l->quitx->w)), l->y, l->quitx->w, l->quitx->h, x, y))
				{
					l->enable = false;

					return false;
				}

				else if(OverButton(l->x, l->y, l->window->w, l->window->h, x, y))
				{
					StartDragWindow(DRAGPARTYWINDOW, x, y, &l->x, &l->y, 0, &l->enable, &l->window->w, &l->window->h);

					return false;
				}
			}
		}

		if(npc.size() > 0)
		{
			for(unsigned int i = 0; i < npc.size(); i++)
			{
				if(mark->enable && mark->marktype == 3 && mark->npcmark == npc.at(i)->id)
				{
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

						for(unsigned int o = 0; o < npc.at(i)->menu.size(); o++)
						{
							if(OverButton(cX, cY, npc.at(i)->menu.at(o).btn->w, npc.at(i)->menu.at(o).btn->h, x, y))
							{
								PacketNPCMenu(npc.at(i)->id, (o + 1));

								npc.at(i)->showMenu = false;

								clicked = true;

								break;
							}

							cY += (npc.at(i)->menu.at(o).btn->h + 1);
						}
					}

					if(OverButton(npc.at(i)->cX, npc.at(i)->cY, npc.at(i)->my->w, npc.at(i)->my->h, x, y))
					{
						for(unsigned int p = 0; p < npc.size(); p++)
						{
							if(npc.at(p)->showMenu)
								npc.at(p)->showMenu = false;

							if(npc.at(p)->showDialog)
							{
								for(unsigned int y = 0; y < npc.at(p)->npct.size(); y++)
									SDL_FreeSurface(npc.at(p)->npct.at(y).text);

								npc.at(p)->npct.clear();

								npc.at(p)->showDialog = false;
							}
						}

						MarkMe(npc.at(i));

						clicked = true;
					}

					if(npc.at(i)->showDialog)
					{
						if(OverButton(npc.at(i)->dialogX, npc.at(i)->dialogY, npc.at(i)->dialogWindow->w, npc.at(i)->dialogWindow->h, x, y))
						{
							int xb = (npc.at(i)->dialogX + 24);
							int yb = (npc.at(i)->dialogY + 149);

							for(unsigned int t = 0; t < npc.at(i)->diab.size(); t++)
							{
								if(OverButton(xb, yb, npc.at(i)->diab.at(t).btn->w, npc.at(i)->diab.at(t).btn->h, x, y))
								{
									PacketHitDialogButton(npc.at(i)->id, (t + 1));

									break;
								}

								xb += (npc.at(i)->diab.at(t).btn->w + 10);
							}

							clicked = true;
						}

						if(OverButton(((npc.at(i)->dialogX + npc.at(i)->dialogWindow->w) - npc.at(i)->quitx->w), npc.at(i)->dialogY, npc.at(i)->quitx->w, npc.at(i)->quitx->h, x, y))
							clicked = false;
					}
				}

				else
				{
					if(OverButton(npc.at(i)->cX, npc.at(i)->cY, npc.at(i)->my->w, npc.at(i)->my->h, x, y))
					{
						MarkMe(npc.at(i));

						clicked = true;
					}
				}
			}
		}

		if(!clicked && (mark->enable && mark->marktype == 3))
		{
			mark->enable = false;
			mark->marktype = 0;
			mark->npcmark = 0;

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

				if(npc.at(i)->showShop)
				{
					npc.at(i)->showShop = false;

					sw->Clean();
					sw->enable = false;
				}
			}

			for(unsigned int i = 0; i < cv.size(); i++)
			{
				if(cv.at(i)->showMenu)
					cv.at(i)->showMenu = false;
			}
		}

		bool clickedPlayer = false;

		if(!clicked)
		{
			for(unsigned int i = 0; i < cv.size(); i++)
			{
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
					
					for(unsigned int o = 0; o < cv.at(i)->menu.size(); o++)
					{
						if(OverButton(cX, cY, cv.at(i)->menu.at(o).btn->w, cv.at(i)->menu.at(o).btn->h, x, y))
						{
							PacketCharacterMenuButton(cv.at(i)->menu.at(o).text, cv.at(i)->id);

							cv.at(i)->showMenu = false;

							clickedPlayer = true;

							break;
						}

						cY += (cv.at(i)->menu.at(o).btn->h + 1);
					}

					if(OverButton(cv.at(i)->cX, cv.at(i)->cY, cv.at(i)->my->w, cv.at(i)->my->h, x, y))
					{
						MarkMePlayer(cv.at(i));

						clickedPlayer = true;
					}

					for(unsigned int o = (i + 1); o < cv.size(); o++)
						cv.at(o)->showMenu = false;

					break;
				}

				else
				{
					if(OverButton(cv.at(i)->cX, cv.at(i)->cY, cv.at(i)->my->w, cv.at(i)->my->h, x, y))
					{
						cv.at(i)->showMenu = true;

						clickedPlayer = true;

						for(unsigned int o = (i + 1); o < cv.size(); o++)
							cv.at(o)->showMenu = false;

						MarkMePlayer(cv.at(i));
					}
				}
			}
		}

		if(!clickedPlayer && (mark->enable && mark->marktype == 1))
		{
			mark->enable = false;
			mark->marktype = 0;
			mark->npcmark = 0;

			for(unsigned int i = 0; i < cv.size(); i++)
			{
				if(cv.at(i)->showMenu)
					cv.at(i)->showMenu = false;
			}
		}
	}

	else if(event.button.button == SDL_BUTTON_RIGHT)
	{
		int x = event.button.x;
		int y = event.button.y;

		lastMouseX = event.button.x;
		lastMouseY = event.button.y;
		
		bool hit = false;

		if(inv->enable && !drag)
		{
			for(int i = 0; i < 25; i++)
			{
				if(inv->items[i] != NULL)
				{
					if(OverButton(inv->items[i]->x, inv->items[i]->y, inv->items[i]->pic->w, inv->items[i]->pic->h, x, y))
					{
						if(sw->enable)
							CreateInputWindow(NPCTRADESELL, 3, (i + 1), inv->items[i]->stack, inv->items[i]->id);

						else
							PacketInventoryUseItem((i + 1));

						return false;
					}
				}
			}

			for(int i = 0; i < 4; i++)
			{
				if(inv->eq[i] != NULL)
				{
					if(OverButton((inv->x + getEquipPosX(inv->eq[i]->slot)), (inv->y + getEquipPosY(inv->eq[i]->slot)), inv->eq[i]->pic->w, inv->eq[i]->pic->h, x, y))
					{
						if(!sw->enable)
							PacketUnEquip(inv->eq[i]->slot);

						else
							PacketErrorMessage("You can not modify your equip inventory while trading.");

						return false;
					}
				}
			}
		}

		for(int i = 0; i < 9; i++)
		{
			if(OverButton(task->pic[i]->x, task->pic[i]->y, 40, 31, x, y))
			{
				PacketRemoveTaskbarItem((i + 1));

				return false;
			}
		}

		if(sw->enable)
		{
			if(OverButton((sw->x + 26), (sw->y + 352), 277, 70, x, y))
			{
				for(int i = 0; i < 20; i++)
				{
					if(sw->cart[i].itemId != 0)
					{
						if(OverButton(sw->cart[i].x, sw->cart[i].y, sw->cart[i].pic->w, sw->cart[i].pic->h, x, y))
						{
							PacketShopRemove((i + 1));

							return false;
						}
					}
				}
			}
		}
	}

	return false;
}

void HandleMouseUp(SDL_Event event)
{
	if(drag && event.button.button == SDL_BUTTON_LEFT)
	{
		drag = false;

		if(dragged.type == INVENTORYITEM)
		{
			int cX = (inv->x + 17);
			int cY = (inv->y + 284);

			int rad = 1;

			for(int i = 0; i < 25; i++)
			{
				if(OverButton(cX, cY, 43, 36, (dragged.x + (dragged.pic->w / 2)), (dragged.y + (dragged.pic->h / 2))))
				{
					if(dragged.slot != (i + 1) && dragged.id != 2)
						PacketInventoryMoveItem(dragged.slot, (i + 1));

					return;
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

			if(OverButton((inv->x + 187), (inv->y + 13), 24, 27, (dragged.x + (dragged.pic->w / 2)), (dragged.y + (dragged.pic->h / 2))))
			{
				CreateInputWindow(INVENTORYREMOVEITEM, 3, dragged.slot, dragged.qty, dragged.id);

				return;
			}

			for(int i = 0; i < 9; i++)
			{
				if(OverButton(task->pic[i]->x, task->pic[i]->y, 40, 31, (dragged.x + (dragged.pic->w / 2)), (dragged.y + (dragged.pic->h / 2))))
				{
					if(dragged.id != 2)
						PacketAddTaskbarItem((i + 1), dragged.type, dragged.slot);

					return;
				}
			}

			if(ptrade->enable)
			{
				int xStart = (ptrade->x + 36);
				int yStart = (ptrade->y + 117);

				if(ptrade->side2 == me->id)
					xStart = (ptrade->x + 238);

				if(OverButton(xStart, yStart, 137, 138, (dragged.x + (dragged.pic->w / 2)), (dragged.y + (dragged.pic->h / 2))))
				{
					CreateInputWindow(PLAYERTRADEADD, 3, dragged.slot, dragged.qty, dragged.id);

					return;
				}
			}

			if(sw->enable)
			{
				if(OverButton((sw->x + 26), (sw->y + 52), 276, 226, (dragged.x + (dragged.pic->w / 2)), (dragged.y + (dragged.pic->h / 2))))
				{
					CreateInputWindow(NPCTRADESELL, 3, dragged.slot, dragged.qty, dragged.id);

					return;
				}
			}

			if(OnMap((dragged.x + (dragged.pic->w / 2)), (dragged.y + (dragged.pic->h / 2))))
			{
				CreateInputWindow(INVENTORYDROPITEM, 3, dragged.slot, dragged.qty, dragged.id);

				return;
			}
		}

		else if(dragged.type == SKILL)
		{
			for(int i = 0; i < 9; i++)
			{
				if(OverButton(task->pic[i]->x, task->pic[i]->y, 40, 31, (dragged.x + (dragged.pic->w / 2)), (dragged.y + (dragged.pic->h / 2))))
				{
					PacketAddTaskbarItem((i + 1), dragged.type, dragged.id);

					return;
				}
			}
		}
	}
}

bool HandleKeyLogin(SDL_Event event)
{
	if(keys[SDLK_LALT])
	{
		if(keys[SDLK_F4])
			return true;
	}

	if(isEvent == 0)
		return false;

	if(event.key.keysym.sym <= 0 || event.key.keysym.sym >= 365)
		return false;

	if(keys[SDLK_RETURN])
	{
		if((unsigned)nextEnter <= getTime(true))
		{
			if(lmsgs.size() > 0)
			{
				lmsgs.at(0)->Clean();
				delete lmsgs.at(0);
				lmsgs.erase(lmsgs.begin());
			}

			else if(linputs.size() > 0)
			{
				if(logplace == CHARSELECT)
				{
					switch(linputs.at(0)->t)
					{
						case CREATECHARACTER:
							{
								if(strlen(write.GetText(1)) <= 0)
									LMessage(6, "Please input a name.");

								else if(strlen(write.GetText(1)) >= 16)
									LMessage(6, "Character name is too long.");

								else
								{
									PacketCharacterChange(1, (char*)write.GetText(1), NULL, linputs.at(0)->i, job);
									wantPacket = true;
									lastSent = 0x7;
								}

								write.StopWrite();
								job = 1;
							break;
							}

						case DELETECHARACTER:
							{
								if(strlen(write.GetText(1)) <= 0)
									LMessage(6, "Please input a password.");

								else
								{
									PacketCharacterChange(2, (char*)charsel.at((linputs.at(0)->i - 1)).name2.c_str(), (char*)write.GetText(1), 0, 0);
									wantPacket = true;
									lastSent = 0x8;
								}

								write.StopWrite();
							break;
							}
					}
				}

				if(linputs.at(0)->win != NULL)
					SDL_FreeSurface(linputs.at(0)->win);

				delete linputs.at(0);
				linputs.erase(linputs.begin());
			}

			else
			{
				if(logplace == LOGINSCREEN)
				{
					if(strlen(write.GetText(1)) <= 0)
					{
						LMessage(5, "Please input a username.");
					}

					else if(strlen(write.GetText(2)) <= 0)
					{
						LMessage(5, "Please input a password.");
					}

					else
					{
						PacketLogin((char*)write.GetText(1), (char*)write.GetText(2), 1);

						wantPacket = true;
					}
				}

				else if(logplace == CHARSELECT)
				{
				}
			}

			nextEnter = (getTime(true) + 200);
		}
	}

	if(write.enable)
	{
		if(isEvent == 0)
			return false;

		if(event.key.keysym.sym <= 0 || event.key.keysym.sym >= 365)
			return false;

		if(keys[SDLK_TAB] && logplace == LOGINSCREEN)
			write.ChangeFocus(3);

		if(keys[SDLK_BACKSPACE])
		{
			if(write.carea == 1)
			{
				if(strlen(write.GetText(1)) > 0)
				{
					write.ar1s.erase(write.ar1s.length() - 1);
					write.NewSurface(1);
				}
			}

			else if(write.carea == 2)
			{
				if(strlen(write.GetText(2)) > 0)
				{
					write.ar2s.erase(write.ar2s.length() - 1);
					write.NewSurface(2);
				}
			}
		}

		if(write.carea == 1)
		{
			if(strlen(write.GetText(1)) >= 19)
				return false;
		}

		else
		{
			if(strlen(write.GetText(2)) >= 19)
				return false;
		}

		string ops = "";
		AddChatText(event, ops, 1);

		if(write.carea == 1)
		{
			write.ar1s += ops;
			write.NewSurface(1);
		}

		else
		{
			write.ar2s += ops;
			write.NewSurface(2);
		}
	}

	return false;
}

bool HandleKey(SDL_Event event, bool &move)
{
	if(mark->enable && !keys[SDLK_i] && !keys[SDLK_RETURN] && !chatInputWindow && !keys[SDLK_r])
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

			if(npc.at(i)->showShop)
			{
				npc.at(i)->showShop = false;

				sw->Clean();
				sw->enable = false;
			}
		}

		for(unsigned int i = 0; i < cv.size(); i++)
		{
			if(cv.at(i)->showMenu)
				cv.at(i)->showMenu = false;
		}

		for(unsigned int i = 0; i < fw->tps.size(); i++)
		{
			if(fw->tps.at(i).showMenu)
				fw->tps.at(i).showMenu = false;
		}
	}

	if(keys[SDLK_LALT])
	{
		if(keys[SDLK_F4])
			return true;
	}

	if(chatting || chatWindow || (chatInputWindow && iw.size() > 0) || (chatMessage && pms->pm.size() > 0))
	{
		bool nopls = true;

		if(keys[SDLK_RETURN])
		{
			if((unsigned)nextEnter <= getTime(true))
			{
				if(chatInputWindow && iw.size() > 0)
				{
					if(iw.back()->inputstr.length() > 0)
					{
						if(atoi(iw.back()->inputstr.c_str()) <= iw.back()->arg2)
						{
							switch(iw.back()->type)
							{
								case INVENTORYREMOVEITEM: PacketInventoryRemoveItem(iw.back()->arg1, atoi(iw.back()->inputstr.c_str()), iw.back()->arg3);
									break;

								case INVENTORYDROPITEM: PacketDropItem(iw.back()->arg1, atoi(iw.back()->inputstr.c_str()), iw.back()->arg3);
									break;

								case NPCTRADESELL: PacketShopSell(iw.back()->arg1, atoi(iw.back()->inputstr.c_str()), iw.back()->arg3);
									break;

								case PLAYERTRADEADD: PacketAddTradeItem(iw.back()->arg1, atoi(iw.back()->inputstr.c_str()), iw.back()->arg3);
									break;
							}
						}
					}

					iw.back()->Clean((iw.size() - 1));

					delete iw.back();
					iw.erase((iw.begin() + (iw.size() - 1)));

					if(iw.size() == 0)
						chatInputWindow = false;

					SDL_EnableKeyRepeat(0, SDL_DEFAULT_REPEAT_INTERVAL);
				}

				else if((chatMessage && pms->pm.size() > 0))
				{
					PMessage *pm = pms->GetMarked();
					
					SDL_EnableKeyRepeat(0, SDL_DEFAULT_REPEAT_INTERVAL);

					if(pm->text.length() > 0)
					{
						PacketSendPM(pm->toname.c_str(), pm->text.c_str());

						pm->text = "";

						if(pm->textp != NULL)
							SDL_FreeSurface(pm->textp);

						pm->textp = NULL;

						pm->chatPlaceW = 0;
						pm->chatPlace = 0;
					}
				}

				else if(chatting)
				{
					chatting = false;
					chatWindow = false;

					SDL_EnableKeyRepeat(0, SDL_DEFAULT_REPEAT_INTERVAL);

					if(chat != "")
					{
						PacketSendChat((char*)chat.c_str());

						chat = "";

						if(chatInfo != NULL)
							SDL_FreeSurface(chatInfo);

						chatInfo = NULL;

						chatPlaceW = 0;
						chatPlace = 0;
					}
				}

				else if(chatWindow)
				{
					chatting = false;
					chatWindow = false;

					SDL_EnableKeyRepeat(0, SDL_DEFAULT_REPEAT_INTERVAL);

					for(unsigned int i = 0; i < win.size(); i++)
					{
						if(win.at(i)->TYPE == INVITEINPUTWINDOW || win.at(i)->TYPE == KICKINPUTWINDOW || win.at(i)->TYPE == FRIENDINPUTWINDOW)
						{
							if(partyw.size() > 0)
							{
								if(win.at(i)->TYPE == INVITEINPUTWINDOW)
								{
									if(chat != "")
									{
										PacketInviteToParty((char*)chat.c_str());

										chat = "";
									}
								}

								else if(win.at(i)->TYPE == KICKINPUTWINDOW)
								{
									if(chat != "")
									{
										PacketKickFromParty((char*)chat.c_str());

										chat = "";
									}
								}

								else if(win.at(i)->TYPE == FRIENDINPUTWINDOW)
								{
									if(chat != "")
									{
										PacketChangeFriend(1, chat.c_str());

										chat = "";
									}
								}

								else
									chat = "";

								if(chatInfo != NULL)
									SDL_FreeSurface(chatInfo);

								chatInfo = NULL;

								chatPlaceW = 0;
								chatPlace = 0;

								partyw.at(0)->enablePress = true;

								SDL_FreeSurface(win.at(i)->inviteButton);
								SDL_FreeSurface(win.at(i)->quitx);
								SDL_FreeSurface(win.at(i)->w);

								delete win.at(i);
								win.erase(win.begin() + i);

								break;
							}
						}
					}
				}
				
				nextEnter = (getTime(true) + 200);
			}
		}

		if(isEvent == 0)
			return false;

		if(event.key.keysym.sym <= 0 || event.key.keysym.sym >= 365)
			return false;

		if(keys[SDLK_BACKSPACE])
		{
			if(chatInputWindow && iw.size() > 0)
			{
				if(iw.back()->inputstr.length() > 0)
				{
					iw.back()->inputstr.erase(iw.back()->inputstr.length() - 1);

					if(iw.back()->input != NULL)
						SDL_FreeSurface(iw.back()->input);

					if(iw.back()->inputstr.length() == 0)
						iw.back()->inputstr = "";

					iw.back()->input = RenderText_Solid(cour, iw.back()->inputstr.c_str(), clrFgBlack);
				}
			}

			else
			{
				if((chatMessage && pms->pm.size() > 0))
				{
					PMessage *pm = pms->GetMarked();

					if(pm->text.length() > 0 && pm->chatPlace > 0)
					{
						string newString = "";

						for(unsigned int r = 0; r < pm->text.length(); r++)
						{
							if(r != pm->chatPlace - 1)
								newString += pm->text.c_str()[r];
						}

						pm->text = newString;
						pm->chatPlace--;

						if(pm->textp != NULL)
							SDL_FreeSurface(pm->textp);

						if(pm->text.length() == 0)
							pm->text = "";

						std::string tempString = "";

						for(int r = 0; r < pm->chatPlace; r++)
							tempString += pm->text.c_str()[r];

						if(tempString.length() > 0)
						{
							SDL_Surface *loltest = RenderText_Solid(cour, tempString.c_str(), clrFgBlack);
							pm->chatPlaceW = loltest->w;

							SDL_FreeSurface(loltest);
						}

						else
							pm->chatPlaceW = 0;

						setChatIconShow = true;

						pm->textp = RenderText_Solid(cour, pm->text.c_str(), clrFgBlack);
					}
				}

				else
				{
					if(chat.length() > 0 && chatPlace > 0)
					{
						string newString = "";

						for(unsigned int r = 0; r < chat.length(); r++)
						{
							if(r != chatPlace - 1)
								newString += chat.c_str()[r];
						}

						chat = newString;
						chatPlace--;

						if(chatInfo != NULL)
							SDL_FreeSurface(chatInfo);

						if(chat.length() == 0)
							chat = "";

						std::string tempString = "";

						for(int r = 0; r < chatPlace; r++)
							tempString += chat.c_str()[r];

						if(tempString.length() > 0)
						{
							SDL_Surface *loltest = RenderText_Solid(cour, tempString.c_str(), clrFgBlack);
							chatPlaceW = loltest->w;

							SDL_FreeSurface(loltest);
						}

						else
							chatPlaceW = 0;

						setChatIconShow = true;

						chatInfo = RenderText_Solid(cour, chat.c_str(), clrFgBlack);
					}
				}
			}
		}

		if(chatInputWindow)
		{
			InputWindow *iws = iw.back();

			if(iws->inputstr.length() < iws->arg2TEXT.length())
			{
				std::string thisSTR = "";
				nopls = false;

				AddChatText(event, thisSTR, 2);

				if(iws->input != NULL)
					SDL_FreeSurface(iws->input);

				iws->inputstr += thisSTR;

				iws->input = RenderText_Solid(cour, iws->inputstr.c_str(), clrFgBlack);
			}
		}

		if((chatMessage && pms->pm.size() > 0))
		{
			PMessage *pm = pms->GetMarked();

			if(keys[SDLK_LEFT])
			{
				if(pm->chatPlace > 0)
				{
					pm->chatPlace--;

					std::string tempString = "";

					for(int r = 0; r < pm->chatPlace; r++)
						tempString += pm->text.c_str()[r];

					if(tempString.length() > 0)
					{
						SDL_Surface *loltest = RenderText_Solid(cour, tempString.c_str(), clrFgBlack);
						pm->chatPlaceW = loltest->w;

						SDL_FreeSurface(loltest);
					}

					else
						pm->chatPlaceW = 0;

					setChatIconShow = true;
				}
			}

			else if(keys[SDLK_RIGHT])
			{
				if((unsigned)pm->chatPlace < pm->text.length())
				{
					pm->chatPlace++;

					std::string tempString = "";

					for(int r = 0; r < pm->chatPlace; r++)
						tempString += pm->text.c_str()[r];

					if(tempString.length() > 0)
					{
						SDL_Surface *loltest = RenderText_Solid(cour, tempString.c_str(), clrFgBlack);
						chatPlaceW = loltest->w;

						SDL_FreeSurface(loltest);
					}

					else
						pm->chatPlaceW = 0;

					setChatIconShow = true;
				}
			}

			else
			{
				int nameLen = strlen(me->name);
				int cLen = pm->text.length();
				int lenGo = 0;

				if(pm->textp == NULL)
					lenGo = 0;
				else
					lenGo = pm->textp->w;

				if(((chatMessage && pms->pm.size() > 0) && (lenGo + me->nameSur.first()->w) < 213))
				{
					std::string tempSTR = "";

					AddChatText(event, tempSTR, 1);

					if(pm->textp != NULL)
						SDL_FreeSurface(pm->textp);

					if(tempSTR.length() > 0)
					{
						if(pm->chatPlace == 0)
							pm->text.insert(0, tempSTR);

						else if(pm->chatPlace != pm->text.length())
							pm->text.insert(pm->chatPlace, tempSTR);

						else
							pm->text += tempSTR;

						pm->chatPlace++;

						std::string tempString = "";

						for(int r = 0; r < pm->chatPlace; r++)
							tempString += pm->text.c_str()[r];

						if(tempString.length() > 0)
						{
							SDL_Surface *loltest = RenderText_Solid(cour, tempString.c_str(), clrFgBlack);
							pm->chatPlaceW = loltest->w;

							SDL_FreeSurface(loltest);
						}

						else
							pm->chatPlaceW = 0;
					}

					pm->textp = RenderText_Solid(cour, pm->text.c_str(), clrFgBlack);

					setChatIconShow = true;
				}
			}
		}

		else if(nopls || (chatting || chatWindow))
		{
			if(keys[SDLK_LEFT])
			{
				if(chatPlace > 0)
				{
					chatPlace--;

					std::string tempString = "";

					for(int r = 0; r < chatPlace; r++)
						tempString += chat.c_str()[r];

					if(tempString.length() > 0)
					{
						SDL_Surface *loltest = RenderText_Solid(cour, tempString.c_str(), clrFgBlack);
						chatPlaceW = loltest->w;

						SDL_FreeSurface(loltest);
					}

					else
						chatPlaceW = 0;

					setChatIconShow = true;
				}
			}

			else if(keys[SDLK_RIGHT])
			{
				if((unsigned)chatPlace < chat.length())
				{
					chatPlace++;

					std::string tempString = "";

					for(int r = 0; r < chatPlace; r++)
						tempString += chat.c_str()[r];

					if(tempString.length() > 0)
					{
						SDL_Surface *loltest = RenderText_Solid(cour, tempString.c_str(), clrFgBlack);
						chatPlaceW = loltest->w;

						SDL_FreeSurface(loltest);
					}

					else
						chatPlaceW = 0;

					setChatIconShow = true;
				}
			}

			else
			{
				int nameLen = strlen(me->name);
				int cLen = chat.length();
				int lenGo = 0;

				if(chatInfo == NULL)
					lenGo = 0;
				else
					lenGo = chatInfo->w;

				if((chatting && (lenGo + me->nameSur.first()->w) < 430) || (chatWindow && lenGo < 245))
				{
					std::string tempSTR = "";

					AddChatText(event, tempSTR, 1);

					if(chatInfo != NULL)
						SDL_FreeSurface(chatInfo);

					if(tempSTR.length() > 0)
					{
						if(chatPlace == 0)
							chat.insert(0, tempSTR);

						else if(chatPlace != chat.length())
							chat.insert(chatPlace, tempSTR);

						else
							chat += tempSTR;

						chatPlace++;

						std::string tempString = "";

						for(int r = 0; r < chatPlace; r++)
							tempString += chat.c_str()[r];

						if(tempString.length() > 0)
						{
							SDL_Surface *loltest = RenderText_Solid(cour, tempString.c_str(), clrFgBlack);
							chatPlaceW = loltest->w;

							SDL_FreeSurface(loltest);
						}

						else
							chatPlaceW = 0;
					}

					chatInfo = RenderText_Solid(cour, chat.c_str(), clrFgBlack);

					setChatIconShow = true;
				}
			}
		}
	}

	else
	{
		if(!me->dead)
		{
			bool any = false;

			if(keys[SDLK_UP])
			{
				if(me->follow)
				{
					me->follow = false;
					me->followId = 0;
				}

				if((currMoveWay == UP || currMoveWay == 0))
				{
					any = true;

					if(!move)
					{
						me->move.start(false);
					}

					if(currMoveWay != UP)
						currMoveWay = UP;

					float Vel = ((float)me->pixSec * ((float)me->move.get_ticks(false) / 1000.0f));

					if((cameraY - Vel) < 26 && (int)cameraY != 26 && (int)cameraY > 26)
						Vel = (cameraY - 26.0f);

					if((cameraY - Vel) >= 26)
					{
						me->y -= Vel;

						if(!move)
						{
							move = true;

							PacketGameMove(UP, (int)me->x, (int)me->y);
						}

						if(cameraY <= ((screen->h / 3) - me->my->h) && currBackY <= (0 - Vel))
						{
							currBackY += Vel;

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
							cameraY -= Vel;
					}

					me->move.start(false);
				}
			}

			if(keys[SDLK_DOWN])
			{
				if(me->follow)
				{
					me->follow = false;
					me->followId = 0;
				}

				if((currMoveWay == DOWN || currMoveWay == 0))
				{
					any = true;

					if(!move)
					{
						me->move.start(false);
					}

					if(currMoveWay != DOWN)
						currMoveWay = DOWN;

					float xTemp = ((float)me->pixSec * ((float)me->move.get_ticks(false) / 1000.0f));

					if((me->y + xTemp) > (msize(MAPY) - me->my->h) && (int)me->y != (msize(MAPY) - me->my->h) && (int)me->y < (msize(MAPY) - me->my->h))
						xTemp = ((msize(MAPY) - me->my->h) - me->y);

					if((me->y + xTemp) <= (msize(MAPY) - me->my->h))
					{
						me->y += xTemp;

						if(!move)
						{
							move = true;

							PacketGameMove(DOWN, (int)me->x, (int)me->y);
						}

						if((cameraY >= (screen->h - (screen->h / 3))) && (msize(MAPY) + currBackY) > (screen->h + xTemp))
						{
							currBackY -= xTemp;

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
							cameraY += xTemp;
					}

					me->move.start(false);
				}
			} 

			if(keys[SDLK_LEFT])
			{
				if(me->follow)
				{
					me->follow = false;
					me->followId = 0;
				}

				if((currMoveWay == LEFT || currMoveWay == 0))
				{
					any = true;

					if(!move)
					{
						me->move.start(false);
					}

					if(currMoveWay != LEFT)
						currMoveWay = LEFT;

					float Vel = ((float)me->pixSec * ((float)me->move.get_ticks(false) / 1000.0f));

					if((cameraX - Vel) < 0 && (int)cameraX != 0 && (int)cameraX > 0)
						Vel = cameraX;

					if((cameraX - Vel) >= 0)
					{
						me->x -= Vel;

						if(!move)
						{
							move = true;

							me->cMove = LEFT;
							me->wMove = LEFT;

							PacketGameMove(LEFT, (int)me->x, (int)me->y);
						}

						if(cameraX <= ((screen->w / 3) - me->my->w) && currBackX <= (0 - Vel))
						{
							currBackX += Vel;

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
							cameraX -= Vel;
					}

					me->move.start(false);
				}
			}

			if(keys[SDLK_RIGHT])
			{
				if(me->follow)
				{
					me->follow = false;
					me->followId = 0;
				}

				if((currMoveWay == RIGHT || currMoveWay == 0))
				{
					any = true;

					if(!move)
					{
						me->move.start(false);
					}

					if(currMoveWay != RIGHT)
						currMoveWay = RIGHT;

					float Vel = ((float)me->pixSec * ((float)me->move.get_ticks(false) / 1000.0f));

					int moveMax = screen->w;

					if(screen->w > msize(MAPX))
						moveMax = msize(MAPX);

					if((cameraX + Vel) > (moveMax - me->my->w) && (int)cameraX != (moveMax - me->my->w) && (int)cameraX < (moveMax - me->my->w))
						Vel = ((moveMax - me->my->w) - cameraX);

					if((cameraX + Vel) <= (moveMax - me->my->w))
					{
						me->x += Vel;
						if(!move)
						{
							move = true;

							me->cMove = RIGHT;
							me->wMove = RIGHT;

							PacketGameMove(RIGHT, (int)me->x, (int)me->y);
						}

						if((cameraX >= (screen->w - (screen->w / 3))) && (msize(MAPX) + currBackX) > (screen->w + Vel))
						{
							currBackX -= Vel;

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
							cameraX += Vel;
					}

					me->move.start(false);
				}
			}

			if(!any)
			{
				if(move && !me->follow)
				{
					move = false;

					PacketGameStopMove((int)me->x, (int)me->y);

					currMoveWay = 0;
				}
			}

			if(keys[SDLK_SPACE])
			{
				if((unsigned)nextSpace <= getTime(true))
				{
					PacketGameUpdatePosition((int)me->x, (int)me->y);

					nextSpace = (getTime(true) + 1000);
				}
			}

			if(keys[SDLK_BACKSPACE])
			{
				if((unsigned)nextMapChange <= getTime(true))
				{
					PacketGameChangeMap((int)me->x, (int)me->y);

					nextMapChange = (getTime(true) + 500);
				}
			}

			if(keys[SDLK_s])
			{
				if((unsigned)nextShot <= getTime(false))
				{
					if(/*me->equipped && */(me->cMove == LEFT || me->cMove == RIGHT))
					{
						Bullet *bullet = new Bullet();

						bullet->cY = (me->y + (me->my->w / 2));
						bullet->cWay = me->cMove;

						bullet->cPic = getSurface(BULLET);

						int plusX = 0;

						if(me->weapon == NULL)
							plusX = 0;

						else
							plusX = me->weapon->w;

						switch(me->cMove)
						{
							case LEFT: bullet->cX = (me->x - ((plusX + 1) + bullet->cPic->w));
								break;

							case RIGHT: bullet->cX = (me->x + (me->my->w + plusX));
								break;
						}

						PacketGameNewBullet(bullet, 1);

						delete bullet;

						long delaySS = GetAttackDelay();
						nextShot = (getTime(false) + delaySS);
					}
				}
			}

			if(keys[SDLK_1])
				PressTaskbar(1);

			if(keys[SDLK_2])
				PressTaskbar(2);

			if(keys[SDLK_3])
				PressTaskbar(3);

			if(keys[SDLK_4])
				PressTaskbar(4);

			if(keys[SDLK_5])
				PressTaskbar(5);

			if(keys[SDLK_6])
				PressTaskbar(6);

			if(keys[SDLK_7])
				PressTaskbar(7);

			if(keys[SDLK_8])
				PressTaskbar(8);

			if(keys[SDLK_9])
				PressTaskbar(9);

			if(keys[SDLK_e])
			{
				if((unsigned)nextEquipWeapon <= getTime(true))
				{
					PacketGameEquipWeapon();

					nextEquipWeapon = (getTime(true) + 300);
				}
			}
		}

		if(keys[SDLK_RETURN])
		{
			if((unsigned)nextEnter <= getTime(true))
			{
				chatting = true;

				SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);

				nextEnter = (getTime(true) + 200);
			}
		}

		if(keys[SDLK_h])
		{
			if((unsigned)nextWindowChange <= getTime(true))
			{
				if(win.size() > 0)
				{
					for(unsigned int i = 0; i < win.size(); i++)
					{
						if(win.at(i)->TYPE == HPWINDOW)
						{
							if(win.at(i)->enable)
								win.at(i)->enable = false;
							else
								win.at(i)->enable = true;

							break;
						}
					}
				}

				nextWindowChange = (getTime(true) + 300);
			}
		}

		if(keys[SDLK_j])
		{
			if((unsigned)nextWindowChange <= getTime(true))
			{
				if(addstatwindow.size() > 0)
				{
					if(addstatwindow.at(0)->enable)
						addstatwindow.at(0)->enable = false;
					
					else
						addstatwindow.at(0)->enable = true;
				}

				nextWindowChange = (getTime(true) + 300);
			}
		}

		if(keys[SDLK_t])
		{
			if((unsigned)nextWindowChange <= getTime(true))
			{
				if(win.size() > 0)
				{
					for(unsigned int i = 0; i < win.size(); i++)
					{
						if(win.at(i)->TYPE == INFOWINDOW)
						{
							if(win.at(i)->enable)
								win.at(i)->enable = false;

							else
								win.at(i)->enable = true;

							break;
						}
					}
				}

				nextWindowChange = (getTime(true) + 300);
			}
		}

		if(keys[SDLK_p])
		{
			if((unsigned)nextWindowChange <= getTime(true))
			{
				if(partyw.size() > 0)
				{
					if(partyw.at(0)->enable)
						partyw.at(0)->enable = false;

					else
						partyw.at(0)->enable = true;
				}

				nextWindowChange = (getTime(true) + 300);
			}
		}

		if(keys[SDLK_f])
		{
			if((unsigned)nextWindowChange <= getTime(true))
			{
				if(fw->enable)
					fw->enable = false;

				else
					fw->enable = true;

				nextWindowChange = (getTime(true) + 300);
			}
		}

		if(keys[SDLK_i])
		{
			if((unsigned int)nextWindowChange <= getTime(true))
			{
				if(inv->enable)
					inv->enable = false;
				else
					inv->enable = true;

				nextWindowChange = (getTime(true) + 300);
			}
		}

		if(keys[SDLK_u])
		{
			if((unsigned int)nextWindowChange <= getTime(true))
			{
				if(transWindow)
				{
					transWindow = false;

					SetTrans(false);

					for(unsigned int i = 0; i < option->op.size(); i++)
					{
						if(option->op.at(i).effect == TRANS)
						{
							option->op.at(i).active = false;

							break;
						}
					}

					AddConfigOption(TRANS, NULL, 0, 0, ONEINT, "Transparency:");
				}

				else
				{
					transWindow = true;

					SetTrans(true);

					for(unsigned int i = 0; i < option->op.size(); i++)
					{
						if(option->op.at(i).effect == TRANS)
						{
							option->op.at(i).active = true;

							break;
						}
					}

					AddConfigOption(TRANS, NULL, 1, 0, ONEINT, "Transparency:");
				}

				nextWindowChange = (getTime(true) + 300);
			}
		}

		if(keys[SDLK_o])
		{
			if((unsigned)nextWindowChange <= getTime(true))
			{
				if(option->enable)
					option->enable = false;
				else
					option->enable = true;

				nextWindowChange = (getTime(true) + 300);
			}
		}

		if(keys[SDLK_k])
		{
			if((unsigned)nextWindowChange <= getTime(true))
			{
				if(sk->enable)
					sk->enable = false;
				else
					sk->enable = true;

				nextWindowChange = (getTime(true) + 300);
			}
		}

		if(keys[SDLK_r])
		{
			if((unsigned)nextWindowChange <= getTime(true))
			{
				win.at(0)->x = 0;
				win.at(0)->y = 0;
				win.at(1)->x = 0;
				win.at(1)->y = (screen->h - win.at(1)->w->h - task->bar->h + 1);
				win.at(2)->x = (screen->w - win.at(2)->w->w);
				win.at(2)->y = 0;

				chatFunc->SetLast();

				addstatwindow.at(0)->x = 0;
				addstatwindow.at(0)->y = (win.at(1)->y - addstatwindow.at(0)->window->h);

				partyw.at(0)->y = (screen->h / 2);
				partyw.at(0)->x = (screen->w - partyw.at(0)->window->w);

				inv->x = (screen->w - inv->inv->w);
				inv->y = (screen->h - inv->inv->h - task->bar->h);

				SetupInventory();

				sk->x = (screen->w / 2);
				sk->y = (screen->h / 2);

				sw->x = (inv->x - sw->win->w - 1);
				sw->y = (inv->y + 23);

				sw->SetupPos();

				ptrade->x = (inv->x - ptrade->win->w - 1);
				ptrade->y = inv->y;

				ptrade->SetupPos();

				option->x = (addstatwindow.at(0)->x + addstatwindow.at(0)->window->w);
				option->y = addstatwindow.at(0)->y;

				tp->x = (screen->w / 2);
				tp->y = (screen->h / 2);

				fw->x = (screen->w - fw->win->w);
				fw->y = (screen->h - fw->win->h - task->bar->h + 1);
				fw->startY = fw->y + 65;

				for(unsigned int i = 0; i < icons.size(); i++)
				{
					Icon *ic = icons.at(i);

					ic->x = (166 + ((i * 42) + 1));
					ic->y = 0;

					ic->timeLeftX = (ic->x + 5);
					ic->timeLeftY = (ic->y + 28);

					ic->name1X = (ic->x + 5);
					ic->name1Y = (ic->y + 3);
					
					ic->name2X = (ic->x + 5);
					ic->name2Y = (ic->name1Y + 10);
				}

				if(dragged.type != INVENTORYITEM && dragged.type != SKILL)
					drag = false;

				nextWindowChange = (getTime(true) + 300);
			}
		}

		if(keys[SDLK_c])
		{
			if((unsigned)nextWindowChange <= getTime(true))
			{
				win.at(1)->enable = !win.at(1)->enable;

				nextWindowChange = (getTime(true) + 300);
			}
		}
	}

	return false;
}

void HandleMouseMotion(SDL_Event event)
{
	if(lastMouseX != event.motion.x || lastMouseY != event.motion.y)
	{
		if(drag)
		{
			if(dragged.type == INVENTORYITEM || dragged.type == SKILL)
			{
				dragged.x = (event.motion.x - (dragged.pic->w / 2));
				dragged.y = (event.motion.y - (dragged.pic->h / 2));
			}

			else if(dragged.type == DRAGFRIENDLIST)
				fw->Drag(event.motion.x, event.motion.y);

			else if(dragged.type == DRAGCHATLIST)
				chatFunc->Drag(event.motion.x, event.motion.y);

			else if(dragged.type == DRAGPMSLIST)
			{
				for(unsigned int i = 0; i < pms->pm.size(); i++)
				{
					if(pms->pm.at(i).toname == dragged.name)
					{
						pms->pm.at(i).Drag(event.motion.x, event.motion.y);
						break;
					}
				}
			}

			else
				MoveKoblaWindow(event.motion.x, event.motion.y);
		}

		bool test = ObjectInfoWindowFunction(event.motion.x, event.motion.y, lastMouseX, lastMouseY);

		if(test)
			ow->enable = true;

		else
			ow->enable = false;

		lastMouseX = event.motion.x;
		lastMouseY = event.motion.y;
	}
}
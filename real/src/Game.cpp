#include <SDL.h>
#include <SDL_TTF.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <vector>
#include <string> 

#include "Friend.h"
#include "Mouse.h"
#include "Packet.h"
#include "Packets.h"
#include "Login.h"
#include "Misc.h"
#include "Animation.h"
#include "Process.h"
#include "Main.h"
#include "Music.h"

using namespace std;

int FPS = 60;

extern SOCKET sock;
fd_set f_readset, f_writeset, f_exset;

extern unsigned char *pac;
extern bool done;
extern bool colorkey;

SDL_Surface *screen;

extern SDL_Surface *fpsSurface;
extern SDL_Surface *pingSurface;
extern SDL_Surface *loadingSurface;

Character *me = new Character();

vector<Character*> cv;
vector<Monster*> cm;
vector<Window*> win;
vector<NPC*> npc;
vector<Notifer*> nv;
vector<Bullet*> bullets;
vector<AddStatWindow*> addstatwindow;
vector<PartyWindow*> partyw;
vector<PartyInvite*> pi;
vector<Icon*> icons;
vector<Drop*> drops;
vector<Damage> dmgs;
vector<Message*> msgs;
vector<InputWindow*> iw;
vector<LoginMessage*> lmsgs;
vector<CharacterSelect> charsel;
vector<LoginInput*> linputs;

extern vector<Animation*> anis;

Mark *mark = new Mark();
Inventory *inv = new Inventory();
Taskbar *task = new Taskbar();
SkillWindow *sk = new SkillWindow();
ObjectInfoWindow *ow = new ObjectInfoWindow();
OptionWindow *option = new OptionWindow();
ShopWindow *sw = new ShopWindow();
PlayerTrade *ptrade = new PlayerTrade();
TeleportWindow *tp = new TeleportWindow();
FriendWindow *fw = new FriendWindow();
PrivateMessage *pms = new PrivateMessage();
ChatManager *chatFunc = new ChatManager();

int eAtk = 0;

string mLoad = "Data/World/Map-";

TTF_Font *cour;
TTF_Font *courMedium;
TTF_Font *courSmall;
TTF_Font *courBold;
TTF_Font *courNotice;
TTF_Font *courOutline;
SDL_Color clrFg = {255, 255, 255, 0};
SDL_Color clrFgBlack = {0, 0, 0, 0};
SDL_Color clrFgRed = {255, 0, 0, 0};
SDL_Color clrFgBlue = {0, 0, 255, 0};
SDL_Color clrFgGreen = {0, 255, 0, 0};
SDL_Color clrFgCyan = {255, 255, 0, 0};
SDL_Color clrFgBajs = {255, 128, 64, 0};

bool spawned = false;
bool chatting = false;
bool chatWindow = false;
bool kördö = false;

string chat = "";

SDL_Surface *chatNot;
SDL_Surface *chatInfo = NULL;

Animation chatIcon = Animation();

long nextEnter = 0;
long nextChatEnter = 0;
long nextChatDelete = 0;
long nextShot = 0;
long nextMove = 0;
long nextEquipWeapon = 0;
long nextWindowChange = 0;
long nextSpace = 0;
long nextMapChange = 0;
long nextSlowSpeedShot = 0;
long nextPoisonShot = 0;
long nextNotiferP = 0;
long nextNotiferS = 0;
long nextSkill = 0;
long nextInput = 0;

int mCharSpawnId = 0;

bool PKENABLE = false;

bool showFps = true;

float currBackX = 0;
float currBackY = 0;

float cameraX = 0;
float cameraY = 0;

int camShowX = 0;
int camShowToX = 0;
int camShowY = 0;
int camShowToY = 0;

bool drag = false;
long unDragTime = 0;

Drag dragged = {0, 0, 0, 0, 0, NULL, 0};

extern bool fullscreen;

extern int resX;
extern int resY;

bool transWindow = false;

//Skill *skills;
//int skillLoaded = 0;

vector<Skill*> skill;

extern Timer fpscheck;

int chatTries = 0;

bool keys[365] = {false};
int keyPressed = 0;
bool botActive = false;

extern int currMoveWay;

extern int lastMouseX;
extern int lastMouseY;

extern long noRemoveTimer;

SDL_Surface *hpBar;
SDL_Surface *manaBar;
SDL_Surface *eBar;

SDL_Surface *markItemShop;

bool cleaned = false;
extern bool chatInputWindow;

extern Map *map;

MusicManager *music = NULL;
SFXManager *sound = NULL;

extern bool noSound;

SDL_Surface *loginScreen;
SDL_Surface *loginFirst;
SDL_Surface *loginChar;
SDL_Surface *loginRem;
bool login = true;

extern int version;

int logplace = LOGINSCREEN;
extern LoginWriter write;

bool wantPacket = false;
int lastSent = 0x1;

int isEvent = 0;

bool remLogin = false;
char loginName[40] = "";

vector<SendPacket> toserv;
CurrentPacket cp = CurrentPacket();
bool donecsp = true;
SendPacket csp = SendPacket();

extern vector<int> programex;
extern bool dc;

SDL_Surface *boxFull = NULL;
SDL_Surface *boxEmpty = NULL;

SDL_Surface *skillShow = NULL;

void StartUp()
{
	if(SDL_Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO) < 0)
		KMessageBox("Error", MB_ICONERROR | MB_OK, "SDL failed to initialize!");

	int audioRate = 48000;
	int audioChannels = 2;
	int audioBuffers = 512;
	Uint16 audioFormat = AUDIO_S16;

	Mix_OpenAudio(audioRate, audioFormat, audioChannels, audioBuffers);
	Mix_QuerySpec(&audioRate, &audioFormat, &audioChannels);

	SDL_WM_SetCaption("Kobla", NULL);

	if(fullscreen)
		screen = SDL_SetVideoMode(resX, resY, 32, SDL_FULLSCREEN | SDL_HWSURFACE | SDL_DOUBLEBUF);

	else
		screen = SDL_SetVideoMode(resX, resY, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);

	if(screen == NULL)
		Log("Unable to set video mode: %s.", SDL_GetError());

	SDL_Flip(screen);

	cameraX = (float)(screen->w / 2);
	cameraY = (float)(screen->h / 2);

	lastMouseX = 0;
	lastMouseY = 0;

	cleaned = false;

	delete map;
	map = new Map();

	delete tp;
	tp = new TeleportWindow();

	TTF_Init();

	cour = TTF_OpenFont("Data/Text/cour.ttf", 16);
	courMedium = TTF_OpenFont("Data/Text/cour.ttf", 14);
	courSmall = TTF_OpenFont("Data/Text/cour.ttf", 10);
	courBold = TTF_OpenFont("Data/Text/cour.ttf", 14);
	courNotice = TTF_OpenFont("Data/Text/cour.ttf", 36);
	courOutline = TTF_OpenFont("Data/Text/cour.ttf", 16);

	if(cour == NULL || courMedium == NULL || courSmall == NULL || courBold == NULL || courNotice == NULL || courOutline == NULL)
		Log("There was an error loading the fonts.");

	else
		TTF_SetFontStyle(courBold, TTF_STYLE_BOLD);

	loadingSurface = RenderText_Solid(cour, "Loading, please wait..", clrFg);

	string opSec2 = "Data/Interface/Login-Screen-";
	char tempss2[20];

	_itoa_s(resX, tempss2, 20, 10);
	opSec2 += tempss2;
	opSec2 += ".png";

	loginScreen = loadimage(opSec2.c_str(), false, 0, 0, 0);
	loginFirst = loadimage("Data/Interface/Login-Window.png", false, 0, 0, 0);
	loginChar = loadimage("Data/Interface/Login-Character.png", false, 0, 0, 0);

	SDL_SetAlpha(loginFirst, SDL_SRCALPHA | SDL_RLEACCEL, 200);
	SDL_SetAlpha(loginChar, SDL_SRCALPHA | SDL_RLEACCEL, 200);

	if(!remLogin)
		loginRem = loadimage("Data/Interface/Option-Empty.png", false, 0, 0, 0);

	else
		loginRem = loadimage("Data/Interface/Option-Full.png", false, 0, 0, 0);
	UpdateLogin();

	chatInputWindow = false;

	skillShow = loadimage("Data/Interface/Skill-Show.png", false, 0, 0, 0);

	char tempss[20];
	string opSec = "Data/Interface/Taskbar-";

	_itoa_s(resX, tempss, 20, 10);

	opSec += tempss;
	opSec += ".png";

	task->bar = loadimage(opSec.c_str(), false, 0, 0, 0);

	task->menu = loadimage("Data/Interface/Taskbar-Menu.png", true, 255, 255, 255);

	task->x = 0;
	task->y = (screen->h - task->bar->h);

	task->enableTask = true;
	task->enableMenu = false;
	task->enablePress = true;

	Window *wi = new Window();

	wi->w = loadimage("Data/Interface/HP-Window.png", false, 0, 0, 0);
	wi->x = 0;
	wi->y = 0;
	wi->enable = true;
	wi->TYPE = HPWINDOW;

	wi->hpStartX = 88;
	wi->hpY = 25;

	//hpBarStart = loadimage("Data/Interface/HP-Bar-Start.png", true, 128, 128, 128);
	hpBar = loadimage("Data/Interface/HP-Bar.png", false, 0, 0, 0);
	manaBar = loadimage("Data/Interface/Mana-Bar.png", false, 0, 0, 0);
	eBar = loadimage("Data/Interface/Energy-Bar.png", false, 0, 0, 0);
	//hpBarStop = loadimage("Data/Interface/HP-Bar-Stop.png", true, 128, 128, 128);

	wi->enableX = true;
	wi->quitx = loadimage("Data/Interface/Quit-Button.png", true, 255, 255, 255);

	win.push_back(wi);

	Window *ch = new Window();

	ch->w = loadimage("Data/Interface/Chat-Window.png", false, 0, 0, 0);
	ch->x = 0;
	ch->y = (screen->h - ch->w->h - task->bar->h + 1);
	ch->enable = true;
	ch->TYPE = CHATWINDOW;

	ch->enableX = false;

	chatIcon.cX = (ch->x + 10);
	chatIcon.cY = (ch->y + 96);
	chatIcon.fps = 2;

	SDL_Surface *loadme = loadimage("Data/Icon/Chat-Icon.png", false, 0, 0, 0);
	SDL_Surface *loadme2 = NULL;

	chatIcon.ani.push_back(loadme);
	chatIcon.ani.push_back(loadme2);

	win.push_back(ch);

	boxEmpty = loadimage("Data/Interface/Option-Empty.png", false, 0, 0, 0);
	boxFull = loadimage("Data/Interface/Option-Full.png", false, 0, 0, 0);

	Window *iw = new Window();

	iw->w = loadimage("Data/Interface/Info-Window.png", false, 0, 0, 0);
	iw->x = (screen->w - iw->w->w);
	iw->y = 0;
	iw->enable = true;
	iw->TYPE = INFOWINDOW;

	iw->hpStartX = (iw->x + 88);
	iw->hpY = 6;

	iw->enableX = true;
	iw->quitx = loadimage("Data/Interface/Quit-Button.png", true, 255, 255, 255);

	iw->levelSurface = RenderText_Solid(cour, "1", clrFgBlack);
	iw->expSurface = RenderText_Solid(cour, "0%/100%", clrFgBlack);
	iw->hpSurface = RenderText_Solid(cour, "1/3", clrFgBlack);

	win.push_back(iw);

	AddStatWindow *asw = new AddStatWindow();

	asw->x = 0;
	
	asw->enable = false;

	asw->power = NULL;
	asw->hp = NULL;
	asw->pointsav = NULL;
	asw->agility = NULL;

	asw->window = loadimage("Data/Interface/AddStat-Window.png", false, 0, 0, 0);
	asw->add = loadimage("Data/Interface/Add-Button.png", true, 255, 255, 255);
	asw->quitx = loadimage("Data/Interface/Quit-Button.png", true, 255, 255, 255);

	asw->y = (ch->y - asw->window->h);

	addstatwindow.push_back(asw);

	PartyWindow *pw = new PartyWindow();

	pw->window = loadimage("Data/Interface/Party-Window.png", false, 0, 0, 0);
	pw->quitx = loadimage("Data/Interface/Quit-Button.png", true, 255, 255, 255);

	pw->invite = loadimage("Data/Interface/InviteParty-Button.png", true, 255, 255, 255);
	pw->kick = loadimage("Data/Interface/KickParty-Button.png", true, 255, 255, 255);
	pw->leave = loadimage("Data/Interface/LeaveParty-Button.png", true, 255, 255, 255);

	pw->levelSur = RenderText_Solid(cour, "1", clrFgBlack);

	pw->x = (screen->w - pw->window->w);
	pw->y = (screen->h / 2);

	partyw.push_back(pw);

	inv->enable = false;
	inv->inv = loadimage("Data/Interface/Inventory-Window.png", false, 0, 0, 0);
	inv->x = (screen->w - inv->inv->w);
	inv->y = (screen->h - inv->inv->h - task->bar->h);
	inv->golds = RenderText_Solid(courMedium, "0", clrFgBlack);
	inv->quitx = loadimage("Data/Interface/Quit-Button.png", true, 255, 255, 255);

	chatNot = RenderText_Solid(cour, "Chatting..", clrFg);

	for(int i = 0; i < 9; i++)
	{
		task->pic[i] = new Picture();

		task->pic[i]->x = ((i * 40) + ((task->bar->w / 2) - 179));
		task->pic[i]->y = (task->y + 4);
	}

	sk->win = loadimage("Data/Interface/Skill-Window.png", false, 0, 0, 0);
	sk->quitx = loadimage("Data/Interface/Quit-Button.png", true, 255, 255, 255);

	markItemShop = loadimage("Data/Interface/NPC-MarkShop.png", true, 255, 255, 255);

	sk->x = (screen->w / 2);
	sk->y = (screen->h / 2);

	sk->enable = false;

	sw->Clean();

	sw->win = loadimage("Data/Interface/NPC-Shop.png", false, 0, 0, 0);
	sw->quitx = loadimage("Data/Interface/Quit-Button.png", true, 255, 255, 255);

	sw->y = (inv->y + 23);
	sw->x = (inv->x - sw->win->w - 1);

	ptrade->enable = false;
	
	ptrade->Clean();

	ptrade->win = loadimage("Data/Interface/Trade-Window.png", false, 0, 0, 0);
	ptrade->quitx = loadimage("Data/Interface/Quit-Button.png", true, 255, 255, 255);
	ptrade->ok = loadimage("Data/Interface/OK-Button.png", true, 255, 255, 255);
	ptrade->cancel = loadimage("Data/Interface/Cancel-Button.png", true, 255, 255, 255);

	ptrade->y = inv->y;
	ptrade->x = (inv->x - ptrade->win->w - 1);

	ptrade->SetupPos();

	option->enable = false;
	option->x = (addstatwindow.at(0)->x + addstatwindow.at(0)->window->w);
	option->y = addstatwindow.at(0)->y;
	option->win = loadimage("Data/Interface/Option-Window.png", false, 0, 0, 0);
	option->quitx = loadimage("Data/Interface/Quit-Button.png", true, 255, 255, 255);
	option->buttonEmpty = loadimage("Data/Interface/Option-Empty.png", false, 0, 0, 0);
	option->buttonFull = loadimage("Data/Interface/Option-Full.png", false, 0, 0, 0);

	Option o = Option();

	o.effect = TRANS;
	o.name = RenderText_Solid(cour, "Transparency:", clrFgBlack);
	o.active = transWindow;

	Option o2 = Option();
	
	o2.effect = SOUND;
	o2.name = RenderText_Solid(cour, "Sound:", clrFgBlack);
	o2.active = !noSound;

	Option o3 = Option();

	o3.effect = MUSIC;
	o3.name = RenderText_Solid(cour, "Music:", clrFgBlack);
	o3.active = !noMusic;

	option->op.push_back(o);
	option->op.push_back(o2);
	option->op.push_back(o3);

	ow->start = loadimage("Data/Interface/ObjectInfo-Start.png", true, 255, 255, 255);
	ow->bar = loadimage("Data/Interface/ObjectInfo-Bar.png", true, 255, 255, 255);
	ow->stop = loadimage("Data/Interface/ObjectInfo-Stop.png", true, 255, 255, 255);

	SDL_SetAlpha(ow->start, SDL_SRCALPHA | SDL_RLEACCEL, 210);
	SDL_SetAlpha(ow->bar, SDL_SRCALPHA | SDL_RLEACCEL, 210);
	SDL_SetAlpha(ow->stop, SDL_SRCALPHA | SDL_RLEACCEL, 210);
	
	ow->enable = false;

	tp->win = loadimage("Data/Interface/Teleport-Window.png", false, 0, 0, 0);
	tp->quitx = loadimage("Data/Interface/Quit-Button.png", true, 255, 255, 255);
	tp->x = (screen->w / 2);
	tp->y = (screen->h / 2);

	tp->enable = false;

	fw->win = loadimage("Data/Interface/Friend-Window.png", false, 0, 0, 0);
	fw->quitx = loadimage("Data/Interface/Quit-Button.png", true, 255, 255, 255);
	fw->noteup = loadimage("Data/Interface/Friend-NoteUp.png", true, 255, 255, 255);
	fw->notedown = loadimage("Data/Interface/Friend-NoteDown.png", true, 255, 255, 255);
	fw->x = (screen->w - fw->win->w);
	fw->y = (screen->h - fw->win->h - task->bar->h + 1);
	fw->startY = fw->y + 65;
	fw->enable = false;

	chatFunc->startY = ch->y + 5;

	pms->win = loadimage("Data/Interface/PrivateMessage-Window.png", false, 0, 0, 0);
	pms->quitx = loadimage("Data/Interface/Quit-Button.png", true, 255, 255, 255);

	PreLoad();
	LoadAnimations();

	me->Load(1);
	me->nameSur.clean();
	CreateNameSurface(&me->nameSur, cour, clrFg, "(None)", 1);

	mark->Load();

	fpsSurface = RenderText_Solid(cour, "FPS: 0", clrFgBlack);
	pingSurface = RenderText_Solid(cour, "Latency: 0 ms", clrFgBlack);

	if(transWindow)
	{
		SetTrans(true);

		for(unsigned int i = 0; i < option->op.size(); i++)
		{
			if(option->op.at(i).effect == TRANS)
			{
				option->op.at(i).active = true;

				break;
			}
		}
	}

	long times = getTime(true);

	nextSpace = (times + 100);
	nextEnter = (times + 100);
	nextChatEnter = (times + 100);
	nextChatDelete = (times + 200);
	nextWindowChange = (times + 300);
	nextShot = (times + 300);
	nextMove = (times + 15);
	nextEquipWeapon = (times + 300);
	nextMapChange = (times + 100);
	nextSlowSpeedShot = (times + 80000);
	nextPoisonShot = (times + 60000);
	nextNotiferP = (times + 500);
	nextNotiferS = (times + 500);
	nextSkill = (times + 500);
	nextInput = (times + 120);

	unDragTime = (times + 300);

	botActive = false;

	SDL_EnableUNICODE(SDL_ENABLE);

	sound = new SFXManager();
	music = new MusicManager();

	//sound->createSound("Skjuta.wav", 1, 1);
	//music->playMusic("TestSong.ogg", 1, -1);
}

bool GameLoop()
{
	bool move = false;
	bool altf = false;

	bool mouseLeftClicked = false;
	bool mouseRightClicked = false;

	for(int i = 0; i < 365; i++)
	{
		keys[i] = false;
	}

	SDL_Event event;

	float fpsWaitTime = (1000.0f / (float)FPS);
	unsigned int checkTime = 0;

	bool runloop = true;

	fpscheck.start(true);

	kördö = true;

	unsigned int nextTick = 0;
	int interval = 1 * 1000/FPS;

	long nextPacketDelay = (getTime(true) + 2000);
	int tries = 0;

	CheckUpdate();
	bool wantUpdate = true;

	unsigned char *buf = new unsigned char[1000000];

	while(runloop)
	{
		isEvent = SDL_PollEvent(&event);

		if(event.type == SDL_QUIT)
		{
			runloop = false;

			break;
		}

		if(event.type == SDL_KEYDOWN)
		{
			if(!keys[event.key.keysym.sym])
			{
				keys[event.key.keysym.sym] = true;

				keyPressed += 1;
			}
		}

		if(event.type == SDL_KEYUP)
		{
			if(keys[event.key.keysym.sym])
			{
				keys[event.key.keysym.sym] = false;

				keyPressed -= 1;
			}
		}

		if(login)
		{
			if(wantUpdate)
			{
				if(done)
				{
					int ve = pac[3];

					done = false;
					wantUpdate = false;

					delete[] pac;

					if(version == ve)
					{
						write.StartWrite(true, true, (((screen->w / 2) - (loginFirst->w / 2)) + 44), (((screen->h / 2) - (loginFirst->h / 2)) + 52), (((screen->w / 2) - (loginFirst->w / 2)) + 44), (((screen->h / 2) - (loginFirst->h / 2)) + 102), 1);
						write.ar1s = loginName;
						write.NewSurface(1);
					}

					else if(ve == 0)
					{
						KMessageBox("Message", MB_OK, "Server capacity full.");
						break;
					}

					else
					{
						KMessageBox("Message", MB_OK, "You are using the wrong version, please download the most current version at our website.");
						break;
					}
				}

				else
				{
					if((unsigned)nextPacketDelay <= getTime(true))
					{
						tries++;

						if(tries > 2)
						{
							KMessageBox("Message", MB_OK, "Server capacity full, please try again later.");
							break;
						}

						nextPacketDelay = (getTime(true) + 2000);

						CheckUpdate();
					}
				}
			}

			else
			{
				if(wantPacket)
				{
					if(done)
					{
						done = false;
						wantPacket = false;

						switch(pac[2])
						{
							case 0x2: Login();
								break;

							case 0x3: Register();
								break;

							case 0x6: CharacterList();
								break;

							case 0x7: EditCharacter();
								break;
						}

						delete[] pac;
					}

					if(keyPressed > 0)
					{
						if(HandleKeyLogin(event))
							break;
					}
				}

				else
				{
					if(event.type == SDL_MOUSEMOTION)
					{
					}

					if(event.type == SDL_MOUSEBUTTONDOWN)
					{
						if(event.button.button == SDL_BUTTON_LEFT)
						{
							if(!mouseLeftClicked)
							{
								if(HandleMouseLogin(event))
									break;

								mouseLeftClicked = true;
							}
						}

						else if(event.button.button = SDL_BUTTON_RIGHT)
						{
							if(!mouseRightClicked)
							{
								if(HandleMouseLogin(event))
									break;

								mouseRightClicked = true;
							}
						}
					}

					if(event.type == SDL_MOUSEBUTTONUP)
					{
						if(event.button.button == SDL_BUTTON_LEFT)
							mouseLeftClicked = false;

						else if(event.button.button == SDL_BUTTON_RIGHT)
							mouseRightClicked = false;
					}

					if(keyPressed > 0)
					{
						if(HandleKeyLogin(event))
							break;
					}
				}
			}
		}

		else
		{
			if(event.type == SDL_MOUSEMOTION)
				HandleMouseMotion(event);

			if(event.type == SDL_MOUSEBUTTONDOWN)
			{
				if(event.button.button == SDL_BUTTON_LEFT)
				{
					if(!mouseLeftClicked)
					{
						if(HandleMouse(event, altf, move))
							break;

						mouseLeftClicked = true;
					}
				}

				else if(event.button.button = SDL_BUTTON_RIGHT)
				{
					if(!mouseRightClicked)
					{
						if(HandleMouse(event, altf, move))
							break;

						mouseRightClicked = true;
					}
				}
			}

			if(event.type == SDL_MOUSEBUTTONUP)
			{
				if(event.button.button == SDL_BUTTON_LEFT)
				{
					HandleMouseUp(event);

					mouseLeftClicked = false;
				}

				else if(event.button.button == SDL_BUTTON_RIGHT)
				{
					HandleMouseUp(event);

					mouseRightClicked = false;
				}
			}

			if(botActive)
				Bot();

			if(keyPressed > 0)
			{
				if(HandleKey(event, move))
					return true;
			}

			else
			{
				if(move)
				{
					move = false;

					PacketGameStopMove((int)me->x, (int)me->y);

					currMoveWay = 0;
				}
			}
		}

		RenderThread(move);

		bool runOnce = true;

		while(nextTick > getTime(true) || runOnce)
		{
			runOnce = false;
			unsigned int setdel = 1;

			if(nextTick > getTime(true))
				setdel = ((nextTick - getTime(true)) * 1000);

			TIMEVAL timeout;
			timeout.tv_sec = 0;
			timeout.tv_usec = setdel;

			InitFD();

			if(select(0, &f_readset, &f_writeset, &f_exset, &timeout) > 0)
			{
				if(FD_ISSET(sock, &f_readset))
				{
					int bufsize = 1000000;

					if(cp.waiting)
						bufsize = cp.iwant;

					int by = recv(sock, (char*)buf, bufsize, 0);

					if(by == 0 || by == SOCKET_ERROR)
					{
						programex.push_back(DISCONNECTED);
						closesocket(sock);
						dc = true;

						continue;
					}

					int ret = HandlePackets(buf, by, move);

					if(ret == 0)
						cp.eraseme();
				}

				if(FD_ISSET(sock, &f_writeset))
				{
					if(donecsp)
					{
						if(toserv.size() > 0)
						{
							csp.bu = toserv.at(0).bu;
							csp.cp = toserv.at(0).cp;
							csp.len = toserv.at(0).len;

							donecsp = false;
							toserv.erase(toserv.begin());
						}
					}

					if(!donecsp)
					{
						SendPacket *sp = &csp;

						if(sp->len == 0)
						{
							delete[] sp->bu;
							donecsp = true;
						}

						else if(sp->cp == sp->len)
						{
							delete[] sp->bu;
							donecsp = true;
						}

						else
						{
							int s = send(sock, (const char*)sp->bu + sp->cp, sp->len - sp->cp, 0);

							if(s == 0)
							{
							}

							else if(s < 0)
							{
							}

							else if(s > 0)
							{
								sp->cp += s;

								if(sp->cp >= sp->len)
								{
									delete[] sp->bu;
									donecsp = true;
								}
							}
						}
					}
				}

				if(FD_ISSET(sock, &f_exset))
				{
					programex.push_back(DISCONNECTED2);
					closesocket(sock);
					dc = true;

					continue;
				}
			}
		}

		nextTick = (getTime(true) + interval);
	}

	return altf;
}

void CleanUp()
{
	PacketQuit();

	for(unsigned int i = 0; i < cm.size(); i++)
	{
		SDL_FreeSurface(cm.at(i)->nameSurface);
		SDL_FreeSurface(cm.at(i)->whenMarkName);

		delete cm.at(i);
	}

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
		}

		for(unsigned int y = 0; y < cv.at(i)->menu.size(); y++)
		{
			SDL_FreeSurface(cv.at(i)->menu.at(y).btn);
			SDL_FreeSurface(cv.at(i)->menu.at(y).tSur);
		}

		cv.at(i)->menu.clear();

		delete cv.at(i);
	}

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

	chatFunc->CleanChat();

	for(unsigned int i = 0; i < win.size(); i++)
	{
		SDL_FreeSurface(win.at(i)->w);

		if(win.at(i)->enableX)
			SDL_FreeSurface(win.at(i)->quitx);

		if(win.at(i)->TYPE == INFOWINDOW)
		{
			SDL_FreeSurface(win.at(i)->expSurface);
			SDL_FreeSurface(win.at(i)->levelSurface);
			SDL_FreeSurface(win.at(i)->hpSurface);
		}

		delete win.at(i);
	}

	for(unsigned int i = 0; i < nv.size(); i++)
	{
		SDL_FreeSurface(nv.at(i)->msg);

		delete nv.at(i);
	}

	for(unsigned int i = 0; i < bullets.size(); i++)
		delete bullets.at(i);

	for(unsigned int i = 0; i < addstatwindow.size(); i++)
	{
		SDL_FreeSurface(addstatwindow.at(i)->window);
		SDL_FreeSurface(addstatwindow.at(i)->add);
		SDL_FreeSurface(addstatwindow.at(i)->quitx);

		if(spawned)
		{
			SDL_FreeSurface(addstatwindow.at(i)->hp);
			SDL_FreeSurface(addstatwindow.at(i)->power);
			SDL_FreeSurface(addstatwindow.at(i)->pointsav);
			SDL_FreeSurface(addstatwindow.at(i)->pkpoints);
			SDL_FreeSurface(addstatwindow.at(i)->servername);
			SDL_FreeSurface(addstatwindow.at(i)->names);
			SDL_FreeSurface(addstatwindow.at(i)->agility);
			SDL_FreeSurface(addstatwindow.at(i)->intt);
		}

		delete addstatwindow.at(i);
	}

	for(unsigned int i = 0; i < partyw.size(); i++)
	{
		SDL_FreeSurface(partyw.at(i)->invite);
		SDL_FreeSurface(partyw.at(i)->kick);
		SDL_FreeSurface(partyw.at(i)->leave);
		SDL_FreeSurface(partyw.at(i)->quitx);
		SDL_FreeSurface(partyw.at(i)->window);
		SDL_FreeSurface(partyw.at(i)->levelSur);

		delete partyw.at(i);
	}

	for(unsigned int i = 0; i < pi.size(); i++)
	{
		SDL_FreeSurface(pi.at(i)->yes);
		SDL_FreeSurface(pi.at(i)->no);
		SDL_FreeSurface(pi.at(i)->from);
		SDL_FreeSurface(pi.at(i)->win);

		delete pi.at(i);
	}

	for(unsigned int i = 0; i < icons.size(); i++)
	{
		SDL_FreeSurface(icons.at(i)->icon);
		SDL_FreeSurface(icons.at(i)->tSurface);

		delete icons.at(i);
	}

	for(unsigned int i = 0; i < drops.size(); i++)
		delete drops.at(i);

	for(unsigned int i = 0; i < dmgs.size(); i++)
	{
		SDL_FreeSurface(dmgs.at(i).dmg);

		if(dmgs.at(i).dmgItem != NULL)
			SDL_FreeSurface(dmgs.at(i).dmgItem);
	}

	for(unsigned int i = 0; i < msgs.size(); i++)
	{
		SDL_FreeSurface(msgs.at(i)->win);
		SDL_FreeSurface(msgs.at(i)->head);
		SDL_FreeSurface(msgs.at(i)->text);

		for(unsigned int y = 0; y < msgs.at(i)->btn.size(); y++)
		{
			SDL_FreeSurface(msgs.at(i)->btn.at(y)->sur);

			delete msgs.at(i)->btn.at(y);
		}

		delete msgs.at(i);
	}

	for(unsigned int i = 0; i < anis.size(); i++)
	{
		if(anis.at(i)->deletePic)
		{
			for(unsigned int y = 0; y < anis.at(i)->ani.size(); y++)
				SDL_FreeSurface(anis.at(i)->ani.at(y));

			anis.at(i)->ani.clear();
		}

		delete anis.at(i);
	}

	for(unsigned int i = 0; i < iw.size(); i++)
	{
		iw.at(i)->Clean(i);

		delete iw.at(i);
	}

	for(unsigned int i = 0; i < lmsgs.size(); i++)
	{
		lmsgs.at(i)->Clean();
		delete lmsgs.at(i);
	}

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

	for(unsigned int i = 0; i < linputs.size(); i++)
	{
		if(linputs.at(i)->win != NULL)
			SDL_FreeSurface(linputs.at(i)->win);

		delete linputs.at(i);
	}

	cm.clear();
	cv.clear();
	npc.clear();
	win.clear();
	nv.clear();
	bullets.clear();
	addstatwindow.clear();
	partyw.clear();
	icons.clear();
	drops.clear();
	dmgs.clear();
	msgs.clear();
	anis.clear();
	iw.clear();
	pi.clear();
	lmsgs.clear();
	charsel.clear();
	linputs.clear();

	if(chatNot != NULL)
		SDL_FreeSurface(chatNot);

	if(spawned)
	{
		for(int i = 0; i < 25; i++)
		{
			if(inv->items[i] != NULL)
			{
				SDL_FreeSurface(inv->items[i]->amount);
				
				delete inv->items[i];
				inv->items[i] = NULL;
			}
		}

		for(int i = 0; i < 4; i++)
		{
			if(inv->eq[i] != NULL)
			{
				SDL_FreeSurface(inv->eq[i]->amount);

				delete inv->eq[i];
				inv->eq[i] = NULL;
			}
		}

		inv->enable = false;
		inv->x = 0;
		inv->y = 0;
		
		SDL_FreeSurface(inv->inv);
		SDL_FreeSurface(inv->golds);
		SDL_FreeSurface(inv->quitx);
	}

	spawned = false;
	PKENABLE = false;
	chatting = false;
	showFps = false;
	chatWindow = false;
	mark->enable = false;
	kördö = false;
	drag = false;
	botActive = false;

	keyPressed = 0;

	dragged.id = 0;
	dragged.pic = NULL;
	dragged.slot = 0;
	dragged.type = 0;
	dragged.x = 0;
	dragged.y = 0;

	if(mark->leftdown != NULL)
		SDL_FreeSurface(mark->leftdown);

	if(mark->rightdown != NULL)
		SDL_FreeSurface(mark->rightdown);

	if(mark->leftup != NULL)
		SDL_FreeSurface(mark->leftup);

	if(mark->rightup != NULL)
		SDL_FreeSurface(mark->rightup);

	if(task->bar != NULL)
		SDL_FreeSurface(task->bar);

	if(task->menu != NULL)
		SDL_FreeSurface(task->menu);

	if(chatInfo != NULL)
		SDL_FreeSurface(chatInfo);

	for(int i = 0; i < 9; i++)
		delete task->pic[i];

	/*
	if(skillLoaded > 0)
	{
		for(int i = 0; i < skillLoaded; i++)
		{
			if(skills[i].text != NULL)
				SDL_FreeSurface(skills[i].text);
		}

		delete skills;
	}
	*/

	if(skill.size() > 0)
	{
		for(unsigned int i = 0; i < skill.size(); i++)
		{
			if(skill.at(i)->text != NULL)
				SDL_FreeSurface(skill.at(i)->text);

			delete skill.at(i);
		}

		skill.clear();
	}

	if(sk->win != NULL)
		SDL_FreeSurface(sk->win);

	if(sk->quitx != NULL)
		SDL_FreeSurface(sk->quitx);

	if(chatIcon.ani.size() > 0)
	{
		if(chatIcon.ani.at(0) != NULL)
			SDL_FreeSurface(chatIcon.ani.at(0));
	}

	if(ow->start != NULL)
		SDL_FreeSurface(ow->start);

	if(ow->bar != NULL)
		SDL_FreeSurface(ow->bar);

	if(ow->stop != NULL)
		SDL_FreeSurface(ow->stop);

	for(unsigned int u = 0; u < ow->text.size(); u++)
	{
		if(ow->text.at(u).s != NULL)
			SDL_FreeSurface(ow->text.at(u).s);
	}

	ow->text.clear();

	if(option->win != NULL)
		SDL_FreeSurface(option->win);

	if(option->quitx != NULL)
		SDL_FreeSurface(option->quitx);

	if(option->buttonFull != NULL)
		SDL_FreeSurface(option->buttonFull);

	if(option->buttonEmpty != NULL)
		SDL_FreeSurface(option->buttonEmpty);

	for(unsigned int i = 0; i < option->op.size(); i++)
	{
		if(option->op.at(i).name != NULL)
			SDL_FreeSurface(option->op.at(i).name);
	}

	option->op.clear();

	if(fpsSurface != NULL)
		SDL_FreeSurface(fpsSurface);

	if(pingSurface != NULL)
		SDL_FreeSurface(pingSurface);

	if(loadingSurface != NULL)
		SDL_FreeSurface(loadingSurface);

	if(manaBar != NULL)
		SDL_FreeSurface(manaBar);

	if(hpBar != NULL)
		SDL_FreeSurface(hpBar);

	if(eBar != NULL)
		SDL_FreeSurface(eBar);

	if(sw->win != NULL)
		SDL_FreeSurface(sw->win);

	if(sw->quitx != NULL)
		SDL_FreeSurface(sw->quitx);

	sw->Clean();

	if(ptrade->win != NULL)
		SDL_FreeSurface(ptrade->win);

	if(ptrade->quitx != NULL)
		SDL_FreeSurface(ptrade->quitx);

	if(ptrade->ok != NULL)
		SDL_FreeSurface(ptrade->ok);

	if(ptrade->cancel != NULL)
		SDL_FreeSurface(ptrade->cancel);

	ptrade->Clean();

	if(markItemShop != NULL)
		SDL_FreeSurface(markItemShop);

	tp->Clean();

	if(tp->win != NULL)
		SDL_FreeSurface(tp->win);

	if(tp->quitx != NULL)
		SDL_FreeSurface(tp->quitx);

	if(loginScreen != NULL)
		SDL_FreeSurface(loginScreen);

	if(loginFirst != NULL)
		SDL_FreeSurface(loginFirst);

	write.Clean();

	if(loginChar != NULL)
		SDL_FreeSurface(loginChar);

	if(loginRem != NULL)
		SDL_FreeSurface(loginRem);

	if(boxFull != NULL)
		SDL_FreeSurface(boxFull);

	if(boxEmpty != NULL)
		SDL_FreeSurface(boxEmpty);

	if(skillShow != NULL)
		SDL_FreeSurface(skillShow);

	skillShow = NULL;

	chatIcon.ani.clear();

	fw->Clean();
	delete fw;

	pms->Clean();
	delete pms;

	delete chatFunc;

	chat = "";

	eAtk = 0;
	CleanPlayer(me);

	CleanPreLoad();
	UnloadAnimations();

	delete sound;
	delete music;

	SDL_EnableUNICODE(SDL_DISABLE);

	TTF_CloseFont(cour);
	TTF_CloseFont(courMedium);
	TTF_CloseFont(courSmall);
	TTF_CloseFont(courBold);
	TTF_CloseFont(courNotice);
	TTF_CloseFont(courOutline);

	TTF_Quit();
	Mix_CloseAudio();
	SDL_Quit();

	cleaned = true;
}

void SmallClean()
{
	PacketQuit();

	for(unsigned int i = 0; i < cm.size(); i++)
	{
		SDL_FreeSurface(cm.at(i)->nameSurface);
		SDL_FreeSurface(cm.at(i)->whenMarkName);

		delete cm.at(i);
	}

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
		}

		for(unsigned int y = 0; y < cv.at(i)->menu.size(); y++)
		{
			SDL_FreeSurface(cv.at(i)->menu.at(y).btn);
			SDL_FreeSurface(cv.at(i)->menu.at(y).tSur);
		}

		cv.at(i)->menu.clear();

		delete cv.at(i);
	}

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

	for(unsigned int i = 0; i < nv.size(); i++)
	{
		SDL_FreeSurface(nv.at(i)->msg);

		delete nv.at(i);
	}

	for(unsigned int i = 0; i < bullets.size(); i++)
		delete bullets.at(i);

	chatFunc->CleanChat();

	for(unsigned int i = 0; i < pi.size(); i++)
	{
		SDL_FreeSurface(pi.at(i)->yes);
		SDL_FreeSurface(pi.at(i)->no);
		SDL_FreeSurface(pi.at(i)->from);
		SDL_FreeSurface(pi.at(i)->win);

		delete pi.at(i);
	}

	for(unsigned int i = 0; i < icons.size(); i++)
	{
		SDL_FreeSurface(icons.at(i)->icon);
		SDL_FreeSurface(icons.at(i)->tSurface);

		delete icons.at(i);
	}

	for(unsigned int i = 0; i < drops.size(); i++)
		delete drops.at(i);

	for(unsigned int i = 0; i < dmgs.size(); i++)
	{
		SDL_FreeSurface(dmgs.at(i).dmg);

		if(dmgs.at(i).dmgItem != NULL)
			SDL_FreeSurface(dmgs.at(i).dmgItem);
	}

	for(unsigned int i = 0; i < msgs.size(); i++)
	{
		SDL_FreeSurface(msgs.at(i)->win);
		SDL_FreeSurface(msgs.at(i)->head);
		SDL_FreeSurface(msgs.at(i)->text);

		for(unsigned int y = 0; y < msgs.at(i)->btn.size(); y++)
		{
			SDL_FreeSurface(msgs.at(i)->btn.at(y)->sur);

			delete msgs.at(i)->btn.at(y);
		}

		delete msgs.at(i);
	}

	for(unsigned int i = 0; i < anis.size(); i++)
	{
		if(anis.at(i)->deletePic)
		{
			for(unsigned int y = 0; y < anis.at(i)->ani.size(); y++)
				SDL_FreeSurface(anis.at(i)->ani.at(y));

			anis.at(i)->ani.clear();
		}

		delete anis.at(i);
	}

	for(unsigned int i = 0; i < iw.size(); i++)
	{
		iw.at(i)->Clean(i);

		delete iw.at(i);
	}

	for(unsigned int i = 0; i < lmsgs.size(); i++)
	{
		lmsgs.at(i)->Clean();
		delete lmsgs.at(i);
	}

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

	for(unsigned int i = 0; i < linputs.size(); i++)
	{
		if(linputs.at(i)->win)
			SDL_FreeSurface(linputs.at(i)->win);

		delete linputs.at(i);
	}

	for(unsigned int i = 0; i < partyw.size(); i++)
	{
		for(unsigned int y = 0; y < partyw.at(i)->pm.size(); y++)
		{
			SDL_FreeSurface(partyw.at(i)->pm.at(y).lvls);
			SDL_FreeSurface(partyw.at(i)->pm.at(y).names);
		}

		partyw.at(i)->pm.clear();
	}

	if(chatNot != NULL)
		SDL_FreeSurface(chatNot);

	chatNot = NULL;

	if(spawned)
	{
		for(int i = 0; i < 25; i++)
		{
			if(inv->items[i] != NULL)
			{
				delete inv->items[i];
				inv->items[i] = NULL;
			}
		}

		for(int i = 0; i < 4; i++)
		{
			if(inv->eq[i] != NULL)
			{
				if(inv->eq[i]->amount != NULL)
					SDL_FreeSurface(inv->eq[i]->amount);

				delete inv->eq[i];
				inv->eq[i] = NULL;
			}
		}

		inv->enable = false;
		
		if(inv->golds != NULL)
			SDL_FreeSurface(inv->golds);

		inv->golds = NULL;

		for(int i = 0; i < 9; i++)
		{
			if(task->pic[i]->stack != NULL)
				SDL_FreeSurface(task->pic[i]->stack);

			delete task->pic[i];
			task->pic[i] = new Picture();

			task->pic[i]->x = ((i * 40) + ((task->bar->w / 2) - 179));
			task->pic[i]->y = (task->y + 4);
		}
	}

	map->pkArea.clear();

	cm.clear();
	cv.clear();
	npc.clear();
	nv.clear();
	bullets.clear();
	pi.clear();
	icons.clear();
	drops.clear();
	dmgs.clear();
	msgs.clear();
	iw.clear();
	lmsgs.clear();
	charsel.clear();
	linputs.clear();

	PKENABLE = false;
	chatting = false;
	mark->enable = false;
	drag = false;
	botActive = false;
	spawned = false;

	keyPressed = 0;

	dragged.id = 0;
	dragged.pic = NULL;
	dragged.slot = 0;
	dragged.type = 0;
	dragged.x = 0;
	dragged.y = 0;

	write.Clean();
	CleanPlayer(me);
}
#include <vector>
#include <SDL_image.h>
#include <SDL_TTF.h>

#include "Packet.h"
#include "Login.h"
#include "Packets.h"
#include "Misc.h"
#include "Game.h"
#include "Main.h"
#include "Music.h"

using namespace std;

extern SOCKET sock;
extern sockaddr_in serv;
extern int len;
extern unsigned char *pac;

extern bool done;

bool colorkey = true;
extern int mCharSpawnId;
extern SDL_Color clrFg;
extern TTF_Font *cour;
extern TTF_Font *courMedium;

extern SDL_Surface *screen;

extern Character *me;

extern bool move;

extern int currBackX;
extern int currBackY;

extern int cameraX;
extern int cameraY;

extern int camShowX;
extern int camShowToX;
extern int camShowY;
extern int camShowToY;

extern int logplace;

extern LoginWriter write;

extern bool wantPacket;
extern int lastSent;

extern vector<CharacterSelect> charsel;

extern SDL_Color clrFgBlack;

extern bool remLogin;
extern char loginName[40];

int Login()
{
	int status = pac[3];
	int timeleft = atoi(getString((char*)pac, 5, (int)pac[4]).c_str());

	switch(status)
	{
		case 1: logplace = CHARSELECT;
			break;

		case 2: LMessage(1, "That username does not exist");
			break;	

		case 3: LMessage(1, "The password is incorrect.");
			break;

		case 4: LMessage(1, "Account already connected.");
			break;

		case 6: LMessage(1, "Account is banned.");
			break;

		case 7: LMessage(1, "Please wait %d seconds.", (timeleft / 1000));
			break;

		default: LMessage(1, "Unknown error: %d.", status);
			break;
	}

	if(status == 1)
	{
		if(remLogin)
		{
			strcpy_s(loginName, write.GetText(1));
			AddConfigOption(0, loginName, 0, 0, TEXTCONFIG, "Login:");
		}

		write.StopWrite();

		PacketCharacterList();
		wantPacket = true;
	}

	else
	{
		if(status != 4 && status != 7)
		{
			write.ar2s = "";

			if(write.ar2 != NULL)
				SDL_FreeSurface(write.ar2);

			write.ar2 = NULL;
		}

		switch(status)
		{
			case 2: write.carea = 1;
				break;

			case 3: write.carea = 2;
				break;

			case 6: write.carea = 1;
				break;
		}
	}

	return 0;
}

void Register()
{
	int status = pac[3];
	int timeleft = atoi(getString((char*)pac, 5, (int)pac[4]).c_str());

	switch(status)
	{
		case 1: LMessage(2, "Register successful.");
			break;

		case 2: LMessage(2, "Account name has already been taken.");
			break;

		case 3: LMessage(2, "Unrecognized letters in account name.");
			break;

		case 7: LMessage(2, "Please wait %d seconds.", (timeleft / 1000));
			break;

		default: LMessage(2, "Unknown error: %d.", status);
			break;
	}
}

void CharacterList()
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

		if(charsel.at(i).intt != NULL)
			SDL_FreeSurface(charsel.at(i).intt);

		charsel.at(i).intt = NULL;
	}

	charsel.clear();

	int pos = 4;

	CharacterSelect cs2 = CharacterSelect();

	cs2.slot = 0;

	for(int i = 0; i < pac[3]; i++)
	{
		CharacterSelect cs = CharacterSelect();

		string level = getString((char*)pac, (pos + 1), (int)pac[pos]);
		pos += ((int)pac[pos] + 1);

		string power = getString((char*)pac, (pos + 1), (int)pac[pos]);
		pos += ((int)pac[pos] + 1);

		string magic = getString((char*)pac, (pos + 1), (int)pac[pos]);
		pos += ((int)pac[pos] + 1);

		string agi = getString((char*)pac, (pos + 1), (int)pac[pos]);
		pos += ((int)pac[pos] + 1);

		string intt = getString((char*)pac, (pos + 1), (int)pac[pos]);
		pos += ((int)pac[pos] + 1);

		int job = pac[pos];
		pos++;

		string name = getString((char*)pac, (pos + 1), (int)pac[pos]);
		pos += ((int)pac[pos] + 1);

		int something = pac[pos];
		int slot = pac[(pos + 1)];

		pos += 2;

		if(something == 1)
			cs.title = RenderText_Solid(courMedium, "Title: PvP champion", clrFgBlack);
		else
			cs.title = RenderText_Solid(courMedium, "Title: None", clrFgBlack);

		cs.dex = RenderText_Solid(courMedium, ("Agility: " + agi).c_str(), clrFgBlack);
		cs.level = RenderText_Solid(courMedium, ("Level: " + level).c_str(), clrFgBlack);
		cs.str = RenderText_Solid(courMedium, ("Strength: " + power).c_str(), clrFgBlack);
		cs.sta = RenderText_Solid(courMedium, ("Stamina: " + magic).c_str(), clrFgBlack);
		cs.name = RenderText_Solid(cour, ("" + name).c_str(), clrFgBlack);
		cs.intt = RenderText_Solid(courMedium, ("Intelligence: " + intt).c_str(), clrFgBlack);
		cs.job = RenderText_Solid(courMedium, ("Job: " + getJobName(job)).c_str(), clrFgBlack);
		cs.slot = slot;
		cs.name2 = name;

		if(cs.slot == 1)
			charsel.push_back(cs);

		else if(cs.slot == 2)
		{
			if(charsel.size() == 0)
				charsel.push_back(cs2);
			
			charsel.push_back(cs);
		}

		else if(cs.slot == 3)
		{
			if(charsel.size() == 0)
			{
				charsel.push_back(cs2);
				charsel.push_back(cs2);
			}

			if(charsel.size() == 1)
				charsel.push_back(cs2);

			charsel.push_back(cs);
		}
	}

	for(int i = charsel.size(); i < 3; i++)
		charsel.push_back(cs2);

	for(int i = 0; i < 3; i++)
	{
		if(charsel.at(i).slot == 0)
			charsel.at(i).deletes = loadimage("Data/Interface/Create-Button.png", true, 255, 255, 255);

		else
		{
			charsel.at(i).createHaha = loadimage("Data/Interface/Login-Button.png", true, 255, 255, 255);
			charsel.at(i).deletes = loadimage("Data/Interface/Delete-Button.png", true, 255, 255, 255);
		}
	}
}

void EditCharacter()
{
	if(lastSent == 0x7)
	{
		switch(pac[3])
		{
			case 0x1:
				break;

			case 0x2: LMessage(3, "Character name already taken.");
				break;

			case 0x3: LMessage(3, "You have already reached the maximum amount of characters.");
				break;

			case 0x4: LMessage(3, "Unrecognized letters in name.");
				break;

			case 0x5: LMessage(3, "Character name is too long.");
				break;

			case 0x6: LMessage(3, "Character name is too short.");
				break;

			case 7: LMessage(3, "You can not be that class.");
				break;

			default: LMessage(3, "Unknown error: %d", pac[3]);
				break;
		}
	}

	else if(lastSent == 0x8)
	{
		switch(pac[3])
		{
			case 1:
				break;

			case 2: LMessage(4, "You can not delete this character.");
				break;	

			case 3: LMessage(4, "That character does not exist.");
				break;

			case 4: LMessage(4, "Account password is wrong.");
				break;

			default: LMessage(4, "Unknown error: %d.", pac[3]);
				break;
		}
	}

	write.StopWrite();

	PacketCharacterList();
	wantPacket = true;
}

void Logout()
{
	Packet pak = Packet();

	pak.addHeader(0x4);

	pak.ready();
}

void Character::Load(int job)
{
	this->my = getPlayerSurface(job);
	this->deadImg = RenderText_Solid(cour, "(dead)", clrFg);

	if(this->my == NULL)
		Log("Error, could not get surface from memory (this->player).");
}

void Monster::Load()
{
	this->m = getMonsterSurface(this->id);

	if(this->m == NULL)
		Log("Error, could not get surface from memory (this->m).");
}

void NPC::Load()
{
	string op = "Data/Mover/NPC/";
	op += this->name;
	op += ".png";

	this->my = loadimage(op.c_str(), true, 0, 0, 0);
}
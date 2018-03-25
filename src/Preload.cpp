#include <SDL_image.h>
#include <fstream>
#include <string>

#include "Main.h"
#include "Game.h"
#include "Login.h"
#include "Misc.h"
#include "Preload.h"
#include "Motion.h"
#include "Packet.h"

using namespace std;

/*
SDL_Surface *player = NULL;
SDL_Surface *bullet = NULL;
SDL_Surface *bulletss = NULL;
SDL_Surface *bulletp = NULL;
SDL_Surface *bulletb = NULL;
SDL_Surface *bulletm = NULL;
SDL_Surface *bullete = NULL;
SDL_Surface *whiteTile = NULL;
*/

SDL_Surface *barFull = NULL;
SDL_Surface *barItem = NULL;

vector<InfoItem> ifitem;
vector<InfoMonster> ifmon;
vector<InfoSkill> ifskill;
vector<InfoBuff> ifbuff;
vector<Tile> iftile;
vector<InfoOT> ifot;
vector<InfoClass> ifplayer;
extern vector<Skill*> skill;
vector<InfoSurface> surs;

extern bool colorkey;

extern TTF_Font *cour;
extern SDL_Color clrFgBlack;

void PreLoad()
{
	/*
	player = loadimage("Data/Mover/Player/Player.png", true, 0, 0, 0);
	bullet = loadimage("Data/Skill/Bullet.png", true, 255, 255, 255);
	bulletss = loadimage("Data/Skill/Bullet-SlowSpeed.png", true, 255, 255, 255);
	bulletp = loadimage("Data/Skill/Bullet-Poison.png", true, 255, 255, 255);
	bulletb = loadimage("Data/Skill/Bullet-Blast.png", true, 255, 255, 255);
	bulletm = loadimage("Data/Skill/Bullet-Missile.png", true, 255, 255, 255);
	bullete = loadimage("Data/Skill/Bullet-EMP.png", true, 255, 255, 255);
	whiteTile = loadimage("Data/Interface/White.png", false, 0, 0, 0);
	*/

	barFull = loadimage("Data/Interface/Ingame-BarFull.png", false, 0, 0, 0);
	barItem = loadimage("Data/Interface/Ingame-Bar.png", false, 0, 0, 0);

	LoadItems();
	LoadMonsters();
	LoadSkills();
	LoadBuffs();
	LoadOtherTexts();
	LoadPlayers();
	LoadSurfaces();
}

void LoadSurfaces()
{
	string temp = "";
	int temp2 = 0;
	int temp3 = 0;
	int temp4 = 0;

	ifstream w("Data/Text/Surface.txt");

	if(!w)
	{
		Log("Could not open file: Data/Text/Surface.txt");
		w.close();

		return;
	}

	while(getline(w, temp))
	{
		InfoSurface is = InfoSurface();

		string loadstring = temp;

		int colorkey = 0, r = 0, g = 0, b = 0;

		getline(w, temp);
		sscanf_s(temp.c_str(), "%d %d %d %d %d", &is.id, &colorkey, &r, &g, &b);

		bool color = false;

		if(colorkey == 1)
			color = true;

		is.pic = loadimage(loadstring.c_str(), color, r, g, b);
		
		surs.push_back(is);

		/*
		sscanf_s(temp.c_str(), "%d", &ic.id);
		
		getline(w, temp);
		string op2 = temp;
		
		getline(w, temp);
		sscanf_s(temp.c_str(), "%d", &temp3);

		for(int i = 0; i < temp3; i++)
		{
			getline(w, temp);
			sscanf_s(temp.c_str(), "%d", &temp2);

			ic.skills.push_back(temp2);
		}

		string op = "Data/Mover/Player/";
		op += op2;

		ic.sur = loadimage(op.c_str(), true, 254, 0, 255);
		
		ifplayer.push_back(ic);
		*/
	}

	w.close();
}

void LoadPlayers()
{
	string temp = "";
	int temp2 = 0;
	int temp3 = 0;
	int temp4 = 0;

	ifstream w("Data/Mover/Player/Player.txt");

	if(!w)
	{
		Log("Could not open file: Data/Mover/Player/Player.txt.");
		w.close();

		return;
	}

	while(getline(w, temp))
	{
		InfoClass ic = InfoClass();

		sscanf_s(temp.c_str(), "%d", &ic.id);
		
		getline(w, temp);
		string op2 = temp;
		
		getline(w, temp);
		sscanf_s(temp.c_str(), "%d", &temp3);

		for(int i = 0; i < temp3; i++)
		{
			getline(w, temp);
			sscanf_s(temp.c_str(), "%d", &temp2);

			ic.skills.push_back(temp2);
		}

		string op = "Data/Mover/Player/";
		op += op2;

		ic.sur = loadimage(op.c_str(), true, 254, 0, 255);
		
		ifplayer.push_back(ic);
	}

	w.close();
}

void LoadItems()
{
	string temp = "";
	int temp2 = 0;
	int temp3 = 0;
	int temp4 = 0;

	ifstream w("Data/Text/Item.txt");

	if(!w)
	{
		Log("Could not open file: Data/Text/Item.txt.");
		w.close();

		return;
	}

	while(getline(w, temp))
	{
		InfoItem ii = InfoItem();

		sscanf_s(temp.c_str(), "%d %d %d %d %d %d %d %d %d %d %d", &ii.id, &ii.effect, &ii.value, &ii.price, &ii.maxstack, &ii.minlvl, &ii.ip.int1, &ii.ip.int2, &ii.ip.int3, &ii.ip.int4, &ii.reqjob);

		getline(w, temp);
		sscanf_s(temp.c_str(), "%d", &temp2);

		for(int i = 0; i < temp2; i++)
		{
			TwoInt tw = TwoInt();

			getline(w, temp);
			sscanf_s(temp.c_str(), "%d %d", &tw.int1, &tw.int2);

			ii.evalues.push_back(tw);
		}

		getline(w, ii.name);
		
		getline(w, temp);
		sscanf_s(temp.c_str(), "%d", &temp2);

		for(int i = 0; i < temp2; i++)
		{
			getline(w, temp);

			ii.desc.push_back(temp);
		}

		string op = "Data/Icon/Item-";
		char temp22[10] = "";

		_itoa_s(ii.id, temp22, 10, 10);

		op += temp22;
		op += "-Icon.png";

		ii.item = loadimage(op.c_str(), true, 255, 255, 255);
		
		ifitem.push_back(ii);
	}

	w.close();
}

void LoadOtherTexts()
{
	string temp = "";

	ifstream w("Data/Text/OtherText.txt");

	if(!w)
	{
		Log("Could not open file: Data/Text/OtherText.txt.");
		w.close();

		return;
	}

	while(getline(w, temp))
	{
		InfoOT ot = InfoOT();
		int lines = 0;

		sscanf_s(temp.c_str(), "%d %d", &ot.id, &lines);

		for(int i = 0; i < lines; i++)
		{
			getline(w, temp);
			ot.line.push_back(temp);
		}

		ifot.push_back(ot);
	}

	w.close();
}

void LoadMonsters()
{
	string temp = "";

	ifstream w("Data/Text/Monster.txt");

	if(!w)
	{
		Log("Could not open file: Data/Text/Monster.txt.");
		w.close();

		return;
	}

	while(getline(w, temp))
	{
		InfoMonster im = InfoMonster();

		sscanf_s(temp.c_str(), "%d", &im.id);

		getline(w, im.name);

		string op = "Data/Mover/Monster/Monster-ID-";

		char t[10] = "";
		_itoa_s(im.id, t, 10, 10);

		op += t;
		op += ".png";

		im.mob = loadimage(op.c_str(), true, 254, 0, 255);

		ifmon.push_back(im);
	}

	w.close();
}

void LoadSkills()
{
	string temp = "";
	int temp2 = 0;

	ifstream w("Data/Text/Skill.txt");

	if(!w)
	{
		Log("Could not open file: Data/Text/Skill.txt.");
		w.close();

		return;
	}

	while(getline(w, temp))
	{
		InfoSkill is = InfoSkill();

		sscanf_s(temp.c_str(), "%d %d %d %d %d", &is.id, &is.invX, &is.invY, &is.take, &is.takeamount);

		getline(w, is.name);

		getline(w, temp);
		sscanf_s(temp.c_str(), "%d", &temp2);
		
		for(int i = 0; i < temp2; i++)
		{
			getline(w, temp);

			is.desc.push_back(temp);
		}

		string op = "Data/Icon/Skill-";

		op += IntToString(is.id);
		op += "-Icon.png";

		is.pic = loadimage(op.c_str(), true, 255, 255, 255);
		is.nameSkill = RenderText_Solid(cour, is.name.c_str(), clrFgBlack);

		ifskill.push_back(is);
	}

	w.close();
}

void LoadBuffs()
{
	string temp = "";
	int temp2 = 0;

	ifstream w("Data/Text/Buff.txt");

	if(!w)
	{
		Log("Could not open file: Data/Text/Buff.txt.");
		w.close();

		return;
	}

	while(getline(w, temp))
	{
		InfoBuff ib = InfoBuff();

		sscanf_s(temp.c_str(), "%d %d", &ib.id, &ib.effect);

		getline(w, temp);
		sscanf_s(temp.c_str(), "%d", &temp2);
		
		for(int i = 0; i < temp2; i++)
		{
			getline(w, temp);

			ib.name.push_back(temp);
		}

		getline(w, temp);
		sscanf_s(temp.c_str(), "%d", &temp2);
		
		for(int i = 0; i < temp2; i++)
		{
			getline(w, temp);

			ib.desc.push_back(temp);
		}

		ifbuff.push_back(ib);
	}

	w.close();
}

void CleanTiles()
{
	for(unsigned int i = 0; i < iftile.size(); i++)
	{
		if(iftile.at(i).su != NULL)
			SDL_FreeSurface(iftile.at(i).su);
	}

	iftile.clear();
}

void CleanPreLoad()
{
	if(barFull != NULL)
		SDL_FreeSurface(barFull);

	if(barItem != NULL)
		SDL_FreeSurface(barItem);

	/*
	if(player != NULL)
		SDL_FreeSurface(player);

	if(bullet != NULL)
		SDL_FreeSurface(bullet);

	if(bulletss != NULL)
		SDL_FreeSurface(bulletss);

	if(bulletp != NULL)
		SDL_FreeSurface(bulletp);

	if(whiteTile != NULL)
		SDL_FreeSurface(whiteTile);

	

	if(bulletb != NULL)
		SDL_FreeSurface(bulletb);

	if(bulletm != NULL)
		SDL_FreeSurface(bulletm);

	if(bullete != NULL)
		SDL_FreeSurface(bullete);
		*/

	barFull = NULL;
	barItem = NULL;
	/*
	player = NULL;
	bullet = NULL;
	bulletss = NULL;
	bulletp = NULL;
	whiteTile = NULL;
	barFull = NULL;
	barItem = NULL;
	*/

	for(unsigned int i = 0; i < ifitem.size(); i++)
	{
		if(ifitem.at(i).item != NULL)
			SDL_FreeSurface(ifitem.at(i).item);
	}

	for(unsigned int i = 0; i < surs.size(); i++)
	{
		if(surs.at(i).pic != NULL)
			SDL_FreeSurface(surs.at(i).pic);
	}

	for(unsigned int i = 0; i < ifmon.size(); i++)
	{
		if(ifmon.at(i).mob != NULL)
			SDL_FreeSurface(ifmon.at(i).mob);
	}

	for(unsigned int i = 0; i < iftile.size(); i++)
	{
		if(iftile.at(i).su != NULL)
			SDL_FreeSurface(iftile.at(i).su);
	}

	for(unsigned int i = 0; i < ifskill.size(); i++)
	{
		if(ifskill.at(i).pic != NULL)
			SDL_FreeSurface(ifskill.at(i).pic);

		if(ifskill.at(i).nameSkill != NULL)
			SDL_FreeSurface(ifskill.at(i).nameSkill);
	}

	for(unsigned int i = 0; i < ifplayer.size(); i++)
	{
		if(ifplayer.at(i).sur != NULL)
			SDL_FreeSurface(ifplayer.at(i).sur);
	}

	ifitem.clear();
	ifmon.clear();
	ifskill.clear();
	ifbuff.clear();
	iftile.clear();
	ifplayer.clear();
	surs.clear();
}

InfoClass *getInfoClass(int PARAM)
{
	for(unsigned int i = 0; i < ifplayer.size(); i++)
	{
		if(ifplayer.at(i).id == PARAM)
		{
			return &ifplayer.at(i);
		}
	}

	Log("Player %d class was not found.", PARAM);

	return NULL;
}

SDL_Surface *getPlayerSurface(int PARAM)
{
	for(unsigned int i = 0; i < ifplayer.size(); i++)
	{
		if(ifplayer.at(i).id == PARAM)
		{
			return ifplayer.at(i).sur;
		}
	}

	Log("Player %d surface was not found.", PARAM);

	return NULL;
}

SDL_Surface *getSurface(int PARAM)
{
	for(unsigned int i = 0; i < surs.size(); i++)
	{
		if(surs.at(i).id == PARAM)
			return surs.at(i).pic;
	}

	Log("Surface %d was not found", PARAM);

	/*
	switch(PARAM)
	{
		case PLAYER: return player;
			break;

		case BULLET: return bullet;
			break;

		case 2: return bulletp;
			break;

		case 3: return bulletss;
			break;

		case 99: return whiteTile;
			break;

		case 4: return bulletb;
			break;

		case 5: return bullete;
			break;

		case 6: return bulletm;
			break;

		default: Log("Invalid param at getSurface(). %d.", PARAM);
	}
	*/

	return NULL;
}

SDL_Surface *getMonsterSurface(int PARAM)
{
	bool found = false;

	for(unsigned int i = 0; i < ifmon.size(); i++)
	{
		if(ifmon.at(i).id == PARAM)
		{
			return ifmon.at(i).mob;

			found = true;
			break;
		}
	}

	if(!found)
		Log("Monster %d surface was not found.", PARAM);

	return NULL;
}

SDL_Surface *getIconSurface(int PARAM)
{
	bool found = false;

	for(unsigned int i = 0; i < ifitem.size(); i++)
	{
		if(ifitem.at(i).id == PARAM)
		{
			return ifitem.at(i).item;

			found = true;
			break;
		}
	}

	if(!found)
		Log("Item %d surface was not found.", PARAM);

	return NULL;
}

int getEquipPos(int PARAM, int ID)
{
	for(unsigned int i = 0; i < ifitem.size(); i++)
	{
		if(ifitem.at(i).id == ID)
		{
			switch(PARAM)
			{
				case INVX: return ifitem.at(i).ip.int1;
					break;

				case INVY: return ifitem.at(i).ip.int2;
					break;

				case PLAYX: return ifitem.at(i).ip.int3;
					break;

				case PLAYY: return ifitem.at(i).ip.int4;
					break;
			}

			break;
		}
	}

	return 0;
}

vector<string> *getBuffText(int id)
{
	for(unsigned int i = 0; i < ifbuff.size(); i++)
	{
		if(ifbuff.at(i).id == id)
			return &ifbuff.at(i).desc;
	}

	Log("Buff %d description was not found.", id);
	return NULL;
}

vector<string> *getSkillText(int id)
{
	for(unsigned int i = 0; i < ifskill.size(); i++)
	{
		if(ifskill.at(i).id == id)
			return &ifskill.at(i).desc;
	}

	Log("Skill %d description was not found.", id);
	return NULL;
}

vector<string> *getOTText(int id)
{
	for(unsigned int i = 0; i < ifot.size(); i++)
	{
		if(ifot.at(i).id == id)
			return &ifot.at(i).line;
	}

	Log("OtherText %d description was not found.", id);
	return NULL;
}

vector<string> *getItemText(int id)
{
	for(unsigned int i = 0; i < ifitem.size(); i++)
	{
		if(ifitem.at(i).id == id)
			return &ifitem.at(i).desc;
	}

	Log("Item %d description was not found.", id);
	return NULL;
}

std::string getItemName(int id)
{
	for(unsigned int i = 0; i < ifitem.size(); i++)
	{
		if(ifitem.at(i).id == id)
			return ifitem.at(i).name;
	}

	Log("Item %d name was not found.", id);
	return "";
}

std::string getBuffNameOne(int id)
{
	for(unsigned int i = 0; i < ifbuff.size(); i++)
	{
		if(ifbuff.at(i).id == id)
		{
			if(ifbuff.at(i).name.size() > 0)
				return ifbuff.at(i).name.at(0);
		}
	}

	Log("Buff %d name #1 was not found.", id);
	return "";
}

std::string getBuffNameTwo(int id)
{
	for(unsigned int i = 0; i < ifbuff.size(); i++)
	{
		if(ifbuff.at(i).id == id)
		{
			if(ifbuff.at(i).name.size() > 1)
				return ifbuff.at(i).name.at(1);
		}
	}

	return "";
}

std::string getMonsterName(int id)
{
	for(unsigned int i = 0; i < ifmon.size(); i++)
	{
		if(ifmon.at(i).id == id)
			return ifmon.at(i).name;
	}

	Log("Monster %d name was not found.", id);
	return "";
}

std::string getSkillName(int id)
{
	for(unsigned int i = 0; i < ifskill.size(); i++)
	{
		if(ifskill.at(i).id == id)
			return ifskill.at(i).name;
	}

	Log("Skill %d name was not found.", id);
	return "";
}

std::string getBuffEffect(int type, int value)
{
	string full = "";
	char buf[10] = "";
	int mult = 1;

	if(type == HPDRAIN)
		mult = 4;

	_itoa_s(value * mult, buf, 10, 10);

	switch(type)
	{
		case SPEED: full += "Speed +";
			break;

		case STAMINA: full += "Stamina +";
			break;

		case STRENGTH: full += "Strength +";
			break;

		case SPEEDMINUS: full += "Speed -";
			break;

		case HPDRAIN: full += "Drain ";
			break;

		case AGILITY: full += "Agility +";
			break;

		case INTT: full += "Intelligence +";
			break;
	}

	full += buf;

	if(type == HPDRAIN)
		full += " HP/s";

	return full;
}

int getItemPrice(int id)
{
	for(unsigned int i = 0; i < ifitem.size(); i++)
	{
		if(ifitem.at(i).id == id)
			return ifitem.at(i).price;
	}

	Log("Item %d price was not found.", id);
	return 0;
}

int getItemStack(int id)
{
	for(unsigned int i = 0; i < ifitem.size(); i++)
	{
		if(ifitem.at(i).id == id)
			return ifitem.at(i).maxstack;
	}

	Log("Item %d max-stack was not found.", id);
	return 0;
}

int getItemEffectById(int id)
{
	for(unsigned int i = 0; i < ifitem.size(); i++)
	{
		if(ifitem.at(i).id == id)
			return ifitem.at(i).effect;
	}

	Log("Item %d effect was not found.", id);
	return 0;
}

int getItemValue(int id)
{
	for(unsigned int i = 0; i < ifitem.size(); i++)
	{
		if(ifitem.at(i).id == id)
			return ifitem.at(i).value;
	}

	Log("Item %d value was not found.", id);
	return 0;
}

InfoItem *getItem(int id)
{
	for(unsigned int i = 0; i < ifitem.size(); i++)
	{
		if(ifitem.at(i).id == id)
			return &ifitem.at(i);
	}

	Log("Item %d was not found.", id);
	return NULL;
}

Tile *getTile(int id)
{
	for(unsigned int i = 0; i < iftile.size(); i++)
	{
		if(iftile.at(i).id == id)
			return &iftile.at(i);
	}

	Tile t = Tile();
	
	string op = "Data/Tile/Tile-";
	char temp22[10] = "";

	t.id = id;
	_itoa_s(id, temp22, 10, 10);

	op += temp22;
	op += ".png";

	//t.su = loadimage(op.c_str(), true, 255, 255, 254);
	t.su = loadimage(op.c_str(), false, 0, 0, 0);

	if(t.su != NULL)
	{
		iftile.push_back(t);
		return &iftile.back();
	}

	else
	{
		Log("Tile %d was not found.", id);
		return NULL;
	}
}

std::vector<int> getSkillIds()
{
	std::vector<int> gsi;

	for(unsigned int i = 0; i < ifskill.size(); i++)
		gsi.push_back(ifskill.at(i).id);

	return gsi;
}

InfoSkill *getSkill(int id)
{
	for(unsigned int i = 0; i < ifskill.size(); i++)
	{
		if(ifskill.at(i).id == id)
			return &ifskill.at(i);
	}

	Log("Skill %d was not found.", id);
	return NULL;
}

InfoOT *getOT(int id)
{
	for(unsigned int i = 0; i < ifot.size(); i++)
	{
		if(ifot.at(i).id == id)
			return &ifot.at(i);
	}

	Log("OtherText %d was not found.", id);
	return NULL;
}

Skill *getMySkill(int id)
{
	for(unsigned int i = 0; i < skill.size(); i++)
	{
		if(skill.at(i)->id == id)
			return skill.at(i);
	}

	return NULL;
}

std::string getJobName(int id)
{
	switch(id)
	{
		case 1: return "Smiley";
			break;

		case 2: return "Blade";
			break;

		case 3: return "Ninja";
			break;

		case 4: return "Mage";
			break;

		default: return "";
	}

	return "";
}
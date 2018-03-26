#ifndef PRELOAD_H
#define PRELOAD_H

#define INVX 2000
#define INVY 2001
#define PLAYX 2002
#define PLAYY 2003

class TwoInt
{
public:
	TwoInt()
	{
		this->int1 = 0;
		this->int2 = 0;
	}

	TwoInt(const TwoInt &c)
	{
		this->int1 = c.int1;
		this->int2 = c.int2;
	}

	int int1;
	int int2;
};

class InfoItem
{
public:
	InfoItem()
	{
		this->id = 0;
		this->effect = 0;
		this->maxstack = 0;
		this->price = 0;
		this->value = 0;
		this->minlvl = 0;

		this->name = "";

		this->item = NULL;

		this->reqjob = 0;
	}

	int id;
	int effect;
	int maxstack;
	int price;
	int value;
	int minlvl;

	std::string name;

	SDL_Surface *item;

	FourInt ip;

	std::vector<TwoInt> evalues;
	std::vector<std::string> desc;

	int reqjob;
};

class InfoClass
{
public:
	InfoClass()
	{
		this->id = 0;
		this->sur = NULL;
	}

	int id;
	SDL_Surface *sur;

	std::vector<int> skills;
};

class InfoMonster
{
public:
	InfoMonster()
	{
		this->id = 0;

		this->name = "";

		this->mob = NULL;
	}

	int id;

	std::string name;

	SDL_Surface *mob;
};

class InfoSkill
{
public:
	InfoSkill()
	{
		this->id = 0;

		this->name = "";

		this->invX = 0;
		this->invY = 0;

		this->pic = NULL;

		this->take = 0;
		this->takeamount = 0;

		this->nameSkill = NULL;
	}

	int id;

	std::string name;

	int invX;
	int invY;

	std::vector<std::string> desc;

	SDL_Surface *pic;

	int take;
	int takeamount;

	SDL_Surface *nameSkill;
};

class InfoSurface
{
public:
	InfoSurface()
	{
		this->id = 0;
		this->pic = NULL;
	}

	int id;
	SDL_Surface *pic;
};
class InfoBuff
{
public:
	InfoBuff()
	{
		this->id = 0;
		this->effect = 0;
	}

	int id;
	int effect;

	std::vector<std::string> name;
	std::vector<std::string> desc;
};

class InfoOT
{
public:
	InfoOT()
	{
		this->id = 0;
	}

	int id;
	
	std::vector<std::string> line;
};

void LoadItems();
void LoadMonsters();
void LoadSkills();
void LoadBuffs();
void LoadOtherTexts();
void LoadPlayers();
void LoadSurfaces();

int getEquipPos(int PARAM, int ID);
std::vector<std::string> *getItemText(int id);
std::vector<std::string> *getSkillText(int id);
std::vector<std::string> *getBuffText(int id);
std::string getItemName(int id);
std::string getMonsterName(int id);
std::string getBuffNameOne(int id);
std::string getBuffNameTwo(int id);
std::string getSkillName(int id);
std::string getBuffEffect(int type, int value);
std::vector<int> getSkillIds();
int getItemPrice(int id);
int getItemStack(int id);
int getItemEffectById(int id);
int getItemValue(int id);
InfoItem *getItem(int id);
Tile *getTile(int id);
InfoSkill *getSkill(int id);
void CleanTiles();
InfoOT *getOT(int id);
std::vector<std::string> *getOTText(int id);
SDL_Surface *getPlayerSurface(int PARAM);
InfoClass *getInfoClass(int PARAM);
Skill *getMySkill(int id);
std::string getJobName(int id);

#endif
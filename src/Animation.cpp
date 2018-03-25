#include <fstream>

#include "Animation.h"
#include "Misc.h"
#include "Main.h"

using namespace std;

vector<Animation*> skillanis;
vector<Animation*> anis;

void LoadAnimation(Animation *ani)
{
	ifstream op;

	int images = 0;
	int fps = 0;

	int trans = 0;

	int r = 0;
	int g = 0;
	int b = 0;

	int disTime;

	bool transs = false;

	char temp[10] = "";
	string ops = "Data/Animation/Skill/";

	_itoa_s(ani->id, temp, 10, 10);

	ops += temp;
	ops += "/";

	string fileop = ops;
	string picOp = ops;
	string tempOp = ops;

	fileop += "Index.txt";

	op.open(fileop.c_str());

	op >> fps;
	op >> images;
	op >> disTime;
	op >> trans;

	if(trans == 1)
	{
		transs = true;

		op >> r;
		op >> g;
		op >> b;
	}

	op.close();

	ani->fps = fps;
	ani->disTimeSec = disTime;

	for(int i = 0; i < images; i++)
	{
		tempOp = picOp;

		_itoa_s((i + 1), temp, 10, 10);

		tempOp += "Picture-";
		tempOp += temp;
		tempOp += ".png";

		SDL_Surface *ptemp = loadimage(tempOp.c_str(), transs, r, g, b);

		ani->ani.push_back(ptemp);
	}
}

void LoadAnimations()
{
	int skill = 1;

	for(int i = 0; i < skill; i++)
	{
		Animation *ani = new Animation();

		ani->id = (i + 1);

		LoadAnimation(ani);	

		skillanis.push_back(ani);
	}
}

void UnloadAnimations()
{
	for(unsigned int i = 0; i < skillanis.size(); i++)
	{
		for(unsigned int w = 0; w < skillanis.at(i)->ani.size(); w++)
		{
			if(skillanis.at(i)->ani.at(w) != NULL)
				SDL_FreeSurface(skillanis.at(i)->ani.at(w));
		}

		delete skillanis.at(i);
	}

	skillanis.clear();
}

Animation *GetSkillAnimation(int id)
{
	for(unsigned int i = 0; i < skillanis.size(); i++)
	{
		if(skillanis.at(i)->id == id)
			return skillanis.at(i);
	}

	Log("Error, could not find skill animation with id: %d.", id);

	return NULL;
}
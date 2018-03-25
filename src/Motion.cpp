#include <SDL_ttf.h>
#include <string>

#include "Game.h"
#include "Misc.h"
#include "Preload.h"
#include "Motion.h"
#include "Main.h"
#include "Packet.h"
#include "Information.h"
#include "Friend.h"

using namespace std;

extern vector<Icon*> icons;
extern vector<Monster*> cm;
extern vector<Character*> cv;
extern vector<InputWindow*> iw;
extern vector<Drop*> drops;
extern vector<Skill*> skill;

extern Inventory *inv;
extern ObjectInfoWindow *ow;
extern Taskbar *task;
extern SkillWindow *sk;
extern ShopWindow *sw;
extern Mark *mark;
extern PlayerTrade *ptrade;
extern FriendWindow *fw;
extern TeleportWindow *tp;
extern OptionWindow *option;
extern vector<AddStatWindow*> addstatwindow;
extern vector<Message*> msgs;
extern vector<PartyInvite*> pi;
extern vector<Window*> win;
extern vector<PartyWindow*> partyw;

extern TTF_Font *courBold;
extern TTF_Font *courMedium;
extern SDL_Color clrFgBlack;
extern SDL_Color clrFgBlue;
extern SDL_Color clrFgRed;
extern SDL_Color clrFgGreen;
extern SDL_Color clrFg;
extern SDL_Color clrFgBajs;

extern Character *me;

extern int skillLoaded;

bool showProcent = false;

void AddObjectInformation(int id, int type)
{
	vector<string> *t = NULL;

	switch(type)
	{
		case INVENTORYITEM: case INVENTORYITEMEQUIP: case TASKBARINVENTORYITEM: case TASKBARINVENTORYITEMEQUIP: case BUYITEM: case CARTITEM: case INPUTITEM: case DROPITEM: case TRADEITEMSIDEONE: case TRADEITEMSIDETWO:t = getItemText(id);
			break;

		case SKILL: case TASKBARSKILL: case SKILLWINDOW: t = getSkillText(id);
			break;

		case BUFF: t = getBuffText(id);
			break;

		case WINDOW_ADDSTAT: t = getOTText(id);
			break;
	}

	if(t != NULL)
	{
		ow->nBars++;

		if(t->size() > 0)
		{
			SurfaceWithInt swi = SurfaceWithInt();

			string op = "Info: ";

			op += t->at(0);

			swi.s = RenderText_Blended(courMedium, op.c_str(), clrFgBlack);
			swi.i = (ow->nBars + 1);

			ow->text.push_back(swi);
			ow->nBars++;

			for(unsigned int i = 1; i < t->size(); i++)
			{
				SurfaceWithInt swi2 = SurfaceWithInt();

				swi2.s = RenderText_Blended(courMedium, t->at(i).c_str(), clrFgBlack);
				swi2.i = (ow->nBars + 1);

				ow->text.push_back(swi2);
				ow->nBars++;
			}
		}
	}
}

void CreateObjectInfo(int type, int item, int x, int y, int value, int effect, string name, int id, int skillId, bool showprice, int minlvl)
{
	bool addMore = false;
	bool addLevel = false;
	bool addJob = false;

	ow->currType = type;
	ow->currItem = item;
	ow->names = name;

	ow->x = x;
	ow->y = y;

	for(unsigned int u = 0; u < ow->text.size(); u++)
		SDL_FreeSurface(ow->text.at(u).s);

	ow->text.clear();

	if(type == INVENTORYITEM || type == TASKBARINVENTORYITEM || type == INVENTORYITEMEQUIP || type == TASKBARINVENTORYITEMEQUIP || type == BUYITEM || type == CARTITEM || type == INPUTITEM || type == DROPITEM || type == TRADEITEMSIDEONE || type == TRADEITEMSIDETWO)
	{
		string op = getItemEffect(effect);
		char buf[20] = "";

		if(type == BUYITEM || type == CARTITEM || showprice)
			op = "Price: ";

		if(sw->enable)
		{
			if(type == BUYITEM)
				_itoa_s(getItemPrice(id), buf, 20, 10);

			else if(type == CARTITEM)
				_itoa_s((getItemPrice(id) * sw->cart[(item - 1)].itemQty), buf, 20, 10);

			else if(showprice)
				_itoa_s(((getItemPrice(id) * iw.at((item - 1))->arg2) / 2), buf, 20, 10);

			else
				_itoa_s((getItemPrice(id) / 2), buf, 20, 10);
		}

		else
			_itoa_s(value, buf, 20, 10);

		op += buf;
		op += ".";

		int doBars = 1;

		SurfaceWithInt swi = SurfaceWithInt();
		SurfaceWithInt swi2 = SurfaceWithInt();
		SurfaceWithInt swi3 = SurfaceWithInt();
		SurfaceWithInt swiPlus = SurfaceWithInt();
		SurfaceWithInt swiLvl = SurfaceWithInt();
		SurfaceWithInt swiJob = SurfaceWithInt();
		vector<SurfaceWithInt> swiVal;

		if((sw->enable && type == CARTITEM) || (ptrade->enable && (type == TRADEITEMSIDEONE || type == TRADEITEMSIDETWO)) || (showprice && type == INPUTITEM))
		{
			name += " x";
			
			if(sw->enable)
			{
				if(type != INPUTITEM)
					_itoa_s(sw->cart[(item - 1)].itemQty, buf, 20, 10);

				else
					_itoa_s(iw.at((item - 1))->arg2, buf, 20, 10);
			}

			else
			{
				if(type == TRADEITEMSIDEONE)
					_itoa_s(ptrade->si[(item - 1)].itemQty, buf, 20, 10);

				if(type == TRADEITEMSIDETWO)
					_itoa_s(ptrade->si2[(item - 1)].itemQty, buf, 20, 10);

				if(type == INPUTITEM)
					_itoa_s(iw.at((item - 1))->arg2, buf, 20, 10);
			}

			name += buf;
		}

		swi.s = RenderText_Blended(courBold, name.c_str(), clrFgBlack);
		swi.i = doBars;

		doBars++;

		if(value != 0 || sw->enable)
		{
			if(strcmp(op.c_str(), "") != 0)
			{
				doBars++;

				swi2.s = RenderText_Blended(courMedium, op.c_str(), clrFgBlue);
				swi2.i = doBars;
			}
		}

		if(effect != 0)
		{
			if(sw->enable && (type == BUYITEM || type == CARTITEM))
			{
				string pp = getItemEffectShop(getItemEffectById(id));

				if(strcmp(pp.c_str(), "") != 0)
				{
					_itoa_s(getItemValue(id), buf, 20, 10);

					pp += buf;
					pp += ".";

					doBars++;
					swiPlus.i = doBars;
					swiPlus.s = RenderText_Blended(courMedium, pp.c_str(), clrFgBlue);

					addMore = true;
				}
			}
		}

		if(getItem(id)->evalues.size() > 0)
		{
			InfoItem *ii = getItem(id);

			for(unsigned int i = 0; i < ii->evalues.size(); i++)
			{
				string opss = getItemExtraValue(ii->evalues.at(i).int1, ii->evalues.at(i).int2);

				SurfaceWithInt si = SurfaceWithInt();

				si.s = RenderText_Blended(courMedium, opss.c_str(), clrFgBlack);

				doBars++;
				si.i = doBars;

				swiVal.push_back(si);
			}
		}

		if(minlvl != 0)
		{
			addLevel = true;

			string lvls = "Required level: ";
			lvls += IntToString(minlvl);
			lvls += ".";

			if(me->level >= minlvl)
				swiLvl.s = RenderText_Blended(courMedium, lvls.c_str(), clrFgBlack);
			
			else
				swiLvl.s = RenderText_Blended(courMedium, lvls.c_str(), clrFgRed);

			doBars++;
			swiLvl.i = doBars;
		}

		InfoItem *ii = getItem(id);

		if(ii != NULL)
		{
			if(ii->reqjob != 0)
			{
				addJob = true;

				string jobs = "Required class: ";
				jobs += getJobName(ii->reqjob);
				jobs += ".";

				if(me->job == ii->reqjob)
					swiJob.s = RenderText_Blended(courMedium, jobs.c_str(), clrFgBlack);

				else
					swiJob.s = RenderText_Blended(courMedium, jobs.c_str(), clrFgRed);

				doBars++;
				swiJob.i = doBars;
			}
		}

		if(effect != 0)
		{
			if(sw->enable && type != BUYITEM && type != CARTITEM)
				swi3.s = RenderText_Blended(courMedium, "Usage: Right click to sell.", clrFgBlack);

			else
				swi3.s = RenderText_Blended(courMedium, "Usage: Right click to activate.", clrFgBlack);

			doBars++;
			swi3.i = doBars;
		}

		ow->nBars = doBars;

		ow->text.push_back(swi);
		ow->text.push_back(swi2);
		ow->text.push_back(swi3);

		if(addMore)
			ow->text.push_back(swiPlus);

		if(swiVal.size() > 0)
		{
			for(unsigned int i = 0; i < swiVal.size(); i++)
				ow->text.push_back(swiVal.at(i));
		}

		if(addLevel)
			ow->text.push_back(swiLvl);

		if(addJob)
			ow->text.push_back(swiJob);
	}

	else if(type == SKILL || type == TASKBARSKILL || type == SKILLWINDOW)
	{
		//char tempLevel[20] = "";
		SurfaceWithInt swi = SurfaceWithInt();
		SurfaceWithInt swi2 = SurfaceWithInt();

		Skill *mySkill = getMySkill(skillId);

		if(mySkill == NULL)
			Log("MYSKILL: 0 ID: %d void CreateObjectInfo()", skillId);

		//_itoa_s(skills[(skillId - 1)].level, tempLevel, 20, 10);

		string fullString = name;

		fullString += " level ";
		fullString += IntToString(mySkill->level);
		//fullString += tempLevel;

		swi.s = RenderText_Blended(courBold, fullString.c_str(), clrFgBlack);
		swi.i = 1;

		ow->nBars = 1;

		string takes = "Cost: ";

		if(mySkill->take != 0)
		{
			takes += IntToString(mySkill->takeamount);
		}

		SDL_Color clrFgColor = clrFg;

		switch(mySkill->take)
		{
			case 0: takes += "No Cost"; clrFgColor = clrFg;
				break;

			case HPUPDATE: takes += " HP"; clrFgColor = clrFgRed;
				break;

			case MANAUPDATE: takes += " MP"; clrFgColor = clrFgBlue;
				break;

			case ENERGYUPDATE: takes += " KP"; clrFgColor = clrFgBajs;
				break;
		}

		swi2.s = RenderText_Blended(courMedium, takes.c_str(), clrFgColor);
		swi2.i = 3;

		ow->nBars += 1;

		/*
		if(effectInfo != "")
		{
			swi2.s = RenderText_Blended(courMedium, effectInfo.c_str(), clrFgBlue);
			swi2.i = 3;

			ow->nBars += 2;
		}
		*/

		ow->text.push_back(swi);
		ow->text.push_back(swi2);
	}

	else if(type == WINDOW_ADDSTAT)
	{
		SurfaceWithInt swi = SurfaceWithInt();
		SurfaceWithInt swi2 = SurfaceWithInt();

		string fullString = name;

		swi.s = RenderText_Blended(courBold, fullString.c_str(), clrFgBlack);
		swi.i = 1;

		ow->nBars = 1;

		ow->text.push_back(swi);
		ow->text.push_back(swi2);
	}

	else if(type == BUFF)
	{
		char tempLevel[20] = "";
		SurfaceWithInt swi = SurfaceWithInt();
		SurfaceWithInt swi2 = SurfaceWithInt();

		_itoa_s(value, tempLevel, 20, 10);

		string fullString = name;

		fullString += " level ";
		fullString += tempLevel;

		swi.s = RenderText_Blended(courBold, fullString.c_str(), clrFgBlack);
		swi.i = 1;

		ow->nBars = 1;

		std::string effectInfo = getBuffEffect(effect, value);

		if(effectInfo != "")
		{
			swi2.s = RenderText_Blended(courMedium, effectInfo.c_str(), clrFgBlue);
			swi2.i = 3;

			ow->nBars += 2;
		}

		ow->text.push_back(swi);
		ow->text.push_back(swi2);
	}

	AddObjectInformation(id, type);
}

void CheckMonsterInfo(int x, int y, bool hit, int type, Monster *mCheck, Character *cCheck, bool showLvl)
{
	bool borrHit = false;
	bool hitOldProcent = false;

	int idHit = 0;
	int typeHit = 0;

	if(showProcent)
	{
		for(int i = (cm.size() - 1); i >= 0; i--)
		{
			if(cm.at(i)->showProc && !thisMarkedMonster(cm.at(i)))
			{
				cm.at(i)->showProc = false;

				if(cm.at(i)->procentType == 1)
				{
					hitOldProcent = true;
					idHit = i;
					typeHit = 1;
				}

				break;
			}
		}

		for(int i = (cv.size() - 1); i >= 0; i--)
		{
			if(cv.at(i)->showProc && !thisMarkedPlayer(cv.at(i)))
			{
				cv.at(i)->showProc = false;

				if(cv.at(i)->procentType == 1)
				{
					hitOldProcent = true;
					idHit = i;
					typeHit = 0;
				}

				break;
			}
		}

		showProcent = false;
	}

	if(!hit)
	{
		if(mCheck != NULL)
		{
			if(!mCheck->showProc)
			{
				mCheck->showProc = true;
				mCheck->showLvl = showLvl;
				mCheck->procentType = type;

				if(type == 0)
					mCheck->enableTimer = false;
				else
				{
					mCheck->enableTimer = true;
					mCheck->timerProc = (getTime(true) + 5000);
				}

				showProcent = true;
			}

			for(unsigned int y = 0; y < cm.size(); y++)
			{
				if(cm.at(y)->showProc && cm.at(y)->mId != mCheck->mId && !thisMarkedMonster(cm.at(y)))
					cm.at(y)->showProc = false;
			}

			borrHit = true;
		}

		else
		{
			for(int i = (cm.size() - 1); i >= 0; i--)
			{
				if(OverButton(cm.at(i)->cX, cm.at(i)->cY, cm.at(i)->m->w, cm.at(i)->m->h, x, y))
				{
					if(!cm.at(i)->showProc)
					{
						cm.at(i)->showProc = true;
						cm.at(i)->showLvl = showLvl;
						cm.at(i)->procentType = type;

						if(type == 0)
							cm.at(i)->enableTimer = false;
						else
						{
							cm.at(i)->enableTimer = true;
							cm.at(i)->timerProc = (getTime(true) + 5000);
						}

						showProcent = true;
					}

					for(int y = (i - 1); y >= 0; y--)
					{
						if(cm.at(y)->showProc && !thisMarkedMonster(cm.at(i)))
							cm.at(y)->showProc = false;
					}
					
					borrHit = true;

					break;
				}

				else
				{
					if(cm.at(i)->showProc && !thisMarkedMonster(cm.at(i)))
						cm.at(i)->showProc = false;
				}
			}
		}
	}

	if(!hit && !borrHit)
	{
		if(cCheck != NULL)
		{
			if(!cCheck->showProc)
			{
				cCheck->showProc = true;
				cCheck->procentType = type;

				if(type == 0)
					cCheck->enableTimer = false;
				else
				{
					cCheck->enableTimer = true;
					cCheck->timerProc = (getTime(true) + 5000);
				}

				showProcent = true;
			}

			for(unsigned int y = 0; y < cv.size(); y++)
			{
				if(cv.at(y)->showProc && cv.at(y)->id != cCheck->id && !thisMarkedPlayer(cv.at(y)))
					cv.at(y)->showProc = false;
			}

			borrHit = true;
		}

		else
		{
			for(int i = (cv.size() - 1); i >= 0; i--)
			{
				if(OverButton(cv.at(i)->cX, cv.at(i)->cY, cv.at(i)->my->w, cv.at(i)->my->h, x, y))
				{
					if(!cv.at(i)->showProc)
					{
						cv.at(i)->showProc = true;
						cv.at(i)->procentType = type;

						if(type == 0)
							cv.at(i)->enableTimer = false;
						else
						{
							cv.at(i)->enableTimer = true;
							cv.at(i)->timerProc = (getTime(true) + 5000);
						}

						showProcent = true;
					}

					for(int y = (i - 1); y >= 0; y--)
					{
						if(cv.at(y)->showProc && !thisMarkedPlayer(cv.at(y)))
							cv.at(y)->showProc = false;
					}
					
					borrHit = true;

					break;
				}

				else
				{
					if(cv.at(i)->showProc && !thisMarkedPlayer(cv.at(i)))
						cv.at(i)->showProc = false;
				}
			}
		}
	}

	if(!hit && !borrHit)
	{
		if(hitOldProcent)
		{
			if(typeHit == 0)
				cv.at(idHit)->showProc = true;

			if(typeHit == 1)
				cm.at(idHit)->showProc = true;

			showProcent = true;
		}
	}
}

bool ObjectInfoWindowFunction(int x, int y, int lastX, int lastY)
{
	bool newMan = false;

	if(lastX != x || lastY != y)
		newMan = true;

	if(task->enableTask)
	{
		bool overAll = OverButton(task->x, task->y, task->bar->w, task->bar->h, x, y);

		for(int i = 0; i < 9; i++)
		{
			if(task->pic[i]->enable)
			{
				if(task->pic[i]->item != NULL)
				{
					if(OverButton(task->pic[i]->itemX, task->pic[i]->itemY, task->pic[i]->item->w, task->pic[i]->item->h, x, y))
					{
						int trys = INVENTORYITEM;
						std::string nameString = "";

						switch(task->pic[i]->type)
						{
							case INVENTORYITEM:
								{
									trys = TASKBARINVENTORYITEM;

									if(!task->pic[i]->eqinv)
										nameString = getItemName(task->pic[i]->itemid);
									else
										nameString = getItemName(inv->eq[(task->pic[i]->slot - 1)]->id);
								break;
								}

							case SKILL: trys = TASKBARSKILL; nameString = getSkillName(getSkill(task->pic[i]->slot)->id);//getSkillName(skills[(task->pic[i]->slot - 1)].id);
								break;
						}

						if(ow->currType == trys && ow->currItem == (i + 1) && ow->names == nameString && !newMan)
							return true;

						switch(trys)
						{
							case TASKBARINVENTORYITEM:
								{
									if(!task->pic[i]->eqinv)
										CreateObjectInfo(trys, (i + 1), x, (y - 13), getItemValue(task->pic[i]->itemid), getItemEffectById(task->pic[i]->itemid), nameString, task->pic[i]->itemid, 0, false, getItem(task->pic[i]->itemid)->minlvl);
									else
										CreateObjectInfo(trys, (i + 1), x, (y - 13), inv->eq[(task->pic[i]->slot - 1)]->value, inv->eq[(task->pic[i]->slot - 1)]->effect, nameString, inv->eq[(task->pic[i]->slot - 1)]->id, 0, false, getItem(inv->eq[(task->pic[i]->slot - 1)]->id)->minlvl);
								break;
								}

							case TASKBARSKILL: CreateObjectInfo(trys, (i + 1), x, (y - 13), 0, 0, nameString, getSkill(task->pic[i]->slot)->id/*skills[(task->pic[i]->slot - 1)].id*/, getSkill(task->pic[i]->slot)->id, false, 0);
								break;
						}

						return true;
					}
				}
			}
		}

		if(overAll)
			return false;
	}

	if(iw.size() > 0)
	{
		for(int i = (iw.size() - 1); i >= 0; i--)
		{
			bool overAll = OverButton(iw.at(i)->x, iw.at(i)->y, iw.at(i)->win->w, iw.at(i)->win->h, x, y);

			if(iw.at(i)->btn3 != NULL)
			{
				if(OverButton((iw.at(i)->x + (193 - (iw.at(i)->btn3->w / 2))), (iw.at(i)->y + (28 - (iw.at(i)->btn3->w / 2))), iw.at(i)->btn3->w, iw.at(i)->btn3->h, x, y))
				{
					if(ow->currType == INPUTITEM && ow->currItem == (i + 1) && ow->names == getItemName(iw.at(i)->arg3) && !newMan)
						return true;

					bool showp = false;

					if(iw.at(i)->type == NPCTRADESELL)
						showp = true;

					CreateObjectInfo(INPUTITEM, (i + 1), x, (y - 13), (getItemValue(iw.at(i)->arg3) * iw.at(i)->arg2), getItemEffectById(iw.at(i)->arg3), getItemName(iw.at(i)->arg3), iw.at(i)->arg3, 0, showp, getItem(iw.at(i)->arg3)->minlvl);

					return true;
				}
			}

			if(overAll)
				return false;
		}
	}

	if(fw->enable)
	{
		if(OverButton(fw->x, fw->y, fw->win->w, fw->win->h, x, y))
			return false;
	}

	if(tp->enable)
	{
		if(OverButton(tp->x, tp->y, tp->win->w, tp->win->h, x, y))
			return false;
	}

	if(ptrade->enable)
	{
		bool overAll = OverButton(ptrade->x, ptrade->y, ptrade->win->w, ptrade->win->h, x, y);

		for(int i = 0; i < 20; i++)
		{
			if(ptrade->si[i].itemId != 0)
			{
				if(OverButton(ptrade->si[i].x, ptrade->si[i].y, ptrade->si[i].pic->w, ptrade->si[i].pic->h, x, y))
				{
					if(ow->currType == TRADEITEMSIDEONE && ow->currItem == (i + 1) && ow->names == getItemName(ptrade->si[i].itemId) && !newMan)
						return true;

					CreateObjectInfo(TRADEITEMSIDEONE, (i + 1), x, (y - 13), getItemValue(ptrade->si[i].itemId), getItemEffectById(ptrade->si[i].itemId), getItemName(ptrade->si[i].itemId), ptrade->si[i].itemId, 0, false, getItem(ptrade->si[i].itemId)->minlvl);

					return true;
				}
			}
		}

		for(int i = 0; i < 20; i++)
		{
			if(ptrade->si2[i].itemId != 0)
			{
				if(OverButton(ptrade->si2[i].x, ptrade->si2[i].y, ptrade->si2[i].pic->w, ptrade->si2[i].pic->h, x, y))
				{
					if(ow->currType == TRADEITEMSIDETWO && ow->currItem == (i + 1) && ow->names == getItemName(ptrade->si2[i].itemId) && !newMan)
						return true;

					CreateObjectInfo(TRADEITEMSIDETWO, (i + 1), x, (y - 13), getItemValue(ptrade->si2[i].itemId), getItemEffectById(ptrade->si2[i].itemId), getItemName(ptrade->si2[i].itemId), ptrade->si2[i].itemId, 0, false, getItem(ptrade->si2[i].itemId)->minlvl);

					return true;
				}
			}
		}

		if(overAll)
			return false;
	}

	if(sw->enable)
	{
		bool overAll = OverButton(sw->x, sw->y, sw->win->w, sw->win->h, x, y);

		for(int i = 0; i < 20; i++)
		{
			if(sw->si[i].itemId != 0)
			{
				if(OverButton(sw->si[i].x, sw->si[i].y, sw->si[i].pic->w, sw->si[i].pic->h, x, y))
				{
					if(ow->currType == BUYITEM && ow->currItem == (i + 1) && ow->names == getItemName(sw->si[i].itemId) && !newMan)
						return true;

					CreateObjectInfo(BUYITEM, (i + 1), x, (y - 13), getItemPrice(sw->si[i].itemId), getItemEffectById(sw->si[i].itemId), getItemName(sw->si[i].itemId), sw->si[i].itemId, 0, false, getItem(sw->si[i].itemId)->minlvl);

					return true;
				}
			}
		}

		for(int i = 0; i < 20; i++)
		{
			if(sw->cart[i].itemId != 0)
			{
				if(OverButton(sw->cart[i].x, sw->cart[i].y, sw->cart[i].pic->w, sw->cart[i].pic->h, x, y))
				{
					if(ow->currType == CARTITEM && ow->currItem == (i + 1) && ow->names == getItemName(sw->cart[i].itemId) && !newMan)
						return true;

					CreateObjectInfo(CARTITEM, (i + 1), x, (y - 13), getItemPrice(sw->cart[i].itemId), getItemEffectById(sw->cart[i].itemId), getItemName(sw->cart[i].itemId), sw->cart[i].itemId, 0, false, getItem(sw->cart[i].itemId)->minlvl);

					return true;
				}
			}
		}

		if(overAll)
			return false;
	}

	if(inv->enable)
	{
		bool overAll = OverButton(inv->x, inv->y, inv->inv->w, inv->inv->h, x, y);

		for(int i = 0; i < 25; i++)
		{
			if(inv->items[i] != NULL)
			{
				if(OverButton(inv->items[i]->x, inv->items[i]->y, inv->items[i]->pic->w, inv->items[i]->pic->h, x, y))
				{
					if(ow->currType == INVENTORYITEM && ow->currItem == (i + 1) && ow->names == getItemName(inv->items[i]->id) && !newMan)
						return true;

					CreateObjectInfo(INVENTORYITEM, (i + 1), x, (y - 13), inv->items[i]->value, inv->items[i]->effect, getItemName(inv->items[i]->id), inv->items[i]->id, 0, false, getItem(inv->items[i]->id)->minlvl);

					return true;
				}
			}
		}

		for(int i = 0; i < 4; i++)
		{
			if(inv->eq[i] != NULL)
			{
				if(OverButton((inv->x + getEquipPosX((i + 1))), (inv->y + getEquipPosY((i + 1))), inv->eq[i]->pic->w, inv->eq[i]->pic->h, x, y))
				{
					if(ow->currType == INVENTORYITEMEQUIP && ow->currItem == (i + 1) && ow->names == getItemName(inv->eq[i]->id) && !newMan)
						return true;

					CreateObjectInfo(INVENTORYITEMEQUIP, (i + 1), x, (y - 13), inv->eq[i]->value, inv->eq[i]->effect, getItemName(inv->eq[i]->id), inv->eq[i]->id, 0, false, getItem(inv->eq[i]->id)->minlvl);

					return true;
				}
			}
		}

		if(overAll)
			return false;
	}

	if(sk->enable)
	{
		bool overAll = OverButton(sk->x, sk->y, sk->win->w, sk->win->h, x, y);

		/*
		for(int i = 0; i < skillLoaded; i++)
		{
			if(OverButton(((sk->x + skills[i].x) - 67), ((sk->y + skills[i].y) - 44), 100, 64, x, y))
			{
				if(ow->currType == SKILLWINDOW && ow->currItem == (i + 1) && ow->names == getSkillName(skills[i].id) && !newMan)
					return true;

				CreateObjectInfo(SKILLWINDOW, (i + 1), x, (y - 13), 0, 0, getSkillName(skills[i].id), skills[i].id, (i + 1), false, 0);

				return true;
			}
		}
		*/

		for(unsigned int i = 0; i < skill.size(); i++)
		{
			if(OverButton(((sk->x + skill.at(i)->x) - 67), ((sk->y + skill.at(i)->y) - 44), 100, 64, x, y))
			{
				if(ow->currType == SKILLWINDOW && ow->currItem == (i + 1) && ow->names == getSkillName(skill.at(i)->id) && !newMan)
					return true;

				CreateObjectInfo(SKILLWINDOW, (i + 1), x, (y - 13), 0, 0, getSkillName(skill.at(i)->id), skill.at(i)->id, skill.at(i)->id, false, 0);

				return true;
			}
		}

		if(overAll)
			return false;
	}

	if(icons.size() > 0)
	{
		bool overAll = OverButton(icons.at(0)->x, icons.at(0)->y, (icons.at(0)->icon->w + ((icons.size() - 1) * icons.at(0)->icon->w)), icons.at(0)->icon->h, x, y);

		for(unsigned int i = 0; i < icons.size(); i++)
		{
			if(OverButton(icons.at(i)->x, icons.at(i)->y, icons.at(i)->icon->w, icons.at(i)->icon->h, x, y))
			{
				std::string fullString = (getBuffNameOne(icons.at(i)->effect) + " " + getBuffNameTwo(icons.at(i)->effect));

				if(ow->currType == BUFF && ow->currItem == icons.at(i)->effect && ow->names == fullString && !newMan)
					return true;

				CreateObjectInfo(BUFF, icons.at(i)->effect, x, (y - 13), icons.at(i)->lvl, icons.at(i)->changeEffect, fullString, icons.at(i)->effect, 0, false, 0);

				return true;
			}
		}

		if(overAll)
			return false;
	}

	if(msgs.size() > 0)
	{
		for(unsigned int i = 0; i < msgs.size(); i++)
		{
			if(OverButton(msgs.at(i)->x, msgs.at(i)->y, msgs.at(i)->win->w, msgs.at(i)->win->h, x, y))
				return false;
		}
	}

	if(pi.size() > 0)
	{
		for(int i = (pi.size() - 1); i >= 0; i--)
		{
			if(OverButton(pi.at(i)->x, pi.at(i)->y, pi.at(i)->win->w, pi.at(i)->win->h, x, y))
				return false;
		}
	}

	if(addstatwindow.at(0)->enable)
	{
		bool overAll = OverButton(addstatwindow.at(0)->x, addstatwindow.at(0)->y, addstatwindow.at(0)->window->w, addstatwindow.at(0)->window->h, x, y);

		if(OverButton(addstatwindow.at(0)->x + 13, addstatwindow.at(0)->y + 44, 128, 22, x, y))
		{
			if(ow->currType == WINDOW_ADDSTAT && ow->currItem == WINDOW_ADDSTAT_STR && !newMan)
				return true;

			CreateObjectInfo(WINDOW_ADDSTAT, WINDOW_ADDSTAT_STR, x, y - 13, 0, 0, "Strength", WINDOW_ADDSTAT_STR, 0, false, 0);

			return true;
		}

		if(OverButton(addstatwindow.at(0)->x + 13, addstatwindow.at(0)->y + 77, 128, 22, x, y))
		{
			if(ow->currType == WINDOW_ADDSTAT && ow->currItem == WINDOW_ADDSTAT_STA && !newMan)
				return true;

			CreateObjectInfo(WINDOW_ADDSTAT, WINDOW_ADDSTAT_STA, x, y - 13, 0, 0, "Stamina", WINDOW_ADDSTAT_STA, 0, false, 0);

			return true;
		}

		if(OverButton(addstatwindow.at(0)->x + 13, addstatwindow.at(0)->y + 107, 128, 22, x, y))
		{
			if(ow->currType == WINDOW_ADDSTAT && ow->currItem == WINDOW_ADDSTAT_AGI && !newMan)
				return true;

			CreateObjectInfo(WINDOW_ADDSTAT, WINDOW_ADDSTAT_AGI, x, y - 13, 0, 0, "Agility", WINDOW_ADDSTAT_AGI, 0, false, 0);

			return true;
		}

		if(OverButton(addstatwindow.at(0)->x + 13, addstatwindow.at(0)->y + 138, 128, 22, x, y))
		{
			if(ow->currType == WINDOW_ADDSTAT && ow->currItem == WINDOW_ADDSTAT_INT && !newMan)
				return true;

			CreateObjectInfo(WINDOW_ADDSTAT, WINDOW_ADDSTAT_INT, x, y - 13, 0, 0, "Intelligence", WINDOW_ADDSTAT_INT, 0, false, 0);

			return true;
		}

		if(OverButton(addstatwindow.at(0)->x + 13, addstatwindow.at(0)->y + 289, 180, 28, x, y))
		{
			if(ow->currType == WINDOW_ADDSTAT && ow->currItem == WINDOW_ADDSTAT_PVP && !newMan)
				return true;

			CreateObjectInfo(WINDOW_ADDSTAT, WINDOW_ADDSTAT_PVP, x, y - 13, 0, 0, "PvP Points", WINDOW_ADDSTAT_PVP, 0, false, 0);

			return true;
		}

		if(overAll)
			return false;
	}

	if(option->enable)
	{
		if(OverButton(option->x, option->y, option->win->w, option->win->h, x, y))
			return false;
	}

	if(win.size() > 0)
	{
		for(unsigned int i = 0; i < win.size(); i++)
		{
			if(OverButton(win.at(i)->x, win.at(i)->y, win.at(i)->w->w, win.at(i)->w->h, x, y))
				return false;
		}
	}

	if(partyw.at(0)->enable)
	{
		if(OverButton(partyw.at(0)->x, partyw.at(0)->y, partyw.at(0)->window->w, partyw.at(0)->window->h, x, y))
			return false;
	}

	bool hit = false;

	CheckMonsterInfo(x, y, hit, 0, NULL, NULL, true);

	if(!showProcent && !hit)
	{
		if(drops.size() > 0)
		{
			for(int i = (drops.size() - 1); i >= 0; i--)
			{
				if(drops.at(i)->inWin)
				{
					if(OverButton(drops.at(i)->cX, drops.at(i)->cY, drops.at(i)->pic->w, drops.at(i)->pic->h, x, y))
					{
						if(ow->currType == DROPITEM && ow->currItem == drops.at(i)->id && ow->names == getItemName(drops.at(i)->itemid) && !newMan)
							return true;

						CreateObjectInfo(DROPITEM, drops.at(i)->id, x, (y - 13), getItemValue(drops.at(i)->itemid), getItemEffectById(drops.at(i)->itemid), getItemName(drops.at(i)->itemid), drops.at(i)->itemid, 0, false, getItem(drops.at(i)->itemid)->minlvl);

						return true;
					}
				}
			}
		}
	}

	return false;
}
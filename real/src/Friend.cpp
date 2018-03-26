#include "Friend.h"
#include "Game.h"
#include "Main.h"
#include "Misc.h"

extern std::vector<Window*> win;
extern SDL_Surface *screen;
extern TTF_Font *courMedium;
extern SDL_Color clrFgBlack;
extern bool chatMessage;
extern bool chatting;
extern bool chatWindow;

void FriendWindow::UpdateNames()
{
	std::vector<Friend> fp;

	for(unsigned int i = 0; i < tps.size(); i++)
	{
		if(fp.size() > 0)
		{
			bool added = false;

			for(unsigned int y = 0; y < fp.size(); y++)
			{
				if(tps.at(i).status && !fp.at(y).status)
				{
					fp.insert((fp.begin() + y), tps.at(i));
					added = true;

					break;
				}
			}

			if(!added)
				fp.push_back(tps.at(i));
		}

		else
			fp.push_back(tps.at(i));
	}

	if(fp.size() != tps.size())
		Log("FriendWindow::UpdateNames() failed %d %d\n", fp.size(), tps.size());

	else
	{
		tps.clear();

		for(unsigned int p = 0; p < fp.size(); p++)
			tps.push_back(fp.at(p));
	}

	CheckUpDown();

	for(unsigned int i = 0; i < tps.size(); i++)
	{
		tps.at(i).showMenu = false;
	}
}

void FriendWindow::Clean()
{
	for(unsigned int i = 0; i < this->tps.size(); i++)
	{
		SDL_FreeSurface(this->tps.at(i).namep);
		SDL_FreeSurface(this->tps.at(i).levelp);

		for(unsigned int y = 0; y < this->tps.at(i).menu.size(); y++)
		{
			SDL_FreeSurface(this->tps.at(i).menu.at(y).tSur);
			SDL_FreeSurface(this->tps.at(i).menu.at(y).btn);
		}
	}

	this->tps.clear();

	SDL_FreeSurface(win);
	SDL_FreeSurface(quitx);
	SDL_FreeSurface(noteup);
	SDL_FreeSurface(notedown);
}

void FriendWindow::CleanNames()
{
	for(unsigned int i = 0; i < this->tps.size(); i++)
	{
		SDL_FreeSurface(this->tps.at(i).namep);
		SDL_FreeSurface(this->tps.at(i).levelp);

		for(unsigned int y = 0; y < this->tps.at(i).menu.size(); y++)
		{
			SDL_FreeSurface(this->tps.at(i).menu.at(y).tSur);
			SDL_FreeSurface(this->tps.at(i).menu.at(y).btn);
		}
	}

	this->tps.clear();
}

void FriendWindow::StartDrag(int mx, int my)
{
	dragged.type = DRAGFRIENDLIST;
	dragged.id = 0;
	dragged.x = x;
	dragged.y = y;
	dragged.pic = NULL;
	dragged.slot = mx;
	dragged.qty = my;
	dragged.winX = NULL;
	dragged.winY = NULL;
	dragged.winEnable = NULL;
	dragged.winSizeX = NULL;
	dragged.winSizeY = NULL;

	drag = true;
}

void FriendWindow::Drag(int mx, int my)
{
	if(tps.size() > 13)
	{
		int sy = (dragged.qty - my);

		startY -= sy;
		dragged.qty = my;

		if(sy > 0)
		{
			int fullThing = startY + (tps.size() * 16);

			if(fullThing < y + 273)
			{
				startY += sy;
			}
		}

		else
		{
			if(startY > (y + 65))
			{
				startY = y + 65;
			}
		}

		CheckUpDown();

		for(unsigned int i = 0; i < tps.size(); i++)
		{
			tps.at(i).showMenu = false;
		}
	}

	else
		startY = y + 65;
}

void FriendWindow::StopDrag()
{
}

void FriendWindow::CheckUpDown()
{
	if(tps.size() > 13)
	{
		int wrote = 0;
		unsigned int i = 0;
		bool firstWrite = true;
		int first = 0;
		int last = 0;

		for(i = 0; i < tps.size(); i++)
		{
			if(wrote >= 13)
			{
				break;
			}

			if((startY + ((int)i * 16)) >= (y + 65))
			{
				if(firstWrite)
					first = i;

				last = i;
				firstWrite = false;

				wrote++;
			}
		}

		if(last == (tps.size() - 1))
			goDown = false;

		else
			goDown = true;

		if(first != 0)
			goUp = true;

		else
			goUp = false;
	}
}

void PMessage::Clean()
{
	for(unsigned int i = 0; i < wrotep.size(); i++)
	{
		SDL_FreeSurface(wrotep.at(i));
	}

	SDL_FreeSurface(tonamep);

	if(textp != NULL)
		SDL_FreeSurface(textp);

	wrote.clear();
	wrotep.clear();
}

void PrivateMessage::Clean()
{
	for(unsigned int i = 0; i < this->pm.size(); i++)
	{
		this->pm.at(i).Clean();
	}

	this->pm.clear();

	SDL_FreeSurface(win);
	SDL_FreeSurface(quitx);
}

void PrivateMessage::AddWindow(const char *name)
{
	PMessage pms2 = PMessage();

	pms2.toname = name;
	pms2.id = 0;
	pms2.x = (screen->w / 2 - win->w / 2);
	pms2.y = (screen->h / 2 - win->h / 2);
	pms2.tonamep = RenderText_Solid(courMedium, name, clrFgBlack);
	pms2.startY = pms2.y + 30;

	pm.push_back(pms2);

	if(!chatting && !chatWindow)
	{
		if(!chatMessage)
		{
			marked = name;
			
			chatMessage = true;
			SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);
		}
	}
}

void PrivateMessage::RemoveWindow(const char *name)
{
	for(unsigned int i = 0; i < pm.size(); i++)
	{
		if(strcmp(pm.at(i).toname.c_str(), name) == 0)
		{
			if(drag && dragged.id == i && dragged.type == DRAGPMSWINDOW)
				drag = false;

			if(marked == pm.at(i).toname)
			{
				marked = "";
				chatMessage = false;
			}

			pm.at(i).Clean();
			pm.erase(pm.begin() + i);

			break;
		}
	}
}

void ChatManager::CleanChat()
{
	for(unsigned int i = 0; i < tps.size(); i++)
	{
		SDL_FreeSurface(tps.at(i).message);
	}

	tps.clear();

	goUp = false;
	goDown = false;
}

void ChatManager::AddMessage(Chat c)
{
	tps.push_back(c);

	if(drag && dragged.type == DRAGCHATLIST)
	{
	}

	else
		SetLast();
}

void ChatManager::StartDrag(int mx, int my)
{
	dragged.type = DRAGCHATLIST;
	dragged.id = 0;
	dragged.x = win.at(1)->x;
	dragged.y = win.at(1)->y;
	dragged.pic = NULL;
	dragged.slot = mx;
	dragged.qty = my;
	dragged.winX = NULL;
	dragged.winY = NULL;
	dragged.winEnable = NULL;
	dragged.winSizeX = NULL;
	dragged.winSizeY = NULL;

	drag = true;
}

void ChatManager::Drag(int mx, int my)
{
	if(tps.size() > 5)
	{
		int sy = (dragged.qty - my);

		startY -= sy;
		dragged.qty = my;

		if(sy > 0)
		{
			int fullThing = startY + (tps.size() * 16);

			if(fullThing < win.at(1)->y + 85)
			{
				startY += sy;
			}
		}

		else
		{
			if(startY > (win.at(1)->y + 5))
			{
				startY = (win.at(1)->y + 5);
			}
		}

		CheckUpDown();
	}

	else
		startY = win.at(1)->y + 5;
}

void ChatManager::SetLast()
{
	if(tps.size() > 5)
	{
		int last = win.at(1)->y + 5;
		int size = ((tps.size() - 5) * 16);

		last -= size;

		startY = last;

		CheckUpDown();
	}

	else
		startY = win.at(1)->y + 5;
}

void ChatManager::CheckUpDown()
{
	if(tps.size() > 5)
	{
		int wrote = 0;
		unsigned int i = 0;
		bool firstWrite = true;
		int first = 0;
		int last = 0;

		for(i = 0; i < tps.size(); i++)
		{
			if(wrote >= 5)
			{
				break;
			}

			if((startY + ((int)i * 16)) >= (win.at(1)->y + 5))
			{
				if(firstWrite)
					first = i;

				last = i;
				firstWrite = false;

				wrote++;
			}
		}

		if(last == (tps.size() - 1))
			goDown = false;

		else
			goDown = true;

		if(first != 0)
			goUp = true;

		else
			goUp = false;
	}
}

void PrivateMessage::AddMessage(const char *name, const char *fullmsg)
{
	for(unsigned int i = 0; i < pm.size(); i++)
	{
		if(strcmp(pm.at(i).toname.c_str(), name) == 0)
		{
			SDL_Surface *newmsg = RenderText_Solid(courMedium, fullmsg, clrFgBlack);

			pm.at(i).wrote.push_back(fullmsg);
			pm.at(i).wrotep.push_back(newmsg);
		}
	}
}

void PMessage::StartDrag(int mx, int my)
{
	dragged.type = DRAGPMSLIST;
	dragged.name = toname;
	dragged.x = x;
	dragged.y = y;
	dragged.pic = NULL;
	dragged.slot = mx;
	dragged.qty = my;
	dragged.winX = NULL;
	dragged.winY = NULL;
	dragged.winEnable = NULL;
	dragged.winSizeX = NULL;
	dragged.winSizeY = NULL;

	drag = true;
}

void PMessage::Drag(int mx, int my)
{
	if(wrote.size() > 10)
	{
		int sy = (dragged.qty - my);

		startY -= sy;
		dragged.qty = my;

		if(sy > 0)
		{
			int fullThing = startY + (wrote.size() * 14);

			if(fullThing < y + 170)
			{
				startY += sy;
			}
		}

		else
		{
			if(startY > (y + 30))
			{
				startY = (y + 30);
			}
		}

		CheckUpDown();
	}

	else
		startY = y + 30;
}

void PMessage::SetLast()
{
	if(wrote.size() > 10)
	{
		int last = y + 30;
		int size = ((wrote.size() - 10) * 14);

		last -= size;

		startY = last;

		CheckUpDown();
	}

	else
		startY = y + 30;
}

void PMessage::CheckUpDown()
{
	if(wrote.size() > 10)
	{
		int wrote2 = 0;
		unsigned int i = 0;
		bool firstWrite = true;
		int first = 0;
		int last = 0;

		for(i = 0; i < wrote.size(); i++)
		{
			if(wrote2 >= 10)
			{
				break;
			}

			if((startY + ((int)i * 14)) >= (y + 30))
			{
				if(firstWrite)
					first = i;

				last = i;
				firstWrite = false;

				wrote2++;
			}
		}

		if(last == (wrote.size() - 1))
			goDown = false;

		else
			goDown = true;

		if(first != 0)
			goUp = true;

		else
			goUp = false;
	}
}

PMessage *PrivateMessage::GetMarked()
{
	PMessage *pm2 = NULL;

	if(pm.size() > 0)
		pm2 = &pm.at(0);

	for(unsigned int i = 0; i < pm.size(); i++)
	{
		if(pm.at(i).toname == marked)
		{
			pm2 = &pm.at(i);
			break;
		}
	}

	return pm2;
}
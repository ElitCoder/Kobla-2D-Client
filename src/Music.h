#ifndef MUSIC_H
#define MUSIC_H

#include "Misc.h"

#define LOGINSCREEN 1
#define CHARSELECT 2
#define REMOVECHAR 3
#define ADDCHAR 4
#define CREATECHARACTER 5
#define DELETECHARACTER 6
#define NEWSTACK 7
#define REMOVEITEM 8
#define ADDITEM 9
#define MOVEITEM 10
#define NEWITEM 11

extern bool noMusic;

class MusicManager
{
public:
	MusicManager()
	{
		this->cm = NULL;
		this->cn = "";
		this->types = 0;
		this->loops = 0;
	}

	~MusicManager()
	{
		if(cm != NULL)
		{
			Mix_HaltMusic();
			Mix_FreeMusic(this->cm);
		}
	}

	void playMusic(char *fname, int type, int loop)
	{
		/*
		if(Mix_PlayingMusic() == 1)
			this->stopMusic();

		if(type == 1)
			this->cm = Mix_LoadMUS(fname);

		else
			printf("Unknown type at playMusic().\n");

		this->cn = fname;
		this->types = type;
		this->loops = loop;

		if(!noMusic)
		{
			Mix_PlayMusic(this->cm, loop);
			Mix_HookMusicFinished(StopMusic);
		}
		*/
	}

	void playCurrent()
	{
		/*
		if(strcmp(this->cn.c_str(), "") != 0)
			this->playMusic((char*)cn.c_str(), this->types, this->loops);

		else
			this->playMusic("TestSong.ogg", 1, -1);
			*/
	}

	void stopMusic()
	{
		Mix_HaltMusic();
		Mix_FreeMusic(this->cm);

		this->cm = NULL;
		this->cn = "";
		this->types = 0;
		this->loops = 0;
	}

	Mix_Music *cm;
	std::string cn;
	int types;
	int loops;
};

void UpdateLogin();
void SmallClean();

#endif
#pragma once

#include "cTexture.h"
#include "Globals.h"

//Image array size
#define NUM_IMG		10

//Image identifiers
#define IMG_TILESET		0
#define IMG_PLAYER		1
#define IMG_DOG			2
#define IMG_OCTOPUS		3
#define IMG_HEART		4
#define IMG_SWORD		5
#define IMG_ISAAC		6

#define IMG_MENU		7
#define IMG_ABOUT		8
#define IMG_CONTROLS	9

class cData
{
public:
	cData(void);
	~cData(void);

	int  GetID(int img);
	void GetSize(int img,int *w,int *h);
	bool LoadImage(int img,char *filename,int type = GL_RGBA);

private:
	cTexture texture[NUM_IMG];
};

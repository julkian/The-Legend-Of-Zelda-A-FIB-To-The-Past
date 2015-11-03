#pragma once

#include "cBicho.h"

#define PLAYER_START_CX		3
#define PLAYER_START_CY		2

#define STEP_LENGTH			0

class cDog: public cBicho
{
public:
	cDog();
	~cDog();

	void Draw(int tex_id);
	void Move(int *map, int playerX, int playerY);
};

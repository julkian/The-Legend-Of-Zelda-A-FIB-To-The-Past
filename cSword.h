#pragma once

#include "cBicho.h"

#define PLAYER_START_CX		3
#define PLAYER_START_CY		2

#define SWORD_DELAY			1
#define SWORD_DAMAGE		1

#define STEP_LENGTH			4

class cSword: public cBicho
{
public:
	cSword();
	~cSword();

	void Draw(int tex_id);
	void Move(int *map);
	void NextFrame(int max);
};

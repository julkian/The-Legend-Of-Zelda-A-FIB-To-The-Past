#pragma once

#include "cBicho.h"

#define PLAYER_START_CX			5
#define PLAYER_START_CY			5

#define CHANGE_DIRECTION_CHANCE	2

#define STEP_LENGTH				1

//#define 

class cOctopus: public cBicho
{
public:
	cOctopus();
	~cOctopus();

	void Draw(int tex_id);
	void Move(int *map);
	int newDirection(int oldState);
};

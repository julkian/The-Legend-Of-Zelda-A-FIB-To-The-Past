#pragma once

#include "cBicho.h"
#include "cBall.h"

#define PLAYER_START_CX			5
#define PLAYER_START_CY			5

#define OCTOPUS_MAX_HEALTH		1
#define OCTOPUS_DAMAGE			0.5

#define CHANGE_DIRECTION_CHANCE	2

#define STEP_LENGTH				1

//#define 

class cOctopus: public cBicho
{
public:
	cOctopus();
	~cOctopus();

	void Draw(int tex_id);
	void Move(int *map, int playerX, int playerY);
	int newDirection(int oldState);
	cBall* getBall();
	bool hasBall();
	void setHasBall(bool isBall);

private:
	cBall Ball;
	bool isBall;
};

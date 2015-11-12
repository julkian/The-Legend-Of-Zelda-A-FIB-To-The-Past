#pragma once

#include "cBicho.h"

#define PLAYER_START_CX		3
#define PLAYER_START_CY		2

#define BALL_DELAY			1
#define BALL_DAMAGE			0.5

#define STEP_LENGTH			2

class cBall: public cBicho
{
public:
	cBall();
	~cBall();

	void Draw(int tex_id);
	void Move(int *map);
	void NextFrame(int max);
};

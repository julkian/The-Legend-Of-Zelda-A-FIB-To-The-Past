#pragma once

#include "cBicho.h"

#define PLAYER_START_CX		3
#define PLAYER_START_CY		2

#define TEAR_DELAY			1
#define TEAR_DAMAGE			3

#define STEP_LENGTH			2

class cTear: public cBicho
{
public:
	cTear();
	~cTear();

	void Draw(int tex_id);
	void Move(int *map);
	void NextFrame(int max);
	void setInfo(int objectiveX, int objectiveY, int isaacX, int isaacY);

private:
	int objectiveX, objectiveY;
	int isaacX, isaacY;
	int proportionX, proportionY;
	float proportion;
};

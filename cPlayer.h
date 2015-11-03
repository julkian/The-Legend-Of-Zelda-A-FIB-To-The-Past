#pragma once

#include "cBicho.h"

#define PLAYER_START_CX		3
#define PLAYER_START_CY		2

#define STEP_LENGTH			2

class cPlayer: public cBicho
{
public:
	cPlayer();
	~cPlayer();

	void Draw(int tex_id);
	void DrawRect(int tex_id,float xo,float yo,float xf,float yf);
};

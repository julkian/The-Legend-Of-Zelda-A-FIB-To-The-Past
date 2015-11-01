#pragma once

#include "cBicho.h"

#define PLAYER_START_CX		3
#define PLAYER_START_CY		2

class Wizard: public cBicho
{
public:
	Wizard();
	~Wizard();

	void Draw(int tex_id);
};
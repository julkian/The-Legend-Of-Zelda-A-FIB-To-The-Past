#pragma once

#include "cBicho.h"
#include "cTear.h"

#define PLAYER_START_CX			5
#define PLAYER_START_CY			5

#define ISAAC_DELAY		2

#define ATTACK_DELAY	30

#define ISAAC_MAX_HEALTH		10.0
#define ISAAC_DAMAGE			1

//#define 

class cIsaac: public cBicho
{
public:
	cIsaac();
	~cIsaac();

	void Draw(int tex_id);
	std::vector<cTear>* getAllTears();
	void NextFrame(int max);
	void Attack(int playerX, int playerY);
	cTear* getTear(int i);
	void deleteTear(int i);

private:
	std::vector<cTear> allTears;
	int attackDelay;
};

#pragma once

#include "cBicho.h"

#define PLAYER_START_CX				3
#define PLAYER_START_CY				2

#define PLAYER_MAX_DELAY_INVENCIBLE	48

#define ATTACK_DELAY				20

#define STEP_LENGTH					2

class cPlayer: public cBicho
{
public:
	cPlayer();
	~cPlayer();

	void	Draw(int tex_id);
	void	DrawRect(int tex_id,float xo,float yo,float xf,float yf);
	void	Attack();
	void	setInvencibility(bool invencible);
	bool	isInvencible();

private:
	bool	invencible;
	int		delayInvencible;
};

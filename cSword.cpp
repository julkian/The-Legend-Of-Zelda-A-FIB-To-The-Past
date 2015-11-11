
#include "cSword.h"

cSword::cSword() {
	setStepLength(STEP_LENGTH);
	this->damage = SWORD_DAMAGE;
}
cSword::~cSword(){}

void cSword::NextFrame(int max)
{
	delay++;
	if(delay == SWORD_DELAY)
	{
		seq++;
		seq%=max;
		delay = 0;
	}
}

void cSword::Draw(int tex_id)
{	
	float xo,yo,xf,yf;

	switch(GetState())
	{
		//1..3
		case STATE_WALKLEFT:	xo = 0.25f;
								yo = 0.25f + (GetFrame()*0.25f);
								NextFrame(4);
								break;

		//4..6
		case STATE_WALKRIGHT:	xo = 0.75f;
								yo = 0.25f + (GetFrame()*0.25f);
								NextFrame(4);
								break;
			
		//1..3
		case STATE_WALKUP:		xo = 0.0f;
								yo = 0.25f + (GetFrame()*0.25f);
								NextFrame(4);
								break;
		//4..6
		case STATE_WALKDOWN:	xo = 0.5f;
								yo = 0.25f + (GetFrame()*0.25f);
								NextFrame(4);
								break;

		default:				xo = yo = xf = yf = 0.0;
								break;
	}
	xf = xo + 0.25f;
	yf = yo + 0.25f;

	DrawRect(tex_id,xo,yo,xf,yf);
}

void cSword::Move(int *map)
{
	int state = GetState();

	if (state == STATE_WALKLEFT)			MoveLeft(map);
	else if (state == STATE_WALKRIGHT)	MoveRight(map);
	else if (state == STATE_WALKDOWN)	MoveDown(map);
	else if (state == STATE_WALKUP)		MoveUp(map);
}


#include "cBall.h"

cBall::cBall() {
	setStepLength(STEP_LENGTH);
	this->damage = BALL_DAMAGE;
}
cBall::~cBall(){}

void cBall::NextFrame(int max)
{
	delay++;
	if(delay == BALL_DELAY)
	{
		seq++;
		seq%=max;
		delay = 0;
	}
}

void cBall::Draw(int tex_id)
{	
	float xo,xf;
	float yo = 1.0;
	float yf = 0.0;

	float width = 1/14.0;

	xo = 8 * width + (GetFrame()*width);
	NextFrame(6);
	xf = xo + width;

	DrawRect(tex_id,xo,yo,xf,yf);
}

void cBall::Move(int *map)
{
	int state = GetState();

	if (state == STATE_WALKLEFT)		MoveLeft(map);
	else if (state == STATE_WALKRIGHT)	MoveRight(map);
	else if (state == STATE_WALKDOWN)	MoveDown(map);
	else if (state == STATE_WALKUP)		MoveUp(map);
}


#include "cDog.h"

cDog::cDog() {
	setStepLength(STEP_LENGTH);
}
cDog::~cDog(){}

void cDog::Draw(int tex_id)
{	
	float xo,yo,xf,yf;

	yo = 1.0f;
	switch(GetState())
	{
		//1
		case STATE_LOOKLEFT:	xo = 0.25f;
								break;
		//4
		case STATE_LOOKRIGHT:	xo = 0.75f;
								break;
		//1
		case STATE_LOOKUP:		xo = 0.5f;
								break;
		//4
		case STATE_LOOKDOWN:	xo = 0.0f;
								break;

		//1..3
		case STATE_WALKLEFT:	xo = 0.25f + (GetFrame()*0.125f);
								NextFrame(2);
								break;
		//4..6
		case STATE_WALKRIGHT:	xo = 0.75f + (GetFrame()*0.125f);;
								NextFrame(2);
								break;
		//1..3
		case STATE_WALKUP:	xo = 0.5f + (GetFrame()*0.125f);
								NextFrame(2);
								break;
		//4..6
		case STATE_WALKDOWN:	xo = 0.0f + (GetFrame()*0.125f);;
								NextFrame(2);
								break;
	}
	xf = xo + 0.125f;
	yf = 0.0f;

	DrawRect(tex_id,xo,yo,xf,yf);
}

void cDog::Move(int *map, int playerX, int playerY)
{
	// TODO bfs to go to player

	//WORKAROUND
	int dogX = this->GetPositionX();
	int dogY = this->GetPositionY();
	if (dogX > playerX) {
		this->MoveLeft(map);
	} else if (dogX < playerX) {
		this->MoveRight(map);
	} else if (dogY > playerY) {
		this->MoveDown(map);
	} else {
		this->MoveUp(map);
	}
}

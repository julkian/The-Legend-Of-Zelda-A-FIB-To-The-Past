
#include "cPlayer.h"

cPlayer::cPlayer() {}
cPlayer::~cPlayer(){}

void cPlayer::Draw(int tex_id)
{	
	float xo,yo,xf,yf;

	int state = GetState();

	switch(state)
	{
		//1
		case STATE_LOOKLEFT:	xo = 0.2f; yo = 1/3.0;
								break;
		//4
		case STATE_LOOKRIGHT:	xo = 0.6f; yo = 1/3.0;
								break;
		//1
		case STATE_LOOKUP:		xo = 0.4f; yo = 1/3.0;
								break;
		//4
		case STATE_LOOKDOWN:	xo = 0.0f; yo = 1/3.0;
								break;

		//1..3
		case STATE_WALKLEFT:	xo = 0.2f + (GetFrame()*0.1f); yo = 1/3.0;
								NextFrame(2);
								break;
		//4..6
		case STATE_WALKRIGHT:	xo = 0.6f + (GetFrame()*0.1f); yo = 1/3.0;
								NextFrame(2);
								break;
		//1..3
		case STATE_WALKUP:		xo = 0.4f + (GetFrame()*0.1f); yo = 1/3.0;
								NextFrame(2);
								break;
		//4..6
		case STATE_WALKDOWN:	xo = 0.0f + (GetFrame()*0.1f); yo = 1/3.0;
								NextFrame(2);
								break;

		case STATE_ATTACKLEFT:	xo = 0.2f; yo = 2/3.0;
		case STATE_ATTACKRIGHT:	xo = 0.2f; yo = 1.0;
		case STATE_ATTACKUP:	xo = 0.1f; yo = 1.0;
		case STATE_ATTACKDOWN:	xo = 0.0f; yo = 1.0;
	}
	xf = xo + 0.1f;
	yf = 0.0f;
	if (state == STATE_ATTACKLEFT) {
		xf = xo + 0.2f;
		yf -= 1/3.0;
	} else if (state == STATE_ATTACKRIGHT) {
		xf = xo + 0.2f;
		yf -= 1/3.0;
	} else if (state == STATE_ATTACKUP) {
		xf = xo + 0.1f;
		yf -= 2/3.0;
	} else if (state == STATE_ATTACKDOWN) {
		xf = xo + 0.2f;
		yf -= 2/3.0;
	}

	DrawRect(tex_id,xo,yo,xf,yf);
}

void cBicho::Attack()
{
	int state = GetState();
	if (state == STATE_LOOKLEFT || state == STATE_WALKLEFT) {
		SetState(STATE_ATTACKLEFT);
	} else if (state == STATE_LOOKRIGHT || state == STATE_WALKRIGHT) {
		SetState(STATE_ATTACKRIGHT);
	} else if (state == STATE_LOOKDOWN || state == STATE_WALKDOWN) {
		SetState(STATE_ATTACKDOWN);
	} else if (state == STATE_LOOKUP || state == STATE_WALKUP) {
		SetState(STATE_ATTACKUP);
	}
}

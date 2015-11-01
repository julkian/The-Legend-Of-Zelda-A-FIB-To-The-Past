
#include "Wizard.h"

Wizard::Wizard() {}
Wizard::~Wizard(){}

void Wizard::Draw(int tex_id)
{	
	float xo,yo,xf,yf;

	yo = 1.0f;
	switch(GetState())
	{
		//1
		case STATE_LOOKLEFT:	xo = 0.2f;
								break;
		//4
		case STATE_LOOKRIGHT:	xo = 0.6f;
								break;
		//1
		case STATE_LOOKUP:		xo = 0.4f;
								break;
		//4
		case STATE_LOOKDOWN:	xo = 0.0f;
								break;

		//1..3
		case STATE_WALKLEFT:	xo = 0.2f + (GetFrame()*0.1f);
								NextFrame(2);
								break;
		//4..6
		case STATE_WALKRIGHT:	xo = 0.6f + (GetFrame()*0.1f);;
								NextFrame(2);
								break;
		//1..3
		case STATE_WALKUP:	xo = 0.4f + (GetFrame()*0.1f);
								NextFrame(2);
								break;
		//4..6
		case STATE_WALKDOWN:	xo = 0.0f + (GetFrame()*0.1f);;
								NextFrame(2);
								break;
	}
	xf = xo + 0.1f;
	yf = 0.0f;

	DrawRect(tex_id,xo,yo,xf,yf);
}

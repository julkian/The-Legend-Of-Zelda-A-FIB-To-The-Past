#include "cTear.h"

cTear::cTear() {
	setStepLength(STEP_LENGTH);
	this->damage = TEAR_DAMAGE;
}
cTear::~cTear(){}

void cTear::NextFrame(int max)
{
	delay++;
	if(delay == TEAR_DELAY)
	{
		seq++;
		seq%=max;
		delay = 0;
	}
}

void cTear::Draw(int tex_id)
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

void cTear::setInfo(int objectiveX, int objectiveY, int isaacX, int isaacY)
{
	proportionX = objectiveX - isaacX;
	proportionY = objectiveY - isaacY;
	if (proportionY != 0) {
		proportion = proportionX/(proportionY*1.0);
	} else proportion = proportionX;
	if (proportion < 0) proportion *= -1;
}

void cTear::Move(int *map)
{
	float stepLength = getStepLength();
	if (proportion > 0) {
		//x has more distance
		proportionX >= 0 ? x += (stepLength*proportion) : x -= (stepLength*proportion); // objective left : objective right
		proportionY >= 0 ? y += stepLength : y -= stepLength; // objective above : objective below
	} else {
		//y has more distance
		proportionX >= 0 ? x += stepLength : x -= stepLength; // objective left : objective right
		proportionY >= 0 ? y += (stepLength*proportion) : y -= (stepLength*proportion); // objective above : objective below
	}
}

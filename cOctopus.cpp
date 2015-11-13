#include "cOctopus.h"

cOctopus::cOctopus() {
	int randDirection = rand() % 100;
	if (randDirection < 25)			SetState(STATE_LOOKLEFT);
	else if (randDirection < 50)	SetState(STATE_LOOKUP);
	else if (randDirection < 75)	SetState(STATE_LOOKDOWN);
	else							SetState(STATE_LOOKRIGHT);
	setStepLength(STEP_LENGTH);
	this->damage = OCTOPUS_DAMAGE;
	this->actualHealth = OCTOPUS_MAX_HEALTH;
	isBall = false;
	this->invincible = false;
	this->beingPushed = false;
}
cOctopus::~cOctopus(){}

void cOctopus::Draw(int tex_id)
{	
	float xo,yo,xf,yf;

	yo = 1.0f;
	int state = GetState();
	float distanceBetweenSprites = 1/14.0;

	if (state == STATE_LOOKLEFT || state == STATE_WALKLEFT)			xo = 2*distanceBetweenSprites + (GetFrame()*distanceBetweenSprites);
	else if (state == STATE_LOOKRIGHT || state == STATE_WALKRIGHT)	xo = 6*distanceBetweenSprites + (GetFrame()*distanceBetweenSprites);
	else if (state == STATE_LOOKDOWN || state == STATE_WALKDOWN)	xo = GetFrame()*distanceBetweenSprites;
	else if (state == STATE_LOOKUP || state == STATE_WALKUP)		xo = 4*distanceBetweenSprites + (GetFrame()*distanceBetweenSprites);

	NextFrame(2);
	
	xf = xo + distanceBetweenSprites;
	yf = 0.0f;

	bool haveToBeDrawn = manageInvincibility();

	if (haveToBeDrawn) DrawRect(tex_id,xo,yo,xf,yf);
}

void cOctopus::Move(int *map, int playerX, int playerY)
{
	int state = GetState();

	if (rand() % 100 <= CHANGE_DIRECTION_CHANCE) state = newDirection(state);
	SetState(state);

	bool shoot = false;
	int ballState = 0;

	if (state == STATE_LOOKLEFT || state == STATE_WALKLEFT)			{
		if (!isBall && playerX < x && ((playerY >= y && playerY <= y+h) || (playerY+h >= y && playerY+h <= y+h)) ) {
			shoot = true;
			ballState = STATE_WALKLEFT;
		} else {
			MoveLeft(map);
		}
	}
	else if (state == STATE_LOOKRIGHT || state == STATE_WALKRIGHT)	{
		if (!isBall && playerX > x && ((playerY >= y && playerY <= y+h) || (playerY+h >= y && playerY+h <= y+h)) ) {
			shoot = true;
			ballState = STATE_WALKRIGHT;
		} else {
			MoveRight(map);
		}
	}
	else if (state == STATE_LOOKDOWN || state == STATE_WALKDOWN)	{
		if (!isBall && playerY < y && ((playerX >= x && playerX <= x+w) || (playerX+w >= x && playerX+w <= x+w))) {
			shoot = true;
			ballState = STATE_WALKDOWN;
		} else {
			MoveDown(map);
		}
	}
	else if (state == STATE_LOOKUP || state == STATE_WALKUP)		{
		if (!isBall && playerY > y && ((playerX >= x && playerX <= x+w) || (playerX+w >= x && playerX+w <= x+w))) {
			shoot = true;
			ballState = STATE_WALKUP;
		} else {
			MoveUp(map);
		}
	}

	int rnd = rand() % 10 + 1;     // v2 in the range 1 to 100
	if (shoot && rnd > 9) {
		isBall = true;
		Ball.SetWidthHeight(16,16);
		Ball.SetPosition(x,y);
		Ball.SetWidthHeight(16,16);
		Ball.SetState(ballState);
	}
}

cBall* cOctopus::getBall()
{
	return &Ball;
}

void cOctopus::setHasBall(bool isBall) 
{
	this->isBall = isBall;
}

bool cOctopus::hasBall()
{
	return this->isBall;
}

int cOctopus::newDirection(int oldState){
	int newState = oldState;
	while (newState == oldState) {
		int randDirection = rand() % 100;
		if (randDirection < 25)			newState = STATE_WALKLEFT;
		else if (randDirection < 50)	newState = STATE_WALKUP;
		else if (randDirection < 75)	newState = STATE_WALKDOWN;
		else							newState = STATE_WALKRIGHT;
	}
	return newState;
}

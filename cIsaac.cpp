#include "cIsaac.h"

cIsaac::cIsaac() {
	this->damage = ISAAC_DAMAGE;
	this->actualHealth = ISAAC_MAX_HEALTH;
	this->w = 32;
	this->h = 32;
	this->attackDelay = 0;
	this->invincible = false;
	this->beingPushed = false;
	this->canAttack = false;
}
cIsaac::~cIsaac(){}

void cIsaac::NextFrame(int max)
{
	delay++;
	if(delay == ISAAC_DELAY)
	{
		seq++;
		seq%=max;
		delay = 0;
	}
}

void cIsaac::Draw(int tex_id)
{	
	float xo,yo,xf,yf;

	yo = 1.0f;
	xo = 0.0f + GetFrame()*0.5f;
	NextFrame(2);
	yf = 0.0f;
	xf = xo + 0.5f;

	DrawRect(tex_id,xo,yo,xf,yf);
}

void cIsaac::Attack(int playerX, int playerY)
{
	if (attackDelay == ATTACK_DELAY && this->canAttack) {
		cTear Tear;
		Tear.SetWidthHeight(16,16);
		Tear.SetPosition(x,y);
		Tear.SetWidthHeight(16,16);
		Tear.setInfo(playerX, playerY, x, y);
		allTears.push_back(Tear);
		attackDelay = 0;
	} else attackDelay++;
	
}

std::vector<cTear>* cIsaac::getAllTears()
{
	return &allTears;
}

cTear* cIsaac::getTear(int i)
{
	return &allTears[i];
}

void cIsaac::deleteTear(int i) 
{
	allTears.erase(allTears.begin()+i);
}

void cIsaac::setCanAttack(bool _canAttack)
{
	this->canAttack = _canAttack;
}
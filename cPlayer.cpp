
#include "cPlayer.h"
#include "cScene.h"

cPlayer::cPlayer() {
	setStepLength(STEP_LENGTH);
	this->damage = PLAYER_DAMAGE;
	this->actualHealth = PLAYER_MAX_HEALTH;
}
cPlayer::~cPlayer(){}

void cPlayer::Draw(int tex_id)
{	
	float xo,yo,xf,yf;

	int state = GetState();

	switch(state)
	{
		case STATE_LOOKLEFT:	xo = 0.2f; yo = 1/3.0;
								break;

		case STATE_LOOKRIGHT:	xo = 0.6f; yo = 1/3.0;
								break;

		case STATE_LOOKUP:		xo = 0.4f; yo = 1/3.0;
								break;

		case STATE_LOOKDOWN:	xo = 0.0f; yo = 1/3.0;
								break;

		case STATE_WALKLEFT:	xo = 0.2f + (GetFrame()*0.1f); yo = 1/3.0;
								NextFrame(2);
								break;

		case STATE_WALKRIGHT:	xo = 0.6f + (GetFrame()*0.1f); yo = 1/3.0;
								NextFrame(2);
								break;

		case STATE_WALKUP:		xo = 0.4f + (GetFrame()*0.1f); yo = 1/3.0;
								NextFrame(2);
								break;

		case STATE_WALKDOWN:	xo = 0.0f + (GetFrame()*0.1f); yo = 1/3.0;
								NextFrame(2);
								break;

		case STATE_ATTACKLEFT:	xo = 0.2f; yo = 2/3.0;
			NextFrame(1);
								break;

		case STATE_ATTACKRIGHT:	xo = 0.2f; yo = 1.0;
			NextFrame(1);
								break;

		case STATE_ATTACKUP:	xo = 0.1f; yo = 1.0;
			NextFrame(1);
								break;

		case STATE_ATTACKDOWN:	xo = 0.0f; yo = 1.0;
			NextFrame(1);
								break;
	}
	
	xf = xo + 0.1f;
	yf = 0.0f;
	
	if (state == STATE_ATTACKLEFT) {
		xf = xo + 0.2f;
		yf = yo - 1/3.0;
	} else if (state == STATE_ATTACKRIGHT) {
		xf = xo + 0.2f;
		yf = yo - 1/3.0;
	} else if (state == STATE_ATTACKUP) {
		xf = xo + 0.1f;
		yf = yo - 2/3.0;
	} else if (state == STATE_ATTACKDOWN) {
		xf = xo + 0.1f;
		yf = yo - 2/3.0;
	}

	if (!this->invincible) DrawRect(tex_id,xo,yo,xf,yf);
	else if (this->delayInvincible <= PLAYER_MAX_DELAY_INVENCIBLE && delayInvincible % 8 < 4)
	{
		DrawRect(tex_id,xo,yo,xf,yf);
		++this->delayInvincible;
	} else if (this->delayInvincible <= PLAYER_MAX_DELAY_INVENCIBLE && delayInvincible % 8 >= 4)
	{
		++this->delayInvincible;
	}
	else this->invincible = false;
}

void cPlayer::Attack()
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

	seq = 0;
	delay = 0;
}

void cPlayer::DrawRect(int tex_id,float xo,float yo,float xf,float yf)
{
	int screen_x,screen_y, width, height;

	GetPosition(&screen_x, &screen_y);
	GetWidthHeight(&width, &height);

	screen_x += SCENE_Xo;
	screen_y += SCENE_Yo + (BLOCK_SIZE - TILE_SIZE);

	int state = GetState();
	float offsetX = 0;
	float offsetY = 0;
	if (state == STATE_ATTACKLEFT || state == STATE_ATTACKRIGHT) {
		if (state == STATE_ATTACKLEFT) offsetX = width;
		width *= 2;
	} else if (state == STATE_ATTACKUP || state == STATE_ATTACKDOWN) {
		if (state == STATE_ATTACKDOWN) offsetY = height;
		height *= 2;
	}

	glEnable(GL_TEXTURE_2D);
	
	glBindTexture(GL_TEXTURE_2D,tex_id);
	glBegin(GL_QUADS);	
		glTexCoord2f(xo,yo);	glVertex2i(screen_x - offsetX			,screen_y - offsetY);
		glTexCoord2f(xf,yo);	glVertex2i(screen_x + width - offsetX	,screen_y - offsetY);
		glTexCoord2f(xf,yf);	glVertex2i(screen_x + width - offsetX	,screen_y + height - offsetY);
		glTexCoord2f(xo,yf);	glVertex2i(screen_x - offsetX			,screen_y + height - offsetY);
	glEnd();

	glDisable(GL_TEXTURE_2D);
}

void cPlayer::setInvincibility(bool invincible)
{
	this->invincible = invincible;
	if (invincible) this->delayInvincible = 0;
}

bool cPlayer::isInvincible() 
{
	return this->invincible;
}

bool cPlayer::isFullHealth()
{
	return (this->actualHealth == PLAYER_MAX_HEALTH);
}

float cPlayer::getMaxHealth()
{
	return PLAYER_MAX_HEALTH;
}
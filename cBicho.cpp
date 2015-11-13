#include "cBicho.h"
#include "cScene.h"
#include "Globals.h"

cBicho::cBicho(void)
{
	seq=0;
	delay=0;
}
cBicho::~cBicho(void){}

cBicho::cBicho(int posx,int posy,int width,int height)
{
	x = posx;
	y = posy;
	w = width;
	h = height;
	this->actualHealth = BICHO_HEALTH;
	this->damage = BICHO_DAMAGE;
}
void cBicho::SetPosition(int posx,int posy)
{
	x = posx;
	y = posy;
}
void cBicho::GetPosition(int *posx,int *posy)
{
	*posx = x;
	*posy = y;
}
int cBicho::GetPositionX()
{
	return x;
}
int cBicho::GetPositionY()
{
	return y;
}
void cBicho::SetTile(int tx,int ty)
{
	x = tx * TILE_SIZE;
	y = ty * TILE_SIZE;
}
void cBicho::GetTile(int *tx,int *ty)
{
	*tx = x / TILE_SIZE;
	*ty = y / TILE_SIZE;
}
void cBicho::SetWidthHeight(int width,int height)
{
	w = width;
	h = height;
}
void cBicho::GetWidthHeight(int *width,int *height)
{
	*width = w;
	*height = h;
}
bool cBicho::Collides(cRect *rc)
{
	return ((x>rc->left) && (x+w<rc->right) && (y>rc->bottom) && (y+h<rc->top));
}
bool cBicho::CollidesMapWall(int *map,bool right)
{
	int tile_x,tile_y;
	int j;
	int width_tiles,height_tiles;

	tile_x = x / TILE_SIZE;
	tile_y = y / TILE_SIZE;
	width_tiles  = w / TILE_SIZE;
	height_tiles = h / TILE_SIZE;

	if( (y % TILE_SIZE) != 0) height_tiles++;

	if(right)	tile_x += width_tiles;
	else tile_x = (x-stepLength)/TILE_SIZE;
	
	for(j=0;j<height_tiles;j++)
	{
		int tileId = map[ tile_x + ((tile_y+j)*SCENE_WIDTH) ];
		if(this->isTileWall(tileId))		{
			return true; // overworld floor, overworld dungeon door, dungeon floor
		}
	}
	
	return false;
}

bool cBicho::CollidesMapFloor(int *map, bool down)
{
	int tile_x,tile_y;
	int width_tiles;
	bool on_base;
	int i;

	tile_x = x / TILE_SIZE;
	tile_y = y / TILE_SIZE;

	width_tiles = w / TILE_SIZE;
	if( (x % TILE_SIZE) != 0) width_tiles++;

	on_base = false;
	i=0;
	while((i<width_tiles) && !on_base)
	{
		if (down) {
			if( (y % TILE_SIZE) == 0 )
			{
				if(this->isTileWall(map[ (tile_x + i) + ((tile_y - 1) * SCENE_WIDTH) ]))
					on_base = true;
			}
			else
			{
				if(this->isTileWall(map[ (tile_x + i) + (tile_y * SCENE_WIDTH) ]))
				{
					y = (tile_y + 1) * TILE_SIZE;
					on_base = true;
				}
			}
		} else {
			if( (y % TILE_SIZE) == 0 )
			{
				if(this->isTileWall(map[ (tile_x + i) + ((tile_y + 1) * SCENE_WIDTH) ]))
					on_base = true;
			}
			else
			{
				if(this->isTileWall(map[ (tile_x + i) + (tile_y * SCENE_WIDTH) ]))
				{
					y = (tile_y - 1) * TILE_SIZE;
					on_base = true;
				}
			}		
		}
		i++;
	}
	return on_base;
}

void cBicho::GetArea(cRect *rc)
{
	rc->left   = x;
	rc->right  = x+w;
	rc->bottom = y;
	rc->top    = y+h;
}
void cBicho::DrawRect(int tex_id,float xo,float yo,float xf,float yf)
{
	int screen_x,screen_y;

	screen_x = x + SCENE_Xo;
	screen_y = y + SCENE_Yo + (BLOCK_SIZE - TILE_SIZE);

	glEnable(GL_TEXTURE_2D);
	
	glBindTexture(GL_TEXTURE_2D,tex_id);
	glBegin(GL_QUADS);	
		glTexCoord2f(xo,yo);	glVertex2i(screen_x  ,screen_y);
		glTexCoord2f(xf,yo);	glVertex2i(screen_x+w,screen_y);
		glTexCoord2f(xf,yf);	glVertex2i(screen_x+w,screen_y+h);
		glTexCoord2f(xo,yf);	glVertex2i(screen_x  ,screen_y+h);
	glEnd();

	glDisable(GL_TEXTURE_2D);
}

void cBicho::MoveLeft(int *map)
{	
	if(CollidesMapWall(map,false))
	{
		state = STATE_LOOKLEFT;
	} else {
		//Whats next tile?
		if( (x % TILE_SIZE) == 0)
		{
			x -= getStepLength();
		}
		//Advance, no problem
		else
		{
			x -= getStepLength();
			if(state != STATE_WALKLEFT)
			{
				state = STATE_WALKLEFT;
				seq = 0;
				delay = 0;
			}
		}
	}
}
void cBicho::MoveRight(int *map)
{
	int xaux;

	if(CollidesMapWall(map,true))
	{
		state = STATE_LOOKRIGHT;
	} else {
		//Whats next tile?
		if( (x % TILE_SIZE) == 0)
		{
			xaux = x;
			x += getStepLength();	
		}
		//Advance, no problem
		else
		{
			x += getStepLength();

			if(state != STATE_WALKRIGHT)
			{
				state = STATE_WALKRIGHT;
				seq = 0;
				delay = 0;
			}
		}
	}
}
void cBicho::MoveUp(int *map)
{
	int yaux;

	if(CollidesMapFloor(map,false))
	{
		state = STATE_LOOKUP;
	} else {
		//Whats next tile?
		if( (y % TILE_SIZE) == 0)
		{
			yaux = y;
			y += getStepLength();
		}
		//Advance, no problem
		else
		{
			y += getStepLength();

			if(state != STATE_WALKUP)
			{
				state = STATE_WALKUP;
				seq = 0;
				delay = 0;
			}
		}
	}
}
void cBicho::MoveDown(int *map)
{
	int yaux;

	if(CollidesMapFloor(map,true))
	{
		state = STATE_LOOKDOWN;
	} else {
		//Whats next tile?
		if( (y % TILE_SIZE) == 0)
		{
			yaux = y;
			y -= getStepLength();
		}
		//Advance, no problem
		else
		{
			y -= getStepLength();

			if(state != STATE_WALKDOWN)
			{
				state = STATE_WALKDOWN;
				seq = 0;
				delay = 0;
			}
		}
	}
}


void cBicho::Stop()
{
	switch(state)
	{
		case STATE_WALKLEFT:	
		//case STATE_ATTACKLEFT:
			state = STATE_LOOKLEFT;		
			break;
		case STATE_WALKRIGHT:
		//case STATE_ATTACKRIGHT:
			state = STATE_LOOKRIGHT;	
			break;
		case STATE_WALKUP:
		//case STATE_ATTACKUP:
			state = STATE_LOOKUP;		
			break;
		case STATE_WALKDOWN:
		//case STATE_ATTACKDOWN:
			state = STATE_LOOKDOWN;		
			break;
	}
}
void cBicho::Logic(int *map)
{
}
void cBicho::NextFrame(int max)
{
	delay++;
	if (delay == ATTACK_FRAME_DELAY && isAttacking()) {
		delay = 0;
		switch(state) {
			case STATE_ATTACKLEFT:
				state = STATE_LOOKLEFT;		
				break;
			case STATE_ATTACKRIGHT:
				state = STATE_LOOKRIGHT;	
				break;
			case STATE_ATTACKUP:
				state = STATE_LOOKUP;		
				break;
			case STATE_ATTACKDOWN:
				state = STATE_LOOKDOWN;		
				break;
		}
	} else if(delay == FRAME_DELAY && !(state == STATE_ATTACKLEFT || state == STATE_ATTACKRIGHT || state == STATE_ATTACKUP || state == STATE_ATTACKDOWN))
	{
		seq++;
		seq%=max;
		delay = 0;
	}
}
int cBicho::GetFrame()
{
	return seq;
}
int cBicho::GetState()
{
	return state;
}
void cBicho::SetState(int s)
{
	state = s;
}

float cBicho::getActualHealth()
{
	return this->actualHealth;
}

void cBicho::takeDamage(float damage, char *pushSide)
{
	this->invincible = true;
	this->delayInvincible = 0;
	this->beingPushed = true;
	this->pushOffset = 0;
	this->actualHealth -= damage;
	this->pushSide = *pushSide;
}

void cBicho::setActualHealth(float health)
{
	this->actualHealth = health;
}

bool cBicho::isDead()
{
	if (this->actualHealth <= 0) return true;
	return false;
}

float cBicho::getDamage()
{
	return this->damage;
}

void cBicho::setDamage(float damage)
{
	this->damage = damage;
}

void cBicho::setStepLength(float stepLength) {
	this->stepLength = stepLength;
}

float cBicho::getStepLength() {
	return this->stepLength;
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

	delay = 0;
}

bool cBicho::isAttacking()
{
	if (state == STATE_ATTACKLEFT || state == STATE_ATTACKRIGHT || state == STATE_ATTACKUP || state == STATE_ATTACKDOWN) return true;

	return false;
}

bool cBicho::manageInvincibility()
{
	bool haveToBeDrawn = false;

	if (!this->invincible) haveToBeDrawn = true;
	else if (this->delayInvincible <= BICHO_MAX_DELAY_INVENCIBLE && delayInvincible % 8 < 4)
	{
		++this->delayInvincible;
		haveToBeDrawn = true;
	} else if (this->delayInvincible <= BICHO_MAX_DELAY_INVENCIBLE && delayInvincible % 8 >= 4) ++this->delayInvincible;
	else this->invincible = false;

	return haveToBeDrawn;
}

bool cBicho::isInvincible() 
{
	return this->invincible;
}

bool cBicho::isTileWall(int tileId)
{
	if(tileId != 0 && tileId != 119)		{
		return true; // overworld floor, dungeon floor
	}
	return false;
}

bool cBicho::isBeingPushed() 
{
	return this->beingPushed;
}

void cBicho::pushMove(int *map)
{
	int movement = BICHO_PUSH_DAMAGE_OFFSET / BICHO_PUSH_DAMAGE_DELAY;
	switch (this->pushSide)
	{
		case 'u':
				for (int i = movement; i > 0; i -= 2) 
				{
					this->y += i;
					if (CollidesMapFloor(map, false))
					{	
						this->y -= i;
						this->pushOffset = BICHO_PUSH_DAMAGE_OFFSET;
					}  else i = -1;
				}
			break;
		case 'd':
				for (int i = movement; i > 0; i -= 2) 
				{
					this->y -= i;
					if (CollidesMapFloor(map, true))
					{	
						this->y += i;
						this->pushOffset = BICHO_PUSH_DAMAGE_OFFSET;
					} else i = -1;
				}
			break;
		case 'l':
				for (int i = movement; i > 0; i -= 2) 
				{
					this->x -= i;
					if (CollidesMapWall(map, false))
					{
						this->x += i;
						this->pushOffset = BICHO_PUSH_DAMAGE_OFFSET;
					} else i = -1;
				}
			break;
		case 'r':
				for (int i = movement; i > 0; i -= 2) 
				{
					this->x += i;
					if (CollidesMapWall(map, true))
					{
						this->x -= i;
						this->pushOffset = BICHO_PUSH_DAMAGE_OFFSET;
					} else i = -1;
				}
			break;
	}
	this->pushOffset += movement;
	if (this->pushOffset >= BICHO_PUSH_DAMAGE_OFFSET)
	{
		this->beingPushed = false;
		this->pushOffset = 0;
	}
}
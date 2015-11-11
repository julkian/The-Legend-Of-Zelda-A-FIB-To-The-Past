#pragma once

#include "cTexture.h"
#include "Globals.h"

#define FRAME_DELAY					8
#define ATTACK_FRAME_DELAY			16

#define STATE_LOOKLEFT				0
#define STATE_LOOKRIGHT				1
#define STATE_LOOKUP				2
#define STATE_LOOKDOWN				3

#define STATE_WALKLEFT				4
#define STATE_WALKRIGHT				5
#define STATE_WALKUP				6
#define STATE_WALKDOWN				7

#define STATE_ATTACKLEFT			8
#define STATE_ATTACKRIGHT			9
#define	STATE_ATTACKUP				10
#define	STATE_ATTACKDOWN			11

#define BICHO_MAX_DELAY_INVENCIBLE	48

#define BICHO_HEALTH				2.0
#define BICHO_DAMAGE				0.5
#define BICHO_PUSH_DAMAGE_DELAY		20		//frames delay when is taking damage
#define BICHO_PUSH_DAMAGE_OFFSET	24

#define PUSH_LEFT					'l'
#define PUSH_UP						'u'
#define PUSH_DOWN					'd'
#define PUSH_RIGHT					'r'


class cRect
{
public:
	int left,top,
		right,bottom;
};

class cBicho
{
public:
	cBicho(void);
	cBicho(int x,int y,int w,int h);
	~cBicho(void);

	void SetPosition(int x,int y);
	void GetPosition(int *x,int *y);
	int GetPositionX();
	int GetPositionY();
	void SetTile(int tx,int ty);
	void GetTile(int *tx,int *ty);
	void SetWidthHeight(int w,int h);
	void GetWidthHeight(int *w,int *h);

	bool Collides(cRect *rc);
	bool CollidesMapWall(int *map,bool right);
	bool CollidesMapFloor(int *map, bool down);
	void GetArea(cRect *rc);
	void DrawRect(int tex_id,float xo,float yo,float xf,float yf);

	void MoveRight(int *map);
	void MoveLeft(int *map);
	void MoveUp(int *map);
	void MoveDown(int *map);
	void Attack();
	void Stop();
	void Logic(int *map);

	int  GetState();
	void SetState(int s);

	void NextFrame(int max);
	int  GetFrame();

	float getActualHealth();
	void setActualHealth(float actualHealth);

	void takeDamage(float damage, char *pushSide);
	bool isDead();

	float getDamage();
	void setDamage(float damage);

	float getStepLength();
	void setStepLength(float sl);

	bool isAttacking();

	bool isInvincible();

	bool manageInvincibility();

	virtual bool isTileWall(int tileId);
	
protected:
	int x,y;
	int w,h;
	int state;
	
	float actualHealth;
	float damage;

	float stepLength;

	int seq,delay;
	
	bool	invincible;
	int		delayInvincible;

	bool	isBeingPushed;
	int		pushOffset;
	char	pushSide;
};

#pragma once

#include "cTexture.h"
#include <iostream>
#include <fstream>
#include <string>
//#include "Tmx.h"

#define SCENE_Xo		0
#define SCENE_Yo		0
#define SCENE_WIDTH		32
#define SCENE_HEIGHT	33

#define TILE_SIZE	16
#define BLOCK_SIZE	16

/*definiendo variables*/

#define TILE_ROWS		12	//numero de cuadraicos por fila en el tileset
#define TILE_COLUMN		14	//numero de cuadraicos por columna en el tileset
#define TILE_ROWS_F		12.0f	//numero de cuadraicos por fila en el tileset
#define TILE_COLUMN_F	14.0f	//numero de cuadraicos por columna en el tileset

class cScene
{
public:
	cScene(void);
	virtual ~cScene(void);

	bool LoadLevel(int level);
	void Draw(int tex_id);
	int *GetMap();
	void blockBossRoom();

private:
	int map[SCENE_WIDTH * SCENE_HEIGHT];	//scene
	int id_DL;								//actual level display list
	bool isBossRoomBlocked;
};

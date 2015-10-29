#pragma once

#include "cTexture.h"
#include <iostream>
#include <fstream>
#include <string>
//#include "Tmx.h"

#define SCENE_Xo		(2*TILE_SIZE)
#define SCENE_Yo		TILE_SIZE
#define SCENE_WIDTH		20
#define SCENE_HEIGHT	20

#define FILENAME		"demo"
#define FILENAME_EXT	".csv"

//#define TILE_SIZE		16
//#define BLOCK_SIZE	24

#define TILE_SIZE	16
#define BLOCK_SIZE	16

/*definiendo variables*/
#define TILE_ROWS		13	//numero de cuadraicos por fila en el tileset
#define TILE_COLUMNS	16	//numero de cuadraicos por columna en el tileset

class cScene
{
public:
	cScene(void);
	virtual ~cScene(void);

	bool LoadLevel(int level);
	void Draw(int tex_id);
	int *GetMap();
//	Tmx::Map *loadMap();


private:
	int map[SCENE_WIDTH * SCENE_HEIGHT];	//scene
	int id_DL;								//actual level display list
};

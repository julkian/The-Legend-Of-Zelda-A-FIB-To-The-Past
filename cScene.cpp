#include "cScene.h"
#include "Globals.h"


cScene::cScene(void)
{
}

cScene::~cScene(void)
{
}

/*
Tmx::Map *loadMap() 
{
	Tmx::Map *map = new Tmx::Map();
	std::string fileName = "./resources/levels/demo.tmx";
    map->ParseFile(fileName);
	return map;
}
*/

bool cScene::LoadLevel(int level)
{
//	Tmx::Map *_map = loadMap();

	bool res;
	//FILE *fd;
	//char file[16];
	int i,j,px,py;
	char tile;
	float coordx_tile, coordy_tile;

	res=true;

	//construyendo nombre de archivo
	//if(level<10) sprintf(file,"%s0%d%s",(char *)FILENAME,level,(char *)FILENAME_EXT);
	//else		 sprintf(file,"%s%d%s",(char *)FILENAME,level,(char *)FILENAME_EXT);

//	fd=fopen(file,"r");
//	if(fd==NULL) return false;

	/*La asquerosidad hecha codigo*/
	std::ifstream fd("level01.csv");

	//archivo con num capas

	//std::ifstream fd(std::string(FILENAME)+ std::string(FILENAME_EXT));
	std::string str;

	//int numLayers;
	//fd >> numLayers;

	id_DL=glGenLists(1);
	glNewList(id_DL,GL_COMPILE);
		glBegin(GL_QUADS);
		//for (int h = 0; h < numLayers; ++h) {
			for(j=SCENE_HEIGHT-1;j>=0;j--)
			{
				px=SCENE_Xo;
				py=SCENE_Yo+(j*TILE_SIZE);

				for(i=0;i<SCENE_WIDTH;i++)
				{
					//map[(j*SCENE_WIDTH)+i]=_map[i][j];

					std::getline(fd, str, ',');
					map[(j*SCENE_WIDTH)+i] = std::stoi(str);

					coordx_tile = ((map[(j*SCENE_WIDTH)+i] % 10)) * (1/10.0f);
					coordy_tile = (map[(j*SCENE_WIDTH)+i] / 10) * (1/10.0f);

					//BLOCK_SIZE = 24, FILE_SIZE = 64
					// 24 / 64 = 0.375
					glTexCoord2f(coordx_tile       ,coordy_tile+(1/10.0f));		glVertex2i(px           ,py           );
					glTexCoord2f(coordx_tile+(1/10.0f),coordy_tile+(1/10.0f));	glVertex2i(px+BLOCK_SIZE,py           );
					glTexCoord2f(coordx_tile+(1/10.0f),coordy_tile       );		glVertex2i(px+BLOCK_SIZE,py+BLOCK_SIZE);
					glTexCoord2f(coordx_tile       ,coordy_tile       );		glVertex2i(px           ,py+BLOCK_SIZE);

					px+=TILE_SIZE;
				}
			}

		glEnd();
	glEndList();

	return res;
}

void cScene::Draw(int tex_id)
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,tex_id);
	glCallList(id_DL);
	glDisable(GL_TEXTURE_2D);
}
int* cScene::GetMap()
{
	return map;
}
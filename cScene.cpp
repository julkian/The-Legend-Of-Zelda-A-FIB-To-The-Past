#include "cScene.h"
#include "Globals.h"


cScene::cScene(void)
{
}

cScene::~cScene(void)
{
}

bool cScene::LoadLevel(int level)
{
	bool res;
	int i,j,px,py;
	char tile;
	float coordx_tile, coordy_tile;

	res=true;

	/*La asquerosidad hecha codigo*/
	std::ifstream fd("level01.csv");

	std::string str;

	id_DL=glGenLists(1);
	glNewList(id_DL,GL_COMPILE);
		glBegin(GL_QUADS);
			for(j=SCENE_HEIGHT-1;j>=0;j--)
			{
				px=SCENE_Xo;
				py=SCENE_Yo+(j*TILE_SIZE);

				for(i=0;i<SCENE_WIDTH;i++)
				{
					std::getline(fd, str, ',');
					map[(j*SCENE_WIDTH)+i] = std::stoi(str);

					int mapContent = map[(j*SCENE_WIDTH)+i];
					if (isBossRoomBlocked) {
						if (j == 21 && (i == 23 || i == 24)) {
							map[(j*SCENE_WIDTH)+i] = 146;
							mapContent = 146;
						}
					}
					coordx_tile = (mapContent % TILE_COLUMN) * (1/TILE_COLUMN_F);
					coordy_tile = (mapContent / TILE_COLUMN) * (1/TILE_ROWS_F);

					glTexCoord2f(coordx_tile					,coordy_tile+(1/TILE_ROWS_F));	glVertex2i(px           ,py           );
					glTexCoord2f(coordx_tile+(1/TILE_COLUMN_F)	,coordy_tile+(1/TILE_ROWS_F));	glVertex2i(px+BLOCK_SIZE,py           );
					glTexCoord2f(coordx_tile+(1/TILE_COLUMN_F)	,coordy_tile       );			glVertex2i(px+BLOCK_SIZE,py+BLOCK_SIZE);
					glTexCoord2f(coordx_tile					,coordy_tile       );			glVertex2i(px           ,py+BLOCK_SIZE);

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

void cScene::blockBossRoom()
{
	isBossRoomBlocked = true;
	this->LoadLevel(1);
}
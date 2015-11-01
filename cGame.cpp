#include "cGame.h"
#include "Globals.h"


cGame::cGame(void)
{
}

cGame::~cGame(void)
{
}

bool cGame::Init()
{
	bool res=true;

	//Graphics initialization
	glClearColor(0.0f,0.0f,0.0f,0.0f);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0,GAME_WIDTH/2,0,GAME_HEIGHT/2,0,1);
	glMatrixMode(GL_MODELVIEW);
	glAlphaFunc(GL_GREATER, 0.05f);
	glEnable(GL_ALPHA_TEST);

	//Scene initialization
	res = Data.LoadImage(IMG_BLOCKS,"resources/level/overworld.png",GL_RGB);
	if(!res) return false;
	res = Scene.LoadLevel(1);
	if(!res) return false;

	//Player initialization
	res = Data.LoadImage(IMG_PLAYER,"resources/charset/link.png",GL_RGBA);
	if(!res) return false;
	Player.SetWidthHeight(16,16);
	Player.SetTile(0,5);
	Player.SetWidthHeight(16,16);
	Player.SetState(STATE_LOOKRIGHT);

	//Enemies initialization
	/*
	res = Data.LoadImage(IMG_PLAYER,"resources/charset/enemyWizard.png",GL_RGBA);
	if(!res) return false;
	//Player.SetWidthHeight(16,16);
	//Player.SetTile(0,5);
	//Player.SetWidthHeight(16,16);
	//Player.SetState(STATE_LOOKRIGHT);

	res = Data.LoadImage(IMG_PLAYER,"resources/charset/enemyOctopus.png",GL_RGBA);
	if(!res) return false;
	//Player.SetWidthHeight(16,16);
	//Player.SetTile(0,5);
	//Player.SetWidthHeight(16,16);
	//Player.SetState(STATE_LOOKRIGHT);

	res = Data.LoadImage(IMG_PLAYER,"resources/charset/enemyDog.png",GL_RGBA);
	if(!res) return false;
	//Player.SetWidthHeight(16,16);
	//Player.SetTile(0,5);
	//Player.SetWidthHeight(16,16);
	//Player.SetState(STATE_LOOKRIGHT);
	*/
	return res;
}

bool cGame::Loop()
{
	bool res=true;

	res = Process();
	if(res) Render();

	return res;
}

void cGame::Finalize()
{
}

//Input
void cGame::ReadKeyboard(unsigned char key, int x, int y, bool press)
{
	keys[key] = press;
}

void cGame::ReadMouse(int button, int state, int x, int y)
{
}

//Process
bool cGame::Process()
{
	bool res=true;
	
	//Process Input
	if(keys[27])	res=false;	
	
	//Game Logic
	if(keys[GLUT_KEY_UP])			Player.MoveUp(Scene.GetMap());
	else if(keys[GLUT_KEY_DOWN])	Player.MoveDown(Scene.GetMap());
	else if(keys[GLUT_KEY_LEFT])	Player.MoveLeft(Scene.GetMap());
	else if(keys[GLUT_KEY_RIGHT])	Player.MoveRight(Scene.GetMap());
	else Player.Stop();

	return res;
}

//Output
void cGame::Render()
{
	glClear(GL_COLOR_BUFFER_BIT);
	
	glLoadIdentity();

	Scene.Draw(Data.GetID(IMG_BLOCKS));
	Player.Draw(Data.GetID(IMG_PLAYER));

	glutSwapBuffers();
}

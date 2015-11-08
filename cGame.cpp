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
	glOrtho(0,GAME_WIDTH/2.5,0,GAME_HEIGHT/2.5,0,1);
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
	Player.SetTile(10,5);
	Player.SetWidthHeight(16,16);
	Player.SetState(STATE_LOOKRIGHT);

	//Enemies initialization
	
	//Octopus
	cOctopus Octopus;
	res = Data.LoadImage(IMG_OCTOPUS,"resources/charset/enemyOctopus.png",GL_RGBA);
	if(!res) return false;
	Octopus.SetWidthHeight(16,16);
	Octopus.SetTile(5,5);
	Octopus.SetWidthHeight(16,16);	
	bichos[0] = Octopus;

	//Dog
	cDog Dog;
	res = Data.LoadImage(IMG_DOG,"resources/charset/enemyDog.png",GL_RGBA);
	if(!res) return false;
	Dog.SetWidthHeight(16,16);
	Dog.SetTile(3,4);
	Dog.SetWidthHeight(16,16);
	Dog.SetState(STATE_LOOKRIGHT);	
	bichos[1] = Dog;

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
	if		(keys['z'] && !Player.isAttacking())	Player.Attack();

	else if (keys[GLUT_KEY_UP])						Player.MoveUp(Scene.GetMap());
	else if (keys[GLUT_KEY_DOWN])					Player.MoveDown(Scene.GetMap());
	else if (keys[GLUT_KEY_LEFT])					Player.MoveLeft(Scene.GetMap());
	else if (keys[GLUT_KEY_RIGHT])					Player.MoveRight(Scene.GetMap());
	
	else Player.Stop();

	Dog.Move(Scene.GetMap(), Player.GetPositionX(), Player.GetPositionY());
	Octopus.Move(Scene.GetMap());

	return res;
}

//Output
void cGame::Render()
{
	glClear(GL_COLOR_BUFFER_BIT);
	
	glLoadIdentity();

	Scene.Draw(Data.GetID(IMG_BLOCKS));
	Player.Draw(Data.GetID(IMG_PLAYER));
	Dog.Draw(Data.GetID(IMG_DOG));
	Octopus.Draw(Data.GetID(IMG_OCTOPUS));

	glutSwapBuffers();
}

void cGame::drawBichos() 
{
	for (int i = 0; i < bichos.size(); ++i) {

	}
}
#include "cGame.h"
#include "Globals.h"
#include "windows.h"

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
	currentLevelX = 0;
	currentLevelY = 0;
	glOrtho(currentLevelX,currentLevelX+LEVEL_WIDTH,currentLevelY,currentLevelY + LEVEL_HEIGHT + MENU_MARGIN,0,1);
	glMatrixMode(GL_MODELVIEW);
	glAlphaFunc(GL_GREATER, 0.05f);
	glEnable(GL_ALPHA_TEST);

	//Scene initialization
	res = Data.LoadImage(IMG_TILESET,"resources/level/tileset.png",GL_RGB);
	if(!res) return false;
	res = Scene.LoadLevel(1);
	if(!res) return false;

	//Heart initialization
	res = Data.LoadImage(IMG_HEART,"resources/charset/heart.png",GL_RGBA);
	if(!res) return false;

	//Player initialization

	res = Data.LoadImage(IMG_PLAYER,"resources/charset/link.png",GL_RGBA);
	if(!res) return false;
	Player.SetWidthHeight(16,16);
	Player.SetTile(10,5);
	Player.SetWidthHeight(16,16);
	Player.SetState(STATE_LOOKRIGHT);

	//Sword initialization

	res = Data.LoadImage(IMG_SWORD,"resources/charset/sword.png",GL_RGBA);
	if(!res) return false;
	/*
	cSword Sword;
	Sword.SetWidthHeight(16,16);
	Sword.SetTile(3,3);
	Sword.SetWidthHeight(16,16);
	Sword.SetState(STATE_WALKRIGHT);
	allSwords.push_back(Sword); */

	//Enemies initialization
	
	//Octopus
	cOctopus Octopus;
	res = Data.LoadImage(IMG_OCTOPUS,"resources/charset/enemyOctopus.png",GL_RGBA);
	if(!res) return false;
	Octopus.SetWidthHeight(16,16);
	Octopus.SetTile(5,5);
	Octopus.SetWidthHeight(16,16);
	//allOctopus.push_back(Octopus);

	//Dog
	cDog Dog;
	res = Data.LoadImage(IMG_DOG,"resources/charset/enemyDog.png",GL_RGBA);
	if(!res) return false;
	Dog.SetWidthHeight(16,16);
	Dog.SetTile(5,5);
	Dog.SetWidthHeight(16,16);
	Dog.SetState(STATE_LOOKRIGHT);
	allDogs.push_back(Dog);

	//Init music
	if (!music.openFromFile("music/overworld.ogg")) {
		//error
	}
	//music.play();

	return res;
}

bool cGame::Loop()
{
	bool res=true;
	Sleep(30);
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
	if(keys['z'] && !Player.isAttacking())	Player.Attack(allSwords);
	else if(keys[GLUT_KEY_UP])				Player.MoveUp(Scene.GetMap());
	else if(keys[GLUT_KEY_DOWN])			Player.MoveDown(Scene.GetMap());
	else if(keys[GLUT_KEY_LEFT])			Player.MoveLeft(Scene.GetMap());
	else if(keys[GLUT_KEY_RIGHT])			Player.MoveRight(Scene.GetMap());
	
	else Player.Stop();

	for (int i = 0; i < allSwords.size(); ++i) allSwords[i].Move(Scene.GetMap());

	//for (int i = 0; i < allDogs.size(); ++i) allDogs[i].Move(Scene.GetMap(), Player.GetPositionX(), Player.GetPositionY());
	//for (int i = 0; i < allOctopus.size(); ++i) allOctopus[i].Move(Scene.GetMap());
	//for (int i = 0; i < allWizards.size(); ++i) allWizards[i].Move(Scene.GetMap());

	if (!Player.isInvincible()) DetectCollisionsPlayer();

	ChangeLevel();

	return res;
}

bool cGame::DetectCollisionsPlayer() 
{
	bool playerDamaged = false;
	char pushSide;

	for (int i = 0; !playerDamaged && i < allDogs.size(); ++i) 
	{
			playerDamaged = collisionBichoPlayer(&allDogs[i], &pushSide);
			if (playerDamaged) Player.takeDamage(allDogs[i].getDamage(), &pushSide);
	}

	for (int i = 0; !playerDamaged && i < allOctopus.size(); ++i) 
	{
		playerDamaged = collisionBichoPlayer(&allOctopus[i], &pushSide);
		if (playerDamaged) Player.takeDamage(allOctopus[i].getDamage(), &pushSide);
	}

	/*
	for (int i = 0; !playerDamaged && i < allWizards.size(); ++i) 
	{
		playerDamaged = collisionBichoPlayer(&allWizards[i], &pushSide););
		if (playerDamaged) Player.takeDamage(allWizards[i].getDamage(), pushSide););
	}
	*/

	return playerDamaged;
}

bool cGame::collisionBichoPlayer(cBicho *bicho, char * pushSide)
{
	int playerX, playerY, playerW, playerH;
	int bichoX, bichoY, bichoW, bichoH;

	Player.GetPosition(&playerX, &playerY);
	Player.GetWidthHeight(&playerW, &playerH);

	bicho->GetPosition(&bichoX, &bichoY);
	bicho->GetWidthHeight(&bichoW, &bichoH);

	int playerSteplength = Player.getStepLength();

	if (playerY == bichoY+bichoH && ((playerX <= bichoX && playerX+playerW >= bichoX) || (playerX >= bichoX && playerX+playerW >= bichoX)))
	{
		int auxPlayerY = playerY - playerSteplength;
		if (auxPlayerY <= bichoY+bichoH && ((playerX <= bichoX && playerX+playerW >= bichoX) || (playerX <= bichoX+bichoW && playerX+playerW >= bichoX)))
		{
			//collision up
			*pushSide = 'u';
			return true;
		}
	} else if (playerY+playerH == bichoY && ((playerX <= bichoX && playerX+playerW >= bichoX) || (playerX <= bichoX+bichoW && playerX+playerW >= bichoX)))
	{
		int auxPlayerY = playerY + playerSteplength;
		if (auxPlayerY + playerH >= bichoY && ((playerX <= bichoX && playerX+playerW >= bichoX) || (playerX <= bichoX+bichoW && playerX+playerW >= bichoX)))
		{
			//collision down
			*pushSide = 'd';
			return true;
		}
	} else if (playerX+playerW == bichoX && ((playerY <= bichoY && playerY+playerH >= bichoY) || (playerY <= bichoY+bichoH && playerY+playerH >= bichoY+bichoH)))
	{
		int auxPlayerX = playerX - playerSteplength;
		if (auxPlayerX+playerW <= bichoX && ((playerY <= bichoY && playerY+playerH >= bichoY) || (playerY <= bichoY+bichoH && playerY+playerH >= bichoY+bichoH)))
		{
			//collision left
			*pushSide = 'l';
			return true;
		}
	} else if (playerX == bichoX+bichoW && ((playerY <= bichoY && playerY+playerH >= bichoY) || (playerY <= bichoY+bichoH && playerY+playerH >= bichoY+bichoH)))
	{
		int auxPlayerX = playerX + playerSteplength;
		if (playerX >= bichoX+bichoW && ((playerY <= bichoY && playerY+playerH >= bichoY) || (playerY <= bichoY+bichoH && playerY+playerH >= bichoY+bichoH)))
		{
			//collision right
			*pushSide = 'r';
			return true;
		}
	}
	return false;
}

void cGame::ChangeLevel()
{
	int tileX;
	int tileY;
	Player.GetTile(&tileX, &tileY);
	int state = Player.GetState();
	if (tileX == 7 || tileX == 8) {
		if (tileY == 11 && state == 6) { // 1 -> 2
			currentLevelY = LEVEL_HEIGHT;
		} else if (tileY == 12 && state == 7) { // 2 -> 1
			currentLevelY = 0;
		} else if (tileY == 21 && state == 6) { // 2 -> 3
			currentLevelY = 2*LEVEL_HEIGHT;
			// TODO update Link position
		} else if (tileY == 23 && state == 7) { // 3 -> 2
			currentLevelY = LEVEL_HEIGHT;
			// TODO update Link position
		}
	} else if (tileY == 27 || tileY == 28) {
		if (tileX == 16 && state == 5) { // 3 -> 4
			currentLevelX = LEVEL_WIDTH;
			// TODO update Link position
		} else if (tileX == 17 && state == 4) { // 4 -> 3
			currentLevelX = 0;
			// TODO update Link position
		}
	} else if (tileX == 24 || tileX == 25) { //
		if (tileY == 22 && state == 6) { // 5 -> 4
			currentLevelY = 2*LEVEL_HEIGHT;
			// TODO update Link position
		} else if (tileY == 23 && state == 7) { // 4 -> 5
			currentLevelY = LEVEL_HEIGHT;
			// TODO update Link position
		}
	}
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(currentLevelX,currentLevelX+LEVEL_WIDTH,currentLevelY,currentLevelY + LEVEL_HEIGHT + MENU_MARGIN,0,1);
	glMatrixMode(GL_MODELVIEW);
}

//Output
void cGame::Render()
{
	glClear(GL_COLOR_BUFFER_BIT);
	
	glLoadIdentity();

	Scene.Draw(Data.GetID(IMG_TILESET));

	for (int i = 0; i < allDogs.size(); ++i) allDogs[i].Draw(Data.GetID(IMG_DOG));
	//for (int i = 0; i < allOctopus.size(); ++i) allOctopus[i].Draw(Data.GetID(IMG_OCTOPUS));
	//for (int i = 0; i < allWizards.size(); ++i) allWizards[i].Draw(Data.GetID(IMG_WIZARD));

	if (!Player.isDead()) Player.Draw(Data.GetID(IMG_PLAYER));
	for (int i = 0; i < allSwords.size(); ++i) allSwords[i].Draw(Data.GetID(IMG_SWORD));

	DrawMenu();

	glutSwapBuffers();
}

void cGame::DrawMenu()
{
	//Menu panel
	glColor3f(0.0f,0.0f,0.0f);
	glRectf(currentLevelX, currentLevelY+LEVEL_HEIGHT+MENU_MARGIN, currentLevelX+LEVEL_WIDTH, currentLevelY+LEVEL_HEIGHT);

	glColor3f(1.0,1.0,1.0); 

	//Draw Hearts

	float playerHealth = Player.getActualHealth();
	
	float xo = 0.0;
	float xf;
	float yo = 1.0;
	float yf = 0.0;

	glEnable(GL_TEXTURE_2D);
	
	glBindTexture(GL_TEXTURE_2D,Data.GetID(IMG_HEART));

	for (int i=0; i<3; ++i) {
		if (playerHealth >= 1.0) {
			//pintar sencer
			xo = 0.0;
		} else if (playerHealth >= 0.5) {
			//pintar mig
			xo = 1/3.0;
		} else {
			//pintar buit
			xo = 2/3.0;
		}
		xf = xo + 1/3.0;
		glBegin(GL_QUADS);	
			glTexCoord2f(xo,yo);	glVertex2i(currentLevelX + LEVEL_WIDTH/2 + (16*(i-1)), currentLevelY + LEVEL_HEIGHT + 7);
			glTexCoord2f(xf,yo);	glVertex2i(currentLevelX + LEVEL_WIDTH/2 + (16*i)	, currentLevelY + LEVEL_HEIGHT + 7);
			glTexCoord2f(xf,yf);	glVertex2i(currentLevelX + LEVEL_WIDTH/2 + (16*i)	, currentLevelY + LEVEL_HEIGHT + 7 + 16);
			glTexCoord2f(xo,yf);	glVertex2i(currentLevelX + LEVEL_WIDTH/2 + (16*(i-1)), currentLevelY + LEVEL_HEIGHT + 7 + 16);
		glEnd();
		--playerHealth;
	}
	glDisable(GL_TEXTURE_2D);
}

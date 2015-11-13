#include "cGame.h"
#include "Globals.h"
#include "windows.h"

cGame::cGame(void)
{
	level = MENU;
	//level = 1;
	levelKind = LEVEL_OVERWORLD;
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

	//Menu initialization
	res = Data.LoadImage(IMG_MENU,"resources/menu/menu.png",GL_RGBA);
	if(!res) return false;

	//Control initialization
	res = Data.LoadImage(IMG_CONTROLS,"resources/menu/controls.png",GL_RGBA);
	if(!res) return false;

	//About initialization
	res = Data.LoadImage(IMG_ABOUT,"resources/menu/about.png",GL_RGBA);
	if(!res) return false;

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

	//Enemies initialization
	
	//Octopus
	cOctopus Octopus;
	res = Data.LoadImage(IMG_OCTOPUS,"resources/charset/enemyOctopus.png",GL_RGBA);
	if(!res) return false;
	Octopus.SetWidthHeight(16,16);
	Octopus.SetTile(5,5);
	Octopus.SetWidthHeight(16,16);
	allOctopus.push_back(Octopus);

	//Dog
	cDog Dog;
	res = Data.LoadImage(IMG_DOG,"resources/charset/enemyDog.png",GL_RGBA);
	if(!res) return false;
	Dog.SetWidthHeight(16,16);
	Dog.SetTile(4,4);
	Dog.SetWidthHeight(16,16);
	Dog.SetState(STATE_LOOKRIGHT);
	allDogs.push_back(Dog);

	//Isaac
	res = Data.LoadImage(IMG_ISAAC,"resources/charset/isaac.png",GL_RGBA);
	if(!res) return false;
	Isaac.SetTile(-3,-3);

	levelKind = LEVEL_OVERWORLD;
	//Init music
	if (!music.openFromFile("resources/music/overworld.ogg")) {
		//error
	}
	music.play();

	return res;
}

bool cGame::Loop()
{
	bool res=true;
	//Sleep(24);
	res = Process();
	if(res) Render();

	switch(level) 
	{
		case MENU:
			res = ProcessMenu();
			if (res) Render();
			break;
		case CONTROLS:
			res = ProcessBack();
			if (res) Render();
			break;
		case ABOUT:
			res = ProcessBack();
			if (res) Render();
			break;
		default:
			res = Process();
			if(res) Render();
			break;
	}

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

	char attackSide = 0;
	//Game Logic
	if (Player.isBeingPushed())					Player.pushMove(Scene.GetMap());
	else if(keys['z'] && !Player.isAttacking())
	{
		Player.Attack(allSwords, &attackSide);
		DetectCollisionPlayerAttack(&attackSide, false);
	}
	else if(keys[GLUT_KEY_UP])					Player.MoveUp(Scene.GetMap());
	else if(keys[GLUT_KEY_DOWN])				Player.MoveDown(Scene.GetMap());
	else if(keys[GLUT_KEY_LEFT])				Player.MoveLeft(Scene.GetMap());
	else if(keys[GLUT_KEY_RIGHT])				Player.MoveRight(Scene.GetMap());
	else Player.Stop();

	if (allSwords.size() > 0) DetectCollisionPlayerAttack(&attackSide, true);

	std::vector<int> swordPositionsToErase;
	for (int i = 0; i < allSwords.size(); ++i) {
		allSwords[i].Move(Scene.GetMap());
		int state = allSwords[i].GetState();
		if (state != STATE_WALKRIGHT && state != STATE_WALKLEFT && state != STATE_WALKUP && state != STATE_WALKDOWN) {
			swordPositionsToErase.push_back(i);
		}
	}

	for (int j = 0; j < swordPositionsToErase.size(); ++j) {
		allSwords.erase(allSwords.begin()+swordPositionsToErase[j]);
	}

	//dogs behaviour
	for (int i = 0; i < allDogs.size(); ++i){
		if (allDogs[i].isBeingPushed()) allDogs[i].pushMove(Scene.GetMap());
		else allDogs[i].Move(Scene.GetMap(), Player.GetPositionX(), Player.GetPositionY());
	}

	//octopus behaviour
	int playerX, playerY;
	Player.GetPosition(&playerX,&playerY);
	for (int i = 0; i < allOctopus.size(); ++i) {
		if (allOctopus[i].isBeingPushed()) allOctopus[i].pushMove(Scene.GetMap());
		else allOctopus[i].Move(Scene.GetMap(), playerX, playerY);
		if (allOctopus[i].hasBall()) {
			allOctopus[i].getBall()->Move(Scene.GetMap());
			int state = allOctopus[i].getBall()->GetState();
			if (state != STATE_WALKRIGHT && state != STATE_WALKLEFT && state != STATE_WALKUP && state != STATE_WALKDOWN) {
				allOctopus[i].setHasBall(false);
				allOctopus[i].getBall()->SetPosition(-1,-1);
			}
		}
	}

	if (levelKind == LEVEL_BOSS) Isaac.Attack(playerX, playerY);

	std::vector<int> tearPositionsToErase;
	for (int i=0; i < Isaac.getAllTears()->size(); ++i) {
		Isaac.getTear(i)->Move(Scene.GetMap());
		int state = Isaac.getTear(i)->GetState();
		if (state == STATE_LOOKLEFT) {
			tearPositionsToErase.push_back(i);
		}
	}
	for (int j=0; j < tearPositionsToErase.size(); ++j) {
		Isaac.deleteTear(tearPositionsToErase[j]);
	}

	if (!Player.isInvincible()) DetectCollisionsPlayer();

	/*
	if (Player.isDead()) {
		music.stop();
		if (!music.openFromFile("resources/music/gameover.ogg")) {
		//error
		}
		music.play();
	}*/
	
	ChangeLevel();
	closeBossRoom();

	return res;
}

bool cGame::DetectCollisionsPlayer() 
{
	bool playerDamaged = false;
	char pushSide;

	for (int i = 0; !playerDamaged && i < allDogs.size(); ++i) 
	{
			playerDamaged = collisionBetweenBichos(&allDogs[i], &Player, &pushSide);
			if (playerDamaged) Player.takeDamage(allDogs[i].getDamage(), &pushSide);
	}

	for (int i = 0; !playerDamaged && i < allOctopus.size(); ++i) 
	{
		playerDamaged = collisionBetweenBichos(&allOctopus[i], &Player, &pushSide);
		if (playerDamaged) Player.takeDamage(allOctopus[i].getDamage(), &pushSide);
		else if (allOctopus[i].hasBall()) 
		{
			playerDamaged = collisionBetweenBichos(allOctopus[i].getBall(), &Player, &pushSide);
			if (playerDamaged) Player.takeDamage(allOctopus[i].getBall()->getDamage(), &pushSide);
		}
	}

	return playerDamaged;
}
/*
*	bichoActive = the one who is gonna make the damage
*	bichoPassive = the one who is gonna take the damage
*/
bool cGame::collisionBetweenBichos(cBicho *bichoActive, cBicho *bichoPassive, char * pushSide)
{
	int bichoPassiveX, bichoPassiveY, bichoPassiveW, bichoPassiveH;
	int bichoActiveX, bichoActiveY, bichoActiveW, bichoActiveH, bichoPassiveStepLength;

	bichoPassive->GetPosition(&bichoPassiveX, &bichoPassiveY);
	bichoPassive->GetWidthHeight(&bichoPassiveW, &bichoPassiveH);
	bichoPassiveStepLength = bichoPassive->getStepLength();

	bichoActive->GetPosition(&bichoActiveX, &bichoActiveY);
	bichoActive->GetWidthHeight(&bichoActiveW, &bichoActiveH);

	if (bichoPassiveY >= bichoActiveY+bichoActiveH && ((bichoPassiveX <= bichoActiveX && bichoPassiveX+bichoPassiveW >= bichoActiveX) || (bichoPassiveX >= bichoActiveX && bichoPassiveX+bichoPassiveW >= bichoActiveX)))
	{
		int auxbichoPassiveY = bichoPassiveY - bichoPassiveStepLength;
		if (auxbichoPassiveY <= bichoActiveY+bichoActiveH && ((bichoPassiveX <= bichoActiveX && bichoPassiveX+bichoPassiveW >= bichoActiveX) || (bichoPassiveX <= bichoActiveX+bichoActiveW && bichoPassiveX+bichoPassiveW >= bichoActiveX)))
		{
			//collision up
			*pushSide = 'u';
			return true;
		}
	} else if (bichoPassiveY+bichoPassiveH <= bichoActiveY && ((bichoPassiveX <= bichoActiveX && bichoPassiveX+bichoPassiveW >= bichoActiveX) || (bichoPassiveX <= bichoActiveX+bichoActiveW && bichoPassiveX+bichoPassiveW >= bichoActiveX)))
	{
		int auxbichoPassiveY = bichoPassiveY + bichoPassiveStepLength;
		if (auxbichoPassiveY + bichoPassiveH >= bichoActiveY && ((bichoPassiveX <= bichoActiveX && bichoPassiveX+bichoPassiveW >= bichoActiveX) || (bichoPassiveX <= bichoActiveX+bichoActiveW && bichoPassiveX+bichoPassiveW >= bichoActiveX)))
		{
			//collision down
			*pushSide = 'd';
			return true;
		}
	} else if (bichoPassiveX >= bichoActiveX && bichoPassiveX <= bichoActiveX + bichoActiveW && ((bichoPassiveY <= bichoActiveY && bichoPassiveY+bichoPassiveH >= bichoActiveY) || (bichoPassiveY <= bichoActiveY+bichoActiveH && bichoPassiveY+bichoPassiveH >= bichoActiveY+bichoActiveH)))
	{
		int auxbichoPassiveX = bichoPassiveX + bichoPassiveStepLength;
		if (auxbichoPassiveX >= bichoActiveX+bichoActiveW && ((bichoPassiveY <= bichoActiveY && bichoPassiveY+bichoPassiveH >= bichoActiveY) || (bichoPassiveY <= bichoActiveY+bichoActiveH && bichoPassiveY+bichoPassiveH >= bichoActiveY+bichoActiveH)))
		{
			//collision right
			*pushSide = 'r';
			return true;
		}
	} else if (bichoPassiveX+bichoPassiveW >= bichoActiveX && bichoPassiveX+bichoPassiveW <= bichoActiveX && ((bichoPassiveY <= bichoActiveY && bichoPassiveY+bichoPassiveH >= bichoActiveY) || (bichoPassiveY <= bichoActiveY+bichoActiveH && bichoPassiveY+bichoPassiveH >= bichoActiveY+bichoActiveH)))
	{
		int auxbichoPassiveX = bichoPassiveX - bichoPassiveStepLength;
		if (auxbichoPassiveX+bichoPassiveW <= bichoActiveX && ((bichoPassiveY <= bichoActiveY && bichoPassiveY+bichoPassiveH >= bichoActiveY) || (bichoPassiveY <= bichoActiveY+bichoActiveH && bichoPassiveY+bichoPassiveH >= bichoActiveY+bichoActiveH)))
		{
			//collision left
			*pushSide = 'l';
			return true;
		}
	}

	return false;
}

void cGame::closeBossRoom()
{
	if (levelKind == LEVEL_BOSS) {
		if (Player.GetPositionY() + 16 < 2*LEVEL_HEIGHT - 16) {
			Scene.blockBossRoom();
		}
	}
}

void cGame::ChangeLevel()
{
	bool hasLevelKindChanged = false;
	int tileX;
	int tileY;
	Player.GetTile(&tileX, &tileY);
	int playerX, playerY, playerW, playerH;
	Player.GetPosition(&playerX,&playerY);
	Player.GetWidthHeight(&playerW, &playerH);

	playerX = (playerX + playerW / 2) ;
	playerY = (playerY + playerH / 2) ;	//coord player center
	
	if (playerY > currentLevelY + LEVEL_HEIGHT) //going up
	{
		++level;
		currentLevelY += LEVEL_HEIGHT;
	} else if (playerY < currentLevelY)	//going down
	{
		if (level == 4) ++level;
		else --level;
		currentLevelY -= LEVEL_HEIGHT;
	} else if (playerX > currentLevelX + LEVEL_WIDTH) //going right
	{
		++level;
		currentLevelX += LEVEL_WIDTH;
	} else if (playerX < currentLevelX) //going left
	{
		--level;
		currentLevelX -= LEVEL_WIDTH;
	}

	if (level < 3 && levelKind != LEVEL_OVERWORLD)
	{
			levelKind = LEVEL_OVERWORLD;
			hasLevelKindChanged = true;
	}
	else if (level > 2 && level < 5 && levelKind != LEVEL_DUNGEON)
	{
		levelKind = LEVEL_DUNGEON;
		hasLevelKindChanged = true;
	}
	else if (level == 5 && levelKind != LEVEL_BOSS)
	{
		levelKind = LEVEL_BOSS;
		hasLevelKindChanged = true;
	}

	if (hasLevelKindChanged) {
		music.stop();
		switch (levelKind) {
			case LEVEL_OVERWORLD: 
				if (!music.openFromFile("resources/music/overworld.ogg")) {
					//error
				}
				break;
			case LEVEL_DUNGEON:
				if (!music.openFromFile("resources/music/dungeon.ogg")) {
					//error
				}
				break;
			case LEVEL_BOSS:
				Isaac.SetWidthHeight(32,32);
				Isaac.SetTile(23,12);
				Isaac.SetWidthHeight(32,32);
				Isaac.SetState(STATE_LOOKRIGHT);
				if (!music.openFromFile("resources/music/boss.ogg")) {
					//error
				}
				break;
		}
		music.play();	
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

	if (level == MENU)
	{
		RenderMenu(IMG_MENU);
	}else if (level == ABOUT)
	{
		RenderMenu(IMG_ABOUT);
	}else if (level == CONTROLS)
	{
		RenderMenu(IMG_CONTROLS);
	}else
	{
		Scene.Draw(Data.GetID(IMG_TILESET));

		for (int i = 0; i < allDogs.size(); ++i) allDogs[i].Draw(Data.GetID(IMG_DOG));
		for (int i = 0; i < allOctopus.size(); ++i) {
			allOctopus[i].Draw(Data.GetID(IMG_OCTOPUS));
			if (allOctopus[i].hasBall()) allOctopus[i].getBall()->Draw(Data.GetID(IMG_OCTOPUS));
		}

		for (int i=0; i < Isaac.getAllTears()->size(); ++i) {
			Isaac.getTear(i)->Draw(Data.GetID(IMG_OCTOPUS));
		}

		if (!Player.isDead()) Player.Draw(Data.GetID(IMG_PLAYER));

		for (int i = 0; i < allSwords.size(); ++i) allSwords[i].Draw(Data.GetID(IMG_SWORD));

		Isaac.Draw(Data.GetID(IMG_ISAAC));

		DrawMenu();
	}

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

void cGame::DetectCollisionPlayerAttack(char * attackSide, bool beam)
{
	int bichoX, bichoY, bichoW, bichoH;
	int playerX, playerY, playerW, playerH;

	Player.GetPosition(&playerX, &playerY);
	Player.GetWidthHeight(&playerW, &playerH);

	int attackXo, attackYo, attackXf, attackYf;
	switch(*attackSide)
	{
		case 'u':
			attackXo = playerX;
			attackYo = playerY + playerH;
			break;
		case 'd':
			attackXo = playerX;
			attackYo = playerY - playerH;
			break;
		case 'l':
			attackXo = playerX - playerW;
			attackYo = playerY;
			break;
		case 'r':
			attackXo = playerX + playerW;
			attackYo = playerY;
			break;
	}

	//Dogs
	std::vector<int> bichosPositionsToErase;
	std::vector<int> swordsPositionsToErase;

	for (int i = 0; i < allDogs.size(); ++i)
	{
		if (!allDogs[i].isInvincible())
		{
			allDogs[i].GetPosition(&bichoX, &bichoY);
			allDogs[i].GetWidthHeight(&bichoW, &bichoH);
			
			//sword attack
			if (!beam && ((attackXo >= bichoX && attackXo <= bichoX+bichoW) || (attackXo+playerW >= bichoX && attackXo+playerW <= bichoX+bichoW)) 
				&& 
				((attackYo >= bichoY && attackYo <= bichoY+bichoH) || (attackYo+playerH >= bichoY && attackYo+playerH <= bichoY+bichoH)))
			{
				allDogs[i].takeDamage(Player.getDamage(), attackSide);
				if (allDogs[i].isDead()) bichosPositionsToErase.push_back(i);
			} else //sword beam
			{
				for (int k = 0; k < allSwords.size(); ++k)
				{
					if (collisionBetweenBichos(&allDogs[i], &allSwords[k], attackSide))
					{
						if (*attackSide == 'r') *attackSide = 'l';
						else if (*attackSide == 'l') *attackSide = 'r';
						else if (*attackSide == 'u') *attackSide = 'd';
						else *attackSide = 'u';
						allDogs[i].takeDamage(Player.getDamage(), attackSide);
						if (allDogs[i].isDead()) bichosPositionsToErase.push_back(i);
						swordsPositionsToErase.push_back(k);
					}
				}
			}
		}
	}

	for (int j = 0; j < bichosPositionsToErase.size(); ++j) {
		allDogs.erase(allDogs.begin()+bichosPositionsToErase[j]);
	}
	bichosPositionsToErase.clear();
	for (int l = 0; l < swordsPositionsToErase.size(); ++l) {
		allSwords.erase(allSwords.begin()+swordsPositionsToErase[l]);
	}
	swordsPositionsToErase.clear();	

	for (int i = 0; i < allOctopus.size(); ++i)
	{
		if (!allOctopus[i].isInvincible())
		{
			allOctopus[i].GetPosition(&bichoX, &bichoY);
			allOctopus[i].GetWidthHeight(&bichoW, &bichoH);

			if (!beam &&((attackXo >= bichoX && attackXo <= bichoX+bichoW) || (attackXo+playerW >= bichoX && attackXo+playerW <= bichoX+bichoW)) 
				&& 
				((attackYo >= bichoY && attackYo <= bichoY+bichoH) || (attackYo+playerH >= bichoY && attackYo+playerH <= bichoY+bichoH)))
			{
				allOctopus[i].takeDamage(Player.getDamage(), attackSide);
				if (allOctopus[i].isDead()) bichosPositionsToErase.push_back(i);
			}else //sword beam
			{
				for (int k = 0; k < allSwords.size(); ++k)
				{
					if (collisionBetweenBichos(&allOctopus[i], &allSwords[k], attackSide))
					{
						if (*attackSide == 'r') *attackSide = 'l';
						else if (*attackSide == 'l') *attackSide = 'r';
						else if (*attackSide == 'u') *attackSide = 'd';
						else *attackSide = 'u';
						allOctopus[i].takeDamage(Player.getDamage(), attackSide);
						if (allOctopus[i].isDead()) bichosPositionsToErase.push_back(i);
						swordsPositionsToErase.push_back(k);
					}
				}
			}
		}
	}

	for (int j = 0; j < bichosPositionsToErase.size(); ++j) {
		allOctopus.erase(allOctopus.begin()+bichosPositionsToErase[j]);
	}
	for (int l = 0; l < swordsPositionsToErase.size(); ++l) {
		allSwords.erase(allSwords.begin()+swordsPositionsToErase[l]);
	}
}

bool cGame::ProcessMenu() 
{
	//Game Logic
	if(keys['p']) {
		level = 1;
	}
	else if(keys['c'])	level = CONTROLS;
	else if(keys['a'])	level = ABOUT;

	return true;
}

void cGame::RenderMenu(int imgId)
{
	glEnable(GL_TEXTURE_2D);
	
	glBindTexture(GL_TEXTURE_2D, Data.GetID(imgId));
	glBegin(GL_QUADS);	
		glTexCoord2f(0,1);		glVertex2i(0  ,0);
		glTexCoord2f(1,1);		glVertex2i(LEVEL_WIDTH,0);
		glTexCoord2f(1,0);		glVertex2i(LEVEL_WIDTH,LEVEL_HEIGHT+MENU_MARGIN);
		glTexCoord2f(1,0);		glVertex2i(0  ,LEVEL_HEIGHT+MENU_MARGIN);
	glEnd();

	glDisable(GL_TEXTURE_2D);
}

bool cGame::ProcessBack()
{
	//Game Logic
	if(keys['b'])		level = MENU;

	return true;
}

int cGame::getLevel()
{
	return this->level;
}

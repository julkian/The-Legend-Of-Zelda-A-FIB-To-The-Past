#pragma once

#include "cScene.h"
#include "cData.h"
#include "cSword.h"
#include "cPlayer.h"
#include "cDog.h"
#include "cOctopus.h"
#include "cBicho.h"
#include "cIsaac.h"
#include "cTear.h"
#include <vector>

#define GAME_WIDTH		640
#define GAME_HEIGHT		550

#define LEVEL_HEIGHT	176 //11*16
#define LEVEL_WIDTH		256 //16*16
#define MENU_MARGIN		30

#define LEVEL_OVERWORLD	1
#define LEVEL_DUNGEON	2
#define LEVEL_BOSS		3

#define MENU			60
#define CONTROLS		70
#define ABOUT			50


class cGame
{
public:
	cGame(void);
	virtual ~cGame(void);

	bool Init();
	bool Loop();
	void Finalize();

	//Input
	void ReadKeyboard(unsigned char key, int x, int y, bool press);
	void ReadMouse(int button, int state, int x, int y);
	
	//Process
	bool Process();
	void ChangeLevel();
	bool DetectCollisionsPlayer();		//collision bichos with player
	void DetectCollisionPlayerAttack(char * attackSide, bool beam); //collision player attacking against bichos

	//Output
	void Render();
	void DrawMenu();

	void drawBichos();

	int getLevel();
	void putEnemies();

private:
	unsigned char keys[256];
	int level;
	int levelKind;
	cScene Scene;
	cData Data;
	cPlayer Player;
	std::vector<cSword> allSwords;
	std::vector<cOctopus> allOctopus;
	std::vector<cDog> allDogs;
	cIsaac Isaac;
	//std::vector<cWizard> allWizards;
	int currentLevelX, currentLevelY;
	sf::Music music;

	//menu
	bool ProcessMenu();
	bool ProcessBack();
	
	void RenderMenu(int imgId);

	//methods
	void closeBossRoom();
	bool collisionBetweenBichos(cBicho *bichoActive, cBicho *bichoPassive, char * pushSide);


};

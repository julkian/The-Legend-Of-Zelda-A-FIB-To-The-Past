#pragma once

#include "cScene.h"
#include "cData.h"
#include "cSword.h"
#include "cPlayer.h"
#include "cDog.h"
#include "cOctopus.h"
#include "cBicho.h"
#include <vector>

#define GAME_WIDTH		640
#define GAME_HEIGHT		550

#define LEVEL_HEIGHT	176 //11*16
#define LEVEL_WIDTH		256 //16*16
#define MENU_MARGIN		30

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
	bool DetectCollisionsPlayer(); //collision

	//Output
	void Render();
	void DrawMenu();

	void drawBichos();

private:
	unsigned char keys[256];
	cScene Scene;
	cData Data;
	cPlayer Player;
	std::vector<cSword> allSwords;
	std::vector<cOctopus> allOctopus;
	std::vector<cDog> allDogs;
	//std::vector<cWizard> allWizards;
	int currentLevelX, currentLevelY;
	sf::Music music;

	//methods
	bool collisionBichoPlayer(cBicho *bicho);
};

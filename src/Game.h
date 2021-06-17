#ifndef GAME_H
#define GAME_H

#include "Scoreboard.h"
#include "olcPixelGameEngine.h"

#include "Ending.h"
#include "Level.h"
#include "Menu.h"
#include "Object.h"
#include "StarMap.h"
#include "TimeAttack.h"
#include "TitleCard.h"

#include <vector>

#ifndef __EMSCRIPTEN__
	#include <Windows.h>
#endif

enum GameState
{
	MAIN_MENU,
	GAME,
	ENDING
};

enum GameContent
{
	NORMAL,
	ENCORE
};

enum GameMode
{
	MAIN,
	TIME_ATTACK,
	SURVIVAL
};

class Game : public olc::PixelGameEngine
{
public:
	GameState state;
	GameContent content;
	GameMode mode;
public:
	MainMenu* mainMenu;
	PauseMenu* pauseMenu;
	Ending* ending;
	TimeAttack* timeAttack;
	Scoreboard* sb;
	StarMap* starMap;
	Level* levels;
	Player* player;
	std::vector<Object*> vObjects;
	bool playerControl;
public:
	Game();
	~Game();

	bool OnUserCreate() override;
	bool OnUserUpdate(float fElapsedTime) override;

	void Update(float fElapsedTime);

	void DrawStringDecalXAligned(const std::string& sText, const olc::vi2d& offset, const olc::Pixel& col = olc::WHITE, const olc::vf2d& scale = { 1.0f, 1.0f });

	void Restart();
};

inline Game* game = new Game();

#endif
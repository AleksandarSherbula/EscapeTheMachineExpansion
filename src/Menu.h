#ifndef MENU_H
#define MENU_H

#include <memory>
#include <string>
#include <array>

#include "olcPixelGameEngine.h"
#include "TitleCard.h"
#include "Level.h"
#include "TimeAttack.h"

class MainMenu
{
public:
	TitleCard* titleCard;
	TitleCard* subTitleCard;

	bool gameStart = false;
	float fTimer = 0.0f;

	std::string strMode;
public:
	enum ChooseList{ CHOOSE_CONTENT, CHOOSE_GAME_MODE, SHOW_LEADERBOARD, CREDITS } chooseList;
private:
	int mMenuHoverLighted = 0;
	std::array<olc::Pixel, 4> mHoverColor;
	bool isRefresh;
public:
	MainMenu();
	~MainMenu();

	void Update(float fElapsedTime);
	void Reset();	
};

class PauseMenu
{
private:
	int mPauseHoverLighted = 0;
	bool m_bPrompt;
	std::string m_strPrompt;
	std::array<olc::Pixel, 2> mPromptHoverColor;	
	std::array<olc::Pixel, 3> mHoverColor;	
public:
	bool bIsOn;
public:
	PauseMenu();
	void Update(float fElapsedTime, Level* level, TimeAttack* ta, olc::vf2d& playerInitPosition);
	void Print();
};
#endif
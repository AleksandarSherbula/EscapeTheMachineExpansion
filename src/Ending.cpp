#include "Game.h"
#include "Ending.h"

#include <time.h>

Ending::Ending()
{
    srand(time(NULL));
    mMountains =  new SpriteObject("Mountains");
    mGrass     =  new SpriteObject("Grass");
    mVictory   =  new SpriteObject("Victory");
    mEncore    =  new SpriteObject("Encore");
    mSurvive   =  new SpriteObject("Survival");
	mLpoGuy    =  new SpriteObject("LPOguy");

	mTimeScoreName = "";
	incrementCount = true;
	mCanType = true;

	mClouds = new SpriteObject* [12];
    for (int i = 0; i < 12; i++)
    {
        mClouds[i] = new SpriteObject("Cloud");
        mClouds[i]->position = { (float)(i * 2) * 32.0f, (float)(rand() % 4) * 16.0f };
    }
}

Ending::~Ending()
{
	delete mMountains;
	delete mGrass;
	delete mVictory;
	delete mEncore;
	delete mSurvive;
	delete mLpoGuy;

	for (int i = 0; i < 12; i++)
		delete mClouds[i];

	delete[] mClouds;
}

void Ending::Update(float fElapsedTime)
{
	if (game->mode != TIME_ATTACK)
	{
		game->player->position = { 10.0f, 13.0f };

		game->player->size.x += 10 * fElapsedTime;
		game->player->size.y += 10 * fElapsedTime;

		if (game->player->size.x >= 16.0f &&
			game->player->size.y >= 16.0f)
		{
			game->player->size = { 16.0f, 16.0f };

			if (mTimer <= 1.0f)
				mTimer += fElapsedTime;
		}

		if (mTimer > 1.0f)
		{
			if (game->content == NORMAL)
			{
				if (game->GetKey(olc::ENTER).bPressed)
				{
					if (game->mode == MAIN)
						game->sb->IncrementCount("normal/main");
					else if (game->mode == SURVIVAL)
						game->sb->IncrementCount("normal/survival");

					mTimer = 0.0f;
					game->Restart();
					game->state = MAIN_MENU;
				}
			}
			else if (game->content == ENCORE)
			{
				if (game->GetKey(olc::ENTER).bPressed)
				{					
					if (mEncoreDialogTextID <= 13)
						mEncoreDialogTextID++;
					else
					{
						if (game->mode == MAIN)
							game->sb->IncrementCount("encore/main");
						else if (game->mode == SURVIVAL)
							game->sb->IncrementCount("encore/survival");

						mTimer = 0.0f;
						game->Restart();
						game->state = MAIN_MENU;
						mLpoGuy->sprSheetOffset = { 0, 0 };
						mEncoreDialogTextID = 0;
					}
				}
				if (game->GetKey(olc::ESCAPE).bPressed)
					mEncoreDialogTextID = 14;
			}
		}

		game->Clear(olc::CYAN);

		for (int i = 0; i < 12; i++)
			mClouds[i]->DrawFullSprite();

		mMountains->DrawFullSprite(olc::vi2d(-10, -40));

		game->FillRectDecal(olc::vi2d(0, 210), olc::vi2d(480, 110), olc::Pixel(0, 128, 255));

		for (int i = 0; i < 15; i++)
			mGrass->DrawFullSprite(olc::vi2d(i * 32, 222.0f));

		game->player->Draw();

		if (mTimer > 1.0f)
		{
			if (game->mode == SURVIVAL)
				mSurvive->DrawFullSprite(olc::vi2d(110, 48));

			if (game->content == NORMAL)
			{
				mVictory->DrawFullSprite(olc::vi2d(120, 120));
				game->DrawStringDecal(olc::vi2d(100, 180), "Press Enter to go back to main menu!");
			}
			else
			{
				if (game->mode == SURVIVAL)
					mEncore->DrawFullSprite(olc::vi2d(140, 16));

				EncoreDialog();
			}
		}

		if (game->content == ENCORE)
			mLpoGuy->Draw(olc::vf2d(16.0f, 12.0f) * 16, olc::WHITE, { 32, 32 });
	}
	else
	{
		game->starMap->Update(fElapsedTime);

		bool curseWordFound = game->timeAttack->FindCurseWord(mTimeScoreName);

		if (mCanType)
		{	
			if (mTimeScoreName.size() < 10)
			{
				for (int i = 1; i < 47; i++)
				{
					if (game->GetKey((olc::Key)i).bPressed)
					{
						if (i >= 27 && i <= 36)
							mTimeScoreName += (i + 21);
						else if (i >= 37 && i <= 46)
							mTimeScoreName += (i + 11);
						else
							mTimeScoreName += (i + 64);
					}
				}				
			}			

			if (game->GetKey(olc::BACK).bPressed && mTimeScoreName.size() > 0)
				mTimeScoreName.resize(mTimeScoreName.size() - 1);

			if (game->GetKey(olc::ENTER).bPressed && !curseWordFound)
			{
				mCanType = false;

				uint32_t& minutes = game->timeAttack->mCurrentMinutes;
				uint32_t& seconds = game->timeAttack->mCurrentSeconds;
				uint32_t& miliseconds = game->timeAttack->mCurrentMiliSeconds;
				
				//Update Leaberboard
				if (game->content == NORMAL)
					game->sb->NewScore("normal", mTimeScoreName, minutes, seconds, miliseconds);
				else
					game->sb->NewScore("encore", mTimeScoreName, minutes, seconds, miliseconds);

				game->sb->RefreshScores();				
			}			
		}
		else
		{
			if (game->GetKey(olc::ENTER).bPressed)
			{
				incrementCount = true;
				if (game->content == NORMAL)
					game->sb->IncrementCount("normal/time");
				else
					game->sb->IncrementCount("encore/time");

				game->Restart();
				game->state = MAIN_MENU;
			}
		}

		game->Clear(olc::Pixel(0, 0, 30));

		game->starMap->Draw();

		if (mCanType)
		{
			game->timeAttack->PrintTime();
			game->DrawStringDecalXAligned("Enter your name:", olc::vi2d(0, 120), olc::WHITE, {2.0f, 2.0f});
			game->FillRectDecal(olc::vi2d(game->ScreenWidth() / 2 - 5 * 16 - 5, 144), olc::vi2d(10 * 16 + 10, 16 + 10), olc::BLUE);
			game->DrawStringDecal(olc::vi2d(game->ScreenWidth() / 2 - 5 * 16, 144 + 5), mTimeScoreName,  olc::WHITE, { 2.0f, 2.0f });
			game->DrawStringDecalXAligned("Press Enter to finish name typing", olc::vi2d(0, 288));
		}
		else
		{
			std::string& name = mTimeScoreName;
			uint32_t& minutes = game->timeAttack->mCurrentMinutes;
			uint32_t& seconds = game->timeAttack->mCurrentSeconds;
			uint32_t& miliseconds = game->timeAttack->mCurrentMiliSeconds;

			game->timeAttack->PrintLeaderboard((TimeAttack::ScoreList)game->content, name, minutes, seconds, miliseconds);
			game->DrawStringDecalXAligned("Press enter to go to main menu", olc::vi2d(0, 286));
		}

		if (curseWordFound)
		{
			game->DrawStringDecalXAligned("FORBIDDEN WORD!!!", olc::vi2d(0, 208), olc::WHITE, { 2.0f, 2.0f });
			game->DrawStringDecalXAligned("PLEASE CHOOSE ANOTHER NAME", olc::vi2d(0, 226), olc::WHITE, { 2.0f, 2.0f });
		}
	}
    
}

void Ending::EncoreDialog()
{
	if (mEncoreDialogTextID < 14)
	{
		game->FillRectDecal(olc::vi2d(22, 243), olc::vi2d(435, 60), olc::BLUE);
		game->FillRectDecal(olc::vi2d(22, 243), olc::vi2d(435, 1));
		game->FillRectDecal(olc::vi2d(22, 243), olc::vi2d(1, 60));
		game->FillRectDecal(olc::vi2d(22, 303), olc::vi2d(435, 1));
		game->FillRectDecal(olc::vi2d(457, 243), olc::vi2d(1, 60));
		game->DrawStringDecal(olc::vi2d(game->ScreenWidth() / 2 - 23 * 8, 307), "Enter:Continue                        ESC:Skip");
	}
	else if (mEncoreDialogTextID == 14)
		game->DrawStringDecal(olc::vi2d(game->ScreenWidth() / 2 - 18 * 8, 307), "Press Enter to go back to main menu!");

	if (mEncoreDialogTextID == 0)
	{
		game->DrawStringDecal(olc::vi2d(29, 250), "Let's Platform Outdoors guy:");
		game->DrawStringDecal(olc::vi2d(29, 260), "Well, well, well, if it isn't a small rectangle that");
		game->DrawStringDecal(olc::vi2d(29, 270), "has enjoyed the success of olcCodeJam 2020.");
	}
	else if (mEncoreDialogTextID == 1)
	{
		game->DrawStringDecal(olc::vi2d(27, 250), "Let's Platform Outdoors guy:");
		game->DrawStringDecal(olc::vi2d(27, 260), "Enjoying your fame? Of course you are. You are");
		game->DrawStringDecal(olc::vi2d(27, 270), "afterall much more succesful than i was in the");
		game->DrawStringDecal(olc::vi2d(27, 280), "olcBeatTheBoredom.");
	}
	else if (mEncoreDialogTextID == 2)
	{
		game->DrawStringDecal(olc::vi2d(27, 250), "Let's Platform Outdoors guy:");
		game->DrawStringDecal(olc::vi2d(27, 260), "But don't get too cocky. You only got at 19th");
		game->DrawStringDecal(olc::vi2d(27, 270), "spot. Though it is better than my 81st spot.");
	}
	else if (mEncoreDialogTextID == 3)
	{
		mLpoGuy->sprSheetOffset = { 1, 0 };
		game->DrawStringDecal(olc::vi2d(27, 250), "Let's Platform Outdoors guy:");
		game->DrawStringDecal(olc::vi2d(27, 260), "Because the programmer here who made your game was");
		game->DrawStringDecal(olc::vi2d(27, 270), "too incompetent to make an actual platformer.");
		game->DrawStringDecal(olc::vi2d(27, 280), "Instead, i got to enjoy being a boring walking");
		game->DrawStringDecal(olc::vi2d(27, 290), "simulator. How could you do this to me?!");
	}
	else if (mEncoreDialogTextID == 4)
	{
		game->DrawStringDecal(olc::vi2d(27, 250), "Alexio:");
		game->DrawStringDecal(olc::vi2d(27, 260), "Oh come on. It was my very first participation in");
		game->DrawStringDecal(olc::vi2d(27, 270), "a jam. Cut me some slack.");
	}
	else if (mEncoreDialogTextID == 5)
	{
		game->DrawStringDecal(olc::vi2d(27, 250), "Let's Platform Outdoors guy:");
		game->DrawStringDecal(olc::vi2d(27, 260), "And you have to toss me into your lameness. Wow,");
		game->DrawStringDecal(olc::vi2d(27, 270), "that's a d*** move.");
	}
	else if (mEncoreDialogTextID == 6)
	{
		game->DrawStringDecal(olc::vi2d(27, 250), "Alexio:");
		game->DrawStringDecal(olc::vi2d(27, 260), "HEY, HEY, don't curse. Kids might be playing this.");
	}
	else if (mEncoreDialogTextID == 7)
	{
		mLpoGuy->sprSheetOffset = { 2, 0 };
		game->DrawStringDecal(olc::vi2d(27, 250), "Let's Platform Outdoors guy:");
		game->DrawStringDecal(olc::vi2d(27, 260), "OH SURE!! Because you are now top star!! Everyone");
		game->DrawStringDecal(olc::vi2d(27, 270), "wants to play your games. They just want to drop");
		game->DrawStringDecal(olc::vi2d(27, 280), "their lives to play your games. SURE! Pro devs");
		game->DrawStringDecal(olc::vi2d(27, 290), "ain't got nothing on you.");
	}
	else if (mEncoreDialogTextID == 8)
	{
		game->DrawStringDecal(olc::vi2d(27, 250), "Alexio:");
		game->DrawStringDecal(olc::vi2d(27, 260), "Ok, ok, i get it! Dear lord, calm down.");
	}
	else if (mEncoreDialogTextID == 9)
	{
		game->DrawStringDecal(olc::vi2d(27, 250), "Alexio:");
		game->DrawStringDecal(olc::vi2d(27, 260), "Tell you what, I will make it up to you. Would you");
		game->DrawStringDecal(olc::vi2d(27, 270), "like me to put you in some of the future games i want");
		game->DrawStringDecal(olc::vi2d(27, 280), "to make?");
	}
	else if (mEncoreDialogTextID == 10)
	{
		game->DrawStringDecal(olc::vi2d(27, 250), "Let's Platform Outdoors guy:");
		game->DrawStringDecal(olc::vi2d(27, 260), "Promise?");
	}
	else if (mEncoreDialogTextID == 11)
	{
		game->DrawStringDecal(olc::vi2d(27, 250), "Alexio:");
		game->DrawStringDecal(olc::vi2d(27, 260), "Promise.");
	}
	else if (mEncoreDialogTextID == 12)
	{
		game->DrawStringDecal(olc::vi2d(27, 250), "Let's Platform Outdoors guy:");
		game->DrawStringDecal(olc::vi2d(27, 260), "...........................");
	}
	else if (mEncoreDialogTextID == 13)
	{
		mLpoGuy->sprSheetOffset = { 3, 0 };
		game->DrawStringDecal(olc::vi2d(27, 250), "Let's Platform Outdoors guy:");
		game->DrawStringDecal(olc::vi2d(27, 260), "DEAL!!!");
	}
	
}

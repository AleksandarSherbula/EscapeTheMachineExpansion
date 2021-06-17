#include "Game.h"
#include "Menu.h"
#include "TitleCard.h"

MainMenu::MainMenu()
{
    titleCard = new TitleCard("Title");
    subTitleCard = new TitleCard("Subtitle");
    chooseList = CHOOSE_CONTENT;
    isRefresh = false;
}

MainMenu::~MainMenu()
{
    delete titleCard;
    delete subTitleCard;
}

void MainMenu::Update(float fElapsedTime)
{
    fTimer += fElapsedTime;

    if (fTimer > 2.0f)
    {
        titleCard->alpha += 200 * fElapsedTime;

        if (titleCard->alpha >= 255.0f)
        {
            titleCard->alpha = 255.0f;
        }

        if (titleCard->alpha >= 180.0f)
        {
            subTitleCard->alpha += 300 * fElapsedTime;
            if (subTitleCard->alpha >= 255.0f)
                subTitleCard->alpha = 255.0f;
        }

        if (titleCard->alpha == 255.0f && subTitleCard->alpha == 255.0f
            && fTimer > 5.0f)
        {
            for (int i = 0; i < 4; i++)
                mHoverColor[i] = olc::DARK_GREY;

            mHoverColor[mMenuHoverLighted] = olc::WHITE;

            if (game->GetKey(olc::W).bPressed || game->GetKey(olc::UP).bPressed)
            {
                if (mMenuHoverLighted == 0)
                {
                    mMenuHoverLighted = 3;
                }
                else
                    mMenuHoverLighted--;
            }
            else if (game->GetKey(olc::S).bPressed || game->GetKey(olc::DOWN).bPressed)
            {                
                if (mMenuHoverLighted == 3)
                    mMenuHoverLighted = 0;
                else
                    mMenuHoverLighted++;
            }

            if (chooseList == SHOW_LEADERBOARD)
            {                
                if ((game->GetKey(olc::D).bPressed || game->GetKey(olc::RIGHT).bPressed)
                    && game->timeAttack->scoreList == TimeAttack::ScoreList::NORMAL)
                {
                    // NORMAL LIST
                    game->timeAttack->scoreList = game->timeAttack->ScoreList::ENCORE;
                }

                if ((game->GetKey(olc::A).bPressed || game->GetKey(olc::LEFT).bPressed)
                    && game->timeAttack->scoreList == TimeAttack::ScoreList::ENCORE)
                {
                    // ENCORE LIST
                    game->timeAttack->scoreList = TimeAttack::ScoreList::NORMAL;
                }
            }

            if (game->GetKey(olc::ENTER).bPressed)
            {
                if (chooseList == SHOW_LEADERBOARD || chooseList == CREDITS)
                {
                    chooseList = CHOOSE_CONTENT;
                }
                else if (chooseList == CHOOSE_GAME_MODE)
                {
                    switch (mMenuHoverLighted)
                    {
                    case 0: game->mode = GameMode::MAIN; break;
                    case 1: game->mode = GameMode::TIME_ATTACK; game->timeAttack->timeRunning = true; game->timeAttack->Start(); break;
                    case 2: game->mode = GameMode::SURVIVAL; break;
                    case 3: chooseList = CHOOSE_CONTENT; break;
                    }

                    for (int i = 0; i < 3; i++)
                    {
                        if (mHoverColor[i] == olc::WHITE)
                        {
                            gameStart = true;
                            game->state = GameState::GAME;
                            fTimer = 0.0f;
                            titleCard->alpha = 0.0f;
                            subTitleCard->alpha = 0.0f;
                        }                        
                    }                    
                }
                else if (chooseList == CHOOSE_CONTENT)
                {
                    switch (mMenuHoverLighted)
                    {
                    case 0: strMode = "normal"; game->content = GameContent::NORMAL; chooseList = CHOOSE_GAME_MODE; break;
                    case 1: strMode = "encore"; game->content = GameContent::ENCORE; chooseList = CHOOSE_GAME_MODE; break;
                    case 2: chooseList = SHOW_LEADERBOARD; game->sb->RefreshScores(); break;
                    case 3: chooseList = CREDITS; break;
                    }
                }

                mMenuHoverLighted = 0;
            }
        }
    }

    game->Clear(olc::Pixel(0, 0, 30));

    if (chooseList != SHOW_LEADERBOARD && chooseList != CREDITS)
    {
        titleCard->Draw(olc::vi2d(130, 80), olc::WHITE);
        subTitleCard->Draw(olc::vi2d(140, 180), olc::DARK_CYAN);
    }    

    if (titleCard->alpha == 255.0f && subTitleCard->alpha == 255.0f
        && fTimer > 5.0f)
    {
        if (chooseList == CHOOSE_CONTENT)
        {
            game->DrawStringDecalXAligned("Normal Game", olc::vi2d(0, 250), mHoverColor[0]);
            game->DrawStringDecalXAligned("Encore Game", olc::vi2d(0, 262), mHoverColor[1]);
            game->DrawStringDecalXAligned("Leaderboard", olc::vi2d(0, 274), mHoverColor[2]);
            game->DrawStringDecalXAligned("Credits", olc::vi2d(0, 286), mHoverColor[3]);
        }
        else if (chooseList == CHOOSE_GAME_MODE)
        {
            game->DrawStringDecalXAligned("Main Mode", olc::vi2d(0, 250), mHoverColor[0]);
            game->DrawStringDecalXAligned("Time Attack Mode", olc::vi2d(0, 262), mHoverColor[1]);
            game->DrawStringDecalXAligned("Survival Mode", olc::vi2d(0, 274), mHoverColor[2]);
            game->DrawStringDecalXAligned("Back", olc::vi2d(0, 286), mHoverColor[3]);
        }
        else if (chooseList == SHOW_LEADERBOARD)
        {     
            game->timeAttack->PrintLeaderboard(game->timeAttack->scoreList);
            game->DrawStringDecalXAligned("Press enter to go back", olc::vi2d(0, 286));
        }
        else
        {
            game->DrawStringDecalXAligned("Game Created by",               olc::vi2d(0, 60),  olc::YELLOW, { 2.0f, 2.0f });
            game->DrawStringDecalXAligned("Aleksandar Sherbula",           olc::vi2d(0, 80),  olc::WHITE, { 2.0f, 2.0f });

            game->DrawStringDecalXAligned("Leaderboard Created by",        olc::vi2d(0, 110), olc::YELLOW, { 2.0f, 2.0f });
            game->DrawStringDecalXAligned("Moros1138",                     olc::vi2d(0, 130), olc::WHITE, { 2.0f, 2.0f });

            game->DrawStringDecalXAligned("---External Credit---",         olc::vi2d(0, 180), olc::YELLOW, { 2.0f, 2.0f });
            game->DrawStringDecalXAligned("olcPixelGameEngine Created by", olc::vi2d(-8, 210), olc::YELLOW, { 2.0f, 2.0f });
            game->DrawStringDecalXAligned("javidx9",                       olc::vi2d(0, 230), olc::WHITE, { 2.0f, 2.0f });

            game->DrawStringDecalXAligned("Press enter to go back",        olc::vi2d(0, 286));
        }

    }
}

void MainMenu::Reset()
{
    chooseList = CHOOSE_CONTENT;
}

PauseMenu::PauseMenu()
{
    m_bPrompt = false;
    bIsOn = false;
}

void PauseMenu::Update(float fElapsedTime, Level* level, TimeAttack* ta, olc::vf2d& playerInitPosition)
{
    if (!m_bPrompt)
    {
        for (int i = 0; i < mHoverColor.size(); i++)
            mHoverColor[i] = olc::DARK_GREY;

        mHoverColor[mPauseHoverLighted] = olc::WHITE;    
    
        if (game->GetKey(olc::W).bPressed || game->GetKey(olc::UP).bPressed)
        {
            if (mPauseHoverLighted == 0)
                mPauseHoverLighted = 2;
            else
                mPauseHoverLighted--;
        }
        else if (game->GetKey(olc::S).bPressed || game->GetKey(olc::DOWN).bPressed)
        {
            if (mPauseHoverLighted == 2)
                mPauseHoverLighted = 0;
            else
                mPauseHoverLighted++;
        }
    }
    else
    {
        for (int i = 0; i < mPromptHoverColor.size(); i++)
            mPromptHoverColor[i] = olc::DARK_GREY;

        mPromptHoverColor[mPauseHoverLighted] = olc::WHITE;

        if (game->GetKey(olc::A).bPressed || game->GetKey(olc::LEFT).bPressed)
        {
            mPauseHoverLighted = 0;
        }
        else if (game->GetKey(olc::D).bPressed || game->GetKey(olc::RIGHT).bPressed)
        {
            mPauseHoverLighted = 1;
        }
    }
    

    if (game->GetKey(olc::ENTER).bPressed)
    {
        if (!m_bPrompt)
        {
            int exit = 2;
            int reset = 1;
            if (mPauseHoverLighted == 0)
            {
                bIsOn = false;
            }
            else
            {
                if (mPauseHoverLighted == exit)
                    m_strPrompt = "You are leaving the game!";
                else
                    m_strPrompt = "You will go back to first level!";

                m_bPrompt = true;
                mPauseHoverLighted = 1;    
            }
        }
        else
        {            
            if (mPauseHoverLighted == 0)
            {
                if (m_strPrompt == "You are leaving the game!")
                    game->state = MAIN_MENU;

                game->timeAttack->Reset();
                game->Restart();                
                bIsOn = false;
            }

            mPauseHoverLighted = 0;
            m_bPrompt = false;
            
        }
    }   
}

void PauseMenu::Print()
{
    game->FillRectDecal(olc::vi2d(game->ScreenWidth() / 2 - 132 - 4, game->ScreenHeight() / 4 - 4), olc::vi2d(11, 1) * 24 + 4, olc::BLUE);
    game->FillRectDecal(olc::vi2d(game->ScreenWidth() / 2 - 132 - 4, game->ScreenHeight() / 4 - 4), olc::vi2d(11 * 24 + 4, 1) , olc::WHITE);
    game->FillRectDecal(olc::vi2d(game->ScreenWidth() / 2 - 132 - 4, game->ScreenHeight() / 4 - 4), olc::vi2d(1, 24 + 4) , olc::WHITE);
    game->FillRectDecal(olc::vi2d(game->ScreenWidth() / 2 - 132 - 4, (game->ScreenHeight() / 4 - 4) + 24 + 4), olc::vi2d(11 * 24 + 4, 1) , olc::WHITE);
    game->FillRectDecal(olc::vi2d((game->ScreenWidth() / 2 - 132 - 4) + (11 * 24 + 4), (game->ScreenHeight() / 4 - 4)), olc::vi2d(1, 24 + 4) , olc::WHITE);
    game->DrawStringDecal(olc::vi2d(game->ScreenWidth() / 2 - 132, game->ScreenHeight() / 4), "GAME PAUSED", olc::WHITE, {3.0f, 3.0f});   

    if (!m_bPrompt)
    {
        game->FillRectDecal(olc::vi2d(game->ScreenWidth() / 2 - 60, game->ScreenHeight() / 2 - 20), olc::vi2d(7, 3) * 16 + 4, olc::BLUE);
        game->FillRectDecal(olc::vi2d(game->ScreenWidth() / 2 - 60, game->ScreenHeight() / 2 - 20), olc::vi2d(7 * 16 + 4, 1), olc::WHITE);
        game->FillRectDecal(olc::vi2d(game->ScreenWidth() / 2 - 60, game->ScreenHeight() / 2 - 20), olc::vi2d(1, 3 * 16 + 4), olc::WHITE);
        game->FillRectDecal(olc::vi2d(game->ScreenWidth() / 2 - 60, (game->ScreenHeight() / 2 - 20) + (3 * 16 + 4)), olc::vi2d(7 * 16 + 4, 1), olc::WHITE);
        game->FillRectDecal(olc::vi2d((game->ScreenWidth() / 2 - 60) + (7 * 16 + 4), (game->ScreenHeight() / 2 - 20)), olc::vi2d(1, 3 * 16 + 4), olc::WHITE);

        game->DrawStringDecal(olc::vi2d(game->ScreenWidth() / 2 - 48, game->ScreenHeight() / 2 - 16), "Resume", mHoverColor[0], { 2.0f, 2.0f });
        game->DrawStringDecal(olc::vi2d(game->ScreenWidth() / 2 - 56, game->ScreenHeight() / 2), "Restart", mHoverColor[1], { 2.0f, 2.0f });
        game->DrawStringDecal(olc::vi2d(game->ScreenWidth() / 2 - 32, game->ScreenHeight() / 2 + 16), "Exit", mHoverColor[2], { 2.0f, 2.0f });
    }
    else
    {
        game->FillRectDecal(olc::vi2d(game->ScreenWidth() / 2 - 16 * 12 - 4, game->ScreenHeight() / 2 - 20), olc::vi2d(32 * 12 + 10, 6 * 12 + 2), olc::BLUE);
        game->FillRectDecal(olc::vi2d(game->ScreenWidth() / 2 - 16 * 12 - 4, game->ScreenHeight() / 2 - 20), olc::vi2d(32 * 12 + 10, 1), olc::WHITE);
        game->FillRectDecal(olc::vi2d(game->ScreenWidth() / 2 - 16 * 12 - 4, game->ScreenHeight() / 2 - 20), olc::vi2d(1, 6 * 12 + 2), olc::WHITE);
        game->FillRectDecal(olc::vi2d(game->ScreenWidth() / 2 - 16 * 12 - 4, (game->ScreenHeight() / 2 - 20) + (6 * 12 + 2)), olc::vi2d(32 * 12 + 10, 1), olc::WHITE);
        game->FillRectDecal(olc::vi2d((game->ScreenWidth() / 2 - 16 * 12 - 4) + (32 * 12 + 10), game->ScreenHeight() / 2 - 20), olc::vi2d(1, 6 * 12 + 2), olc::WHITE);

        if (m_strPrompt == "You are leaving the game!")
            game->DrawStringDecal(olc::vi2d(game->ScreenWidth() / 2 - (m_strPrompt.size() / 2) * 12 - 6, game->ScreenHeight() / 2 - 16), m_strPrompt, olc::YELLOW, { 1.5f, 1.5f });
        else
            game->DrawStringDecal(olc::vi2d(game->ScreenWidth() / 2 - (m_strPrompt.size() / 2) * 12 + 2, game->ScreenHeight() / 2 - 16), m_strPrompt, olc::YELLOW, { 1.5f, 1.5f });

        game->DrawStringDecal(olc::vi2d(game->ScreenWidth() / 2 - 13 * 12, game->ScreenHeight() / 2), "All progress will be lost!", olc::YELLOW, {1.5f, 1.5f});
        game->DrawStringDecal(olc::vi2d(game->ScreenWidth() / 2 - 14.5f * 12, game->ScreenHeight() / 2 + 16), "Are you sure you want to quit?", olc::YELLOW, {1.5f, 1.5f});

        game->DrawStringDecal(olc::vi2d(game->ScreenWidth() / 2 - 64, game->ScreenHeight() / 2 + 40), "YES", mPromptHoverColor[0], {1.5f, 1.5f});
        game->DrawStringDecal(olc::vi2d(game->ScreenWidth() / 2 + 28, game->ScreenHeight() / 2 + 40), "NO", mPromptHoverColor[1], {1.5f, 1.5f});
    }
    
}

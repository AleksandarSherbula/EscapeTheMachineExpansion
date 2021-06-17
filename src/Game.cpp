#include "Game.h"

Game::Game()
{
    sAppName = "Escape the Machine: Death Machine Returns";
}

Game::~Game()
{
    delete mainMenu;
    delete pauseMenu;
    delete ending;
    delete timeAttack;
    delete starMap;
    delete player;
    delete levels;
    delete sb;

    for (auto& obj : vObjects)
        delete obj;

    vObjects.clear();
}

bool Game::OnUserCreate()
{
    ImageAssets::get().LoadSprites();

    playerControl = true;

    state = MAIN_MENU;

    mainMenu    = new MainMenu();
    pauseMenu   = new PauseMenu();
    ending      = new Ending();
    timeAttack  = new TimeAttack();
    starMap     = new StarMap();
    player      = new Player(olc::vf2d(2.0f, 17.0f), olc::BLUE);
    levels      = new Level();
    sb          = new Scoreboard();

    starMap->Create();    

    return true;
}

bool Game::OnUserUpdate(float fElapsedTime)
{
    if (state != ENDING)
        starMap->Update(fElapsedTime);

    switch (state)
    {    
    case MAIN_MENU: mainMenu->Update(fElapsedTime);  break;
    case GAME:      Update(fElapsedTime);             break;    
    case ENDING:    ending->Update(fElapsedTime);    break;
    }    

    if (state != ENDING)
        starMap->Draw();


    return true;
}

void Game::Update(float fElapsedTime)
{
    if (GetKey(olc::ESCAPE).bPressed)
        pauseMenu->bIsOn = true;

    if (pauseMenu->bIsOn)
        pauseMenu->Update(fElapsedTime, levels, timeAttack, player->initPosition);
    else
    {
        bool OutOfBoundary = 
            player->position.x < 0.0f || player->position.x > ScreenWidth() / 16 ||
            player->position.y < 0.0f || player->position.y > ScreenHeight() / 16;

        if (mode == TIME_ATTACK && timeAttack->timeRunning)
        {
            timeAttack->Update();
        }

        if (OutOfBoundary || mainMenu->gameStart)
        {
            levels->Load(vObjects, mainMenu->strMode, mainMenu->gameStart);

            if (mainMenu->gameStart)
                mainMenu->gameStart = false;
        }

        player->Behaviour(fElapsedTime);

        for (auto& obj : vObjects)
        {
            if (player->position.x + 0.5f > obj->position.x && player->position.y + 0.5f > obj->position.y &&
                player->position.x + 0.5f < obj->position.x + 1.0f && player->position.y + 0.5f < obj->position.y + 1.0f)
                obj->Behaviour(fElapsedTime);
        }
        if (levels->GetTile(player->position.x + 0.5f, player->position.y + 0.5f) == 'C')
        {
            if (mode == TIME_ATTACK)
                timeAttack->timeRunning = false;
            playerControl = false;
            if (player->size.x <= 0.0f &&
                player->size.y <= 0.0f)                    
            {
                vObjects.clear();
                state = ENDING;
            }
        }

        if (levels->bHasFloorSwap)
            levels->Update(fElapsedTime);
    } 

    Clear(olc::Pixel(0, 0, 30));

    starMap->Draw();

    levels->Draw();

    for (auto& obj : vObjects)
        obj->Draw();

    player->Draw();

    if (pauseMenu->bIsOn)
        pauseMenu->Print();

    if (mode == TIME_ATTACK)
        timeAttack->PrintTime();
}

void Game::DrawStringDecalXAligned(const std::string& sText, const olc::vi2d& offset, const olc::Pixel& col, const olc::vf2d& scale)
{
    DrawStringDecal(olc::vi2d(ScreenWidth() / 2 - sText.size() / 2 * (scale.x * 8) + offset.x, offset.y), sText, col, scale);
}

void Game::Restart()
{
    if (state == MAIN_MENU || state == ENDING ||
        mode == SURVIVAL || pauseMenu->bIsOn)
    {
        levels->nID = 1;
        levels->strID = "1";
        player->position = { 2.0f, 17.0f };
        player->initPosition = player->position;
        
        if (levels->colorFloorLockLevel)
        {
            levels->colorFloorLockLevel = false;
            ColorFloor::sClearedFloorCount = 0;
        }
        if (levels->branched)
            levels->branched = false;

        if (state == MAIN_MENU || state == ENDING)
            mainMenu->Reset();
    }

    player->position = player->initPosition;
    if (state != ENDING)
        levels->Load(vObjects, mainMenu->strMode, false);
    player->size = { 16.0f, 16.0f };

    if (state == MAIN_MENU && mode == TIME_ATTACK)
        timeAttack->timeRunning = true;
    
    if (levels->colorFloorLockLevel)
        ColorFloor::sClearedFloorCount = ColorFloor::sStartingClearedFloorCount;
    player->color = olc::BLUE;
    playerControl = true;
}
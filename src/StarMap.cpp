#include "Game.h"
#include "StarMap.h"

StarMap::StarMap()
{
    
}

void StarMap::Create()
{
    for (auto& s : mStars) {
        s = { (float)(rand() % game->ScreenWidth()), (float)(rand() % game->ScreenHeight()) };
    }
}

void StarMap::Update(float fElapsedTime)
{
    for (size_t i = 0; i < mStars.size(); i++)
    {
        auto& s = mStars[i];
        auto& c = mColors[i];
        float modifier = 1.0f;
        mColors[i] = olc::WHITE;
        if (i <= mStars.size() * 0.25f) {
            modifier = 0.4f;
            c = olc::VERY_DARK_GREY;
        }
        else if (i <= mStars.size() * 0.5f) {
            modifier = 0.6f;
            c = olc::DARK_GREY;
        }
        else if (i <= mStars.size() * 0.75f) {
            modifier = 0.8f;
            c = olc::GREY;
        }

        s.y += 18.0f * fElapsedTime * modifier;

        if (s.y >= game->ScreenHeight()) {
            s = { (float)(rand() % game->ScreenWidth()), 0.0f };
        }
    }
}

void StarMap::Draw()
{
    for (int i = 0; i < mStars.size(); i++)
    {
        auto& s = mStars[i];
        auto& c = mColors[i];

        game->FillRect(s, olc::vi2d(1, 1), c);
    }
}

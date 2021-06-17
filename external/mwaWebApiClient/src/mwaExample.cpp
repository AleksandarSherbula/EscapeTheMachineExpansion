#include "Scoreboard.h"

#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#include <iostream>
#include <sstream>

class Game : public olc::PixelGameEngine
{
public:
    Game()
    {
        sAppName = "Example";
    }
    
    bool OnUserCreate() override
    {
        return true;
    }

    bool OnUserUpdate(float fElapsedTime) override
    {
        if(GetMouse(0).bHeld)
        {
            FillCircle(GetMousePos(), 10, olc::Pixel(rand() % 256,rand() % 256,rand() % 256));
        }

        if(GetMouse(1).bPressed)
        {
            FillCircle(GetMousePos(), 10, olc::BLACK);

            sb.RefreshScores();

            // cycle through and list all normal time scores
            for(int i = 0; i < sb.vNormalScores.size(); i++)
            {
                // name             item.get<0>
                // minutes          item.get<1>
                // seconds          item.get<2>
                // milliseconds     item.get<3>
                
                std::stringstream buf;
                
                buf << std::get<0>(sb.vNormalScores[i]) << ": ";
                buf << std::setfill('0') << std::setw(2) << std::get<1>(sb.vNormalScores[i]) << ":"; 
                buf << std::setfill('0') << std::setw(2) << std::get<2>(sb.vNormalScores[i]) << ".";
                buf << std::setfill('0') << std::setw(3) << std::get<3>(sb.vNormalScores[i]);

                DrawString({5, i * 8 + 5}, buf.str(), olc::BLACK);
                DrawString({4, i * 8 + 4}, buf.str());
            }

        }

        return !GetKey(olc::ESCAPE).bPressed;
    }



private:
    Scoreboard sb;

};

int main(int argc, char **argv)
{
    Game game;

    if(game->Construct(320, 240, 4, 4))
        game->Start();

    return 0;
    
    // normal/main
    // normal/survival
    // normal/time
    // encore/main
    // encore/survival
    // encore/time
    // api.IncrementCount("normal/main");

    // // refresh the counts
    // api.RefreshCounts();
    
    // // cycle through and list all counts
    // for(auto &item : api.mCounts)
    // {
    //     std::cout << item.first << ": " << item.second << std::endl;
    // }
    // std::cout << std::endl;

    // // access a specific counter by key
    // std::cout << std::endl << api.mCounts["normal/main"] << std::endl;
    
    
    // // Add a new time score to normal, change to encore
    // api.NewScore("normal", "Super Macho Man", 1, 0, 500);

    // // refresh the scores (both normal and encore)
    // api.RefreshScores();

    // // cycle through and list all normal time scores
    // for(auto &item : api.vNormalScores)
    // {
    //     // name             item.get<0>
    //     // minutes          item.get<1>
    //     // seconds          item.get<2>
    //     // milliseconds     item.get<3>

    //     std::cout << std::get<0>(item) << ": " << std::get<1>(item) << ":" << std::get<2>(item) << "." << std::get<3>(item) << std::endl;
    // }

    // // cycle through and list all normal time scores
    // for(auto &item : api.vEncoreScores)
    // {
    //     std::cout << std::get<0>(item) << ": " << std::get<1>(item) << ":" << std::get<2>(item) << "." << std::get<3>(item) << std::endl;
    // }

    return 0;
}

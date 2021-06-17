#ifndef STARMAP_H
#define STARMAP_H

#include <array>

#include "olcPixelGameEngine.h"

class StarMap {
public:
    StarMap();

    void Create();
    void Update(float fElapsedTime);
    void Draw();    

private:    
    std::array<olc::Pixel, 400> mColors;
    std::array<olc::vf2d, 400> mStars;
};
#endif
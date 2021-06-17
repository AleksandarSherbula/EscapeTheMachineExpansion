#include "TitleCard.h"
#include "Object.h"
#include <string>

TitleCard::TitleCard(const std::string& name)
{
    m_sprObj = new SpriteObject(name);
}

TitleCard::~TitleCard()
{
    delete m_sprObj;
}

void TitleCard::Draw(const olc::vi2d& position, const olc::Pixel& color)
{
    m_sprObj->DrawFullSprite(position, olc::Pixel(color.r, color.g, color.b, alpha));
}
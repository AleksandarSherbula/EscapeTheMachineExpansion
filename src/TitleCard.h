#ifndef TITLECARD_H
#define TITLECARD_H

#include "Object.h"

class TitleCard
{
private:
	SpriteObject* m_sprObj;
public:
	float alpha = 0.0f;
public:
	TitleCard(const std::string& name);
	~TitleCard();

	void Draw(const olc::vi2d& position, const olc::Pixel& color);
};

#endif
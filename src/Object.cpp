#include "Game.h"
#include "Object.h"

#include <thread>

ImageAssets::ImageAssets()
{

}

ImageAssets::~ImageAssets()
{
	delete Sprite["KeyLock"];
	delete Sprite["Switch"];
	delete Sprite["ColorSwitches"];
	delete Sprite["BridgeSwitch"];
	delete Sprite["Subtitle"];
	delete Sprite["Cloud"];
	delete Sprite["Mountains"];
	delete Sprite["Grass"];
	delete Sprite["Title"];
	delete Sprite["Victory"];
	delete Sprite["LPOguy"];
	delete Sprite["Encore"];
	delete Sprite["Survival"];
}				  

void ImageAssets::LoadSprites()
{
	auto load = [&](std::string name, std::string filename)
	{
		olc::Sprite* s = new olc::Sprite(filename);
		Sprite[name] = s;		
	};

	load("KeyLock", "assets/images/KeyLocks.png");
	load("Switch", "assets/images/Switches.png");
	load("ColorSwitches", "assets/images/ColorSwitches.png");
	load("BridgeSwitch", "assets/images/Switches.png");
	load("Subtitle", "assets/images/Subtitle.png");
	load("Cloud", "assets/images/Cloud.png");
	load("Mountains", "assets/images/Mountains.png");
	load("Grass", "assets/images/Grass.png");
	load("Title", "assets/images/Title.png");
	load("Victory", "assets/images/Victory.png");
	load("LPOguy", "assets/images/LPOguy.png");	
	load("Encore", "assets/images/EncoreVictory32x32.png");	
	load("Survival", "assets/images/SurvivalVictory32x32.png");	
}

Object::Object()
{
	size = { 16, 16 };
	color = olc::WHITE;
}

void Object::Draw()
{
	game->FillRectDecal(position * 16, size, color);
}

void Object::Draw(const olc::vi2d& position, const olc::Pixel& color, const olc::vf2d& size)
{
	game->FillRectDecal(position, size, color);
}

Object::Object(const olc::vf2d& position, const olc::Pixel& color, const olc::vf2d& size)
{
	this->position = position;
	this->size = size;
	this->color = color;
}

SpriteObject::SpriteObject()
	: Object()
{
}

SpriteObject::SpriteObject(const std::string& name)
	: Object()
{
	mSprite = ImageAssets::get().GetSprite(name);
	mDecal = new olc::Decal(mSprite);
}

SpriteObject::~SpriteObject()
{
	delete mDecal;
}

void SpriteObject::Behaviour(float fElapsedTime)
{
}

void SpriteObject::Draw()
{
	game->DrawPartialDecal(position * size, size, mDecal, sprSheetOffset * size, size, color);
}

void SpriteObject::Draw(const olc::vi2d& position, const olc::Pixel& color, const olc::vf2d& size)
{
	game->DrawPartialDecal(position, size, mDecal, sprSheetOffset * size, size, color);
}

void SpriteObject::DrawFullSprite()
{	
	game->DrawDecal(position, mDecal, { 1.0f, 1.0f }, color);
}

void SpriteObject::DrawFullSprite(const olc::vi2d& position, const olc::Pixel& color)
{
	game->DrawDecal(position, mDecal, { 1.0f, 1.0f }, color);
}

olc::Sprite* ImageAssets::GetSprite(const std::string& name)
{
	return Sprite[name];
}

Player::Player()
{
}

Player::Player(const olc::vf2d& position, const olc::Pixel& color, const olc::vi2d& size)
	: Object(position, color, size)
{
	initPosition = position;
}

void Player::Behaviour(float fElapsedTime)
{
	velocity = {0.0f, 0.0f};	

	if (!game->playerControl)
	{
		Death(fElapsedTime);
	}
	else 
	{
		if (game->GetKey(olc::A).bHeld || game->GetKey(olc::LEFT).bHeld)	velocity.x = -mSpeed;
		if (game->GetKey(olc::D).bHeld || game->GetKey(olc::RIGHT).bHeld)	velocity.x =  mSpeed;
		if (game->GetKey(olc::W).bHeld || game->GetKey(olc::UP).bHeld)	velocity.y = -mSpeed;
		if (game->GetKey(olc::S).bHeld || game->GetKey(olc::DOWN).bHeld)	velocity.y =  mSpeed;

		if (game->GetKey(olc::SHIFT).bHeld)
			mSpeed = 15.0f;
		else
			mSpeed = 7.5f;		

		if (game->levels->GetTile(position + 0.5f) == '.' || game->levels->GetTile(position + 0.5f) == '-')
		{			
			game->playerControl = false;			
		}
	}

	olc::vf2d newPlayerPos = position + velocity * fElapsedTime;

	if (velocity.x < 0.0f)
	{
		if (game->levels->IsSolid(olc::vi2d(newPlayerPos.x, position.y)) || game->levels->IsSolid(olc::vi2d(newPlayerPos.x, position.y + 0.9f)))
		{
			newPlayerPos.x = (int)newPlayerPos.x + 1;
			velocity.x = 0;
		}
	}
	else
	{
		if (game->levels->IsSolid(olc::vi2d(newPlayerPos.x + 1.0f, position.y)) || game->levels->IsSolid(olc::vi2d(newPlayerPos.x + 1.0f, position.y + 0.9f)))
		{
			newPlayerPos.x = (int)newPlayerPos.x;
			velocity.x = 0;
		}
	}
	
	if (velocity.y < 0.0f)
	{
		if (game->levels->IsSolid(olc::vi2d(position.x, newPlayerPos.y)) || game->levels->IsSolid(olc::vi2d(position.x + 0.9f, newPlayerPos.y )))
		{
			newPlayerPos.y = (int)newPlayerPos.y + 1;
			velocity.y = 0;
		}
	}
	else
	{
		if (game->levels->IsSolid(olc::vi2d(position.x, newPlayerPos.y + 1.0f)) || game->levels->IsSolid(olc::vi2d(position.x + 0.9f, newPlayerPos.y + 1.0f)))
		{
			newPlayerPos.y = (int)newPlayerPos.y;
			velocity.y = 0;
		}
	}

	if (game->levels->strID != "7" &&  game->levels->strID != "7b" &&
		game->levels->strID != "13" && game->levels->strID != "13c")
	{
		if (newPlayerPos.x < 0.0f && velocity.x < 0.0f)
		{
			newPlayerPos.x = (int)newPlayerPos.x;
			velocity.x = 0;
		}
	}
	
	if (game->levels->strID != "7c" && game->levels->strID != "13")
	{
		if (newPlayerPos.y + 1 > game->ScreenHeight() / 16 && velocity.y > 0.0f)
		{
			newPlayerPos.y = (int)newPlayerPos.y;
			velocity.y = 0;
		}
	}	

	position = newPlayerPos;	
}

void Player::Death(float fElapsedTime)
{
	for (int x = 0; x < game->levels->width; x++)
		for (int y = 0; y < game->levels->height; y++)
		{
			if (position.x + 0.5f >= x && position.x + 0.5f < x + 1.0f &&
				position.y + 0.5f >= y && position.y + 0.5f < y + 1.0f &&
				(game->levels->GetTile(x, y) == '.' || game->levels->GetTile(x, y) == '-' ||
				 game->levels->GetTile(x, y) == 'R' || game->levels->GetTile(x, y) == 'G' ||
				 game->levels->GetTile(x, y) == 'B'))
			{
				position.x = x;
				position.y = y;
			}
		}

	if (size.x > 0.0f &&
		size.y > 0.0f)
	{
		size.x -= 10 * fElapsedTime;
		size.y -= 10 * fElapsedTime;
	}
	else
	{
		mWaitForRestart += fElapsedTime;
		
		if (mWaitForRestart > 2.0f)
		{
			game->playerControl = true;
			size = { 16.0f, 16.0f };
			mWaitForRestart = 0.0f;
			game->Restart();
		}	
	}
}

KeySwitch::KeySwitch(const olc::vi2d& position, const olc::vi2d& sprSheetOffset, const char keyLockIndex)
	: SpriteObject("Switch")
{
	this->position = position;
	mKeyLockIndex = keyLockIndex;
	this->sprSheetOffset = sprSheetOffset;
}

KeySwitch::~KeySwitch()
{
}

void KeySwitch::Behaviour(float fElapsedTime)
{
	for (int x = 0; x < game->levels->width; x++)
		for (int y = 0; y < game->levels->height; y++)
		{
			if (game->levels->GetTile(olc::vi2d(x, y)) == mKeyLockIndex)
			{
				game->levels->SetTile(olc::vi2d(x, y), '0');
			}
		}
	sprSheetOffset.x = 1;
}

ColorSwitch::ColorSwitch(const olc::vi2d& position, const olc::vi2d& sprSheetOffset)
	: SpriteObject("ColorSwitches")
{
	this->position = position;
	this->sprSheetOffset = sprSheetOffset;
}

void ColorSwitch::Behaviour(float fElapsedTime)
{
	if (game->levels->GetTile(position) == 'r')
		game->player->color = olc::RED;
	else if (game->levels->GetTile(position) == 'g')
		game->player->color = olc::GREEN;
	else if (game->levels->GetTile(position) == 'b')
		game->player->color = olc::BLUE;
}

int ColorFloor::sStartingClearedFloorCount = 0;
int ColorFloor::sClearedFloorCount = 0;
int ColorFloor::sMaxClearedFloorCount = 30;

ColorFloor::ColorFloor(const olc::vi2d& position, const olc::Pixel& color)
	: Object()
{
	this->position = position;
	this->color = color;
}

void ColorFloor::Behaviour(float fElapsedTime)
{
	if (game->levels->colorFloorLockLevel && game->content == ENCORE)
	{
		if (sClearedFloorCount >= sMaxClearedFloorCount)
		{
			for (int x = 0; x < game->levels->width; x++)
				for (int y = 0; y < game->levels->height; y++)
				{
					if (game->levels->GetTile(x, y) == 'L')
					{
						game->levels->SetTile(x, y, '0');
					}
				}
		}		
	}

	if ((color == olc::RED && game->player->color == olc::RED) ||
		(color == olc::GREEN && game->player->color == olc::GREEN) ||
		(color == olc::BLUE && game->player->color == olc::BLUE))
	{
		game->levels->SetTile(game->player->position + 0.5f, '0');		
		color = olc::GREY;
		mEdgeColor = olc::GREY;

		if (game->levels->colorFloorLockLevel)
			sClearedFloorCount++;
	}
	else if ((game->player->color == olc::RED || game->player->color == olc::GREEN || game->player->color == olc::BLUE) && color != olc::GREY)
	{
		game->playerControl = false;			
	}	
}

void ColorFloor::Draw()
{
	game->FillRectDecal(position * size, size, color);
	game->FillRectDecal(position * size, olc::vi2d(16, 1), mEdgeColor);
	game->FillRectDecal(position * size, olc::vi2d(1, 16), mEdgeColor);
	game->FillRectDecal(olc::vi2d(position.x * size.x, position.y * size.y + 15), olc::vi2d(16, 1), mEdgeColor);
	game->FillRectDecal(olc::vi2d(position.x * size.x + 15, position.y * size.y), olc::vi2d(1, 16), mEdgeColor);
}

void ColorFloor::Draw(const olc::vi2d& position, const olc::Pixel& color)
{
	game->FillRectDecal(position * 16, olc::vi2d(16, 16), color);
	game->FillRectDecal(position * 16, olc::vi2d(16, 1), olc::WHITE);
	game->FillRectDecal(position * 16, olc::vi2d(1, 16), olc::WHITE);
	game->FillRectDecal(olc::vi2d(position.x * 16, position.y * 16 + 15), olc::vi2d(16, 1), olc::WHITE);
	game->FillRectDecal(olc::vi2d(position.x * 16 + 15, position.y * 16), olc::vi2d(1, 16), olc::WHITE);
}

BrigdeSwitch::BrigdeSwitch(const olc::vi2d& position)
	: SpriteObject("Switch")
{
	this->position = position;
	sprSheetOffset = { 0, 2 };
}

void BrigdeSwitch::Behaviour(float fElapsedTime)
{
	for (int x = 0; x < game->levels->width; x++)
		for (int y = 0; y < game->levels->height; y++)
		{
			if (game->levels->GetTile(olc::vi2d(x, y)) == '-')
			{
				game->levels->SetTile(olc::vi2d(x, y), '0');

			}
		}
	sprSheetOffset.x = 1;
}
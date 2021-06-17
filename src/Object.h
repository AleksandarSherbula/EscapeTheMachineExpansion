#ifndef OBJECT_H
#define OBJECT_H

#include <memory>

#include "olcPixelGameEngine.h"

class ImageAssets
{
public:
	static ImageAssets& get()
	{
		static ImageAssets me;
		return me;
	}

	ImageAssets(ImageAssets const&) = delete;
	void operator=(ImageAssets const&) = delete;

	std::map<std::string, olc::Sprite*> Sprite;

	void LoadSprites();

	olc::Sprite* GetSprite(const std::string& name);
private:
	ImageAssets();
	~ImageAssets();
};

class Object
{
public:
	olc::vf2d position;
	olc::vf2d size;
	olc::Pixel color;
public:
	Object();
	Object(const olc::vf2d& position, const olc::Pixel& color = olc::WHITE, const olc::vf2d& size = { 16.0f, 16.0f });

	virtual void Behaviour(float fElapsedTime) {}

	virtual void Draw();
	virtual void Draw(const olc::vi2d& position, const olc::Pixel& color = olc::WHITE, const olc::vf2d& size = { 16.0f, 16.0f });
};

class Player : public Object
{
private:
	float mWaitForRestart = 0.0f;
	float mSpeed;
public:
	olc::vf2d initPosition;
	olc::vf2d velocity;
public:
	Player();
	Player(const olc::vf2d& position, const olc::Pixel& color = olc::WHITE, const olc::vi2d& size = { 16, 16 });

	void Behaviour(float fElapsedTime) override;

	void Death(float fElapsedTime);
};

class SpriteObject : public Object
{
public:
	olc::vi2d sprSheetOffset;
private:
	olc::Sprite* mSprite = nullptr;
	olc::Decal* mDecal = nullptr;
public:
	SpriteObject();
	SpriteObject(const std::string& name);
	~SpriteObject();

	void Behaviour(float fElapsedTime) override;


	void Draw() override;	
	void Draw(const olc::vi2d& position, 
			  const olc::Pixel& color = olc::WHITE, 
			  const olc::vf2d& size = { 16, 16 }) override;

	void DrawFullSprite();
	void DrawFullSprite(const olc::vi2d& position,
		const olc::Pixel& color = olc::WHITE);
};

class KeySwitch : public SpriteObject
{
public:
	KeySwitch(const olc::vi2d& position, const olc::vi2d& sprSheetOffset, const char keyLockIndex);
	~KeySwitch();
	
	void Behaviour(float fElapsedTime) override;
private:
	char mKeyLockIndex;
};

class BrigdeSwitch : public SpriteObject
{
public:
	BrigdeSwitch(const olc::vi2d& position);
	void Behaviour(float fElapsedTime) override;
};

class ColorSwitch : public SpriteObject
{
public:
	ColorSwitch(const olc::vi2d& position, const olc::vi2d& sprSheetOffset);
	void Behaviour(float fElapsedTime) override;
};

class ColorFloor : public Object
{
public:
	static int sStartingClearedFloorCount;
	static int sClearedFloorCount;
	static int sMaxClearedFloorCount;
public:
	ColorFloor(const olc::vi2d& position, const olc::Pixel& color);
	void Behaviour(float fElapsedTime) override;

	static void Draw(const olc::vi2d& position, const olc::Pixel& color);
	void Draw() override;
private:
	olc::Pixel mEdgeColor = olc::WHITE;	
};
#endif
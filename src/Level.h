#ifndef LEVEL_H
#define LEVEL_H

#include <string>
#include <vector>

#include "Object.h"

class FloorSwapping
{
private:
	float mTime;
	std::vector<olc::vi2d> mIndicesPos;
	bool m_bNoFloor;
public:
	FloorSwapping();

	void Init(bool noFloor);
	void Update(float fElapsedTime);

	void AddPosition(const olc::vi2d& pos);
	void ClearPositions();
};

class Level
{
private:
	std::string m_sData;
	SpriteObject* mKeyLock;
	SpriteObject* mKeyLockBlue;
	FloorSwapping* mSwap1;
	FloorSwapping* mSwap2;
public:
	char mBranchID;
	int width;
	int height;
	int nID;
	bool bHasFloorSwap;
	bool colorFloorLockLevel;
	bool branched;
	std::string strID;
	olc::vi2d tileSize;
public:
	Level();
	~Level();

	void Update(float fElapsedTime);

	char GetTile(int x, int y);
	char GetTile(const olc::vi2d& id);

	void SetTile(int x, int y, char c);
	void SetTile(const olc::vi2d& id, char c);

	void Load(std::vector<Object*>& obj, const std::string& mode, bool gameStart);

	bool IsSolid(const olc::vi2d& id);

	void Draw();
private:
	void ReadFile(const std::string& filepath);
	void ChangeLevel();
	void Reposition();
};

#endif // !LEVEL_H

#include "Game.h"
#include "Level.h"

#include <sstream>
#include <fstream>

FloorSwapping::FloorSwapping()
{	
}

void FloorSwapping::Update(float fElapsedTime)
{
	if (game->playerControl)
		mTime += fElapsedTime;

	if (mTime >= 1.0f)
	{
		m_bNoFloor = !m_bNoFloor;
		mTime = 0.0f;
	}

	for (int x = 0; x < game->levels->width; x++)
		for (int y = 0; y < game->levels->height; y++)
		{
			for (int i = 0; i < mIndicesPos.size(); i++)
				if (x == mIndicesPos[i].x &&
					y == mIndicesPos[i].y)
				{					
					if (m_bNoFloor)
						game->levels->SetTile(x, y, '.');
					else
						game->levels->SetTile(x, y, '0');
				}
		}
}

void FloorSwapping::Init(bool noFloor)
{
	mTime = 0.0f;
	m_bNoFloor = noFloor;
}

void FloorSwapping::AddPosition(const olc::vi2d& pos)
{
	mIndicesPos.push_back(pos);
}

void FloorSwapping::ClearPositions()
{
	if (!mIndicesPos.empty())
		mIndicesPos.clear();
}

Level::Level()
{
	width = 0;
	height = 0;

	tileSize = { 16, 16 };

	nID = 1;
	colorFloorLockLevel = false;
	branched = false;	
	strID = (nID != 16) ? std::to_string(nID) : "final";

	bHasFloorSwap = false;
	mSwap1 = new FloorSwapping();
	mSwap2 = new FloorSwapping();

	mKeyLock = new SpriteObject("KeyLock");
	mKeyLockBlue = new SpriteObject("KeyLock");
}

Level::~Level()
{
	delete mKeyLockBlue;
	delete mKeyLock;

	delete mSwap2;
	delete mSwap1;
}

void Level::Update(float fElapsedTime)
{
	mSwap1->Update(fElapsedTime);
	mSwap2->Update(fElapsedTime);	
}

void Level::ReadFile(const std::string& filepath)
{
	std::ifstream stream(filepath);
	std::string line;	
	std::stringstream ss;

	int lineCount = 0;
	while (getline(stream, line))
	{
		width = line.size();
		ss << line;
		++lineCount;
	}

	height = lineCount;

	m_sData = ss.str();
}

char Level::GetTile(int x, int y)
{
	if (x >= 0 && x < width && y >= 0 && y < height)
		return m_sData[y * width + x];
	else
		return ' ';
}

char Level::GetTile(const olc::vi2d& id)
{
	return GetTile(id.x, id.y);
}

void Level::SetTile(int x, int y, char c)
{
	if (x >= 0 && x < width && y >= 0 && y < height)
		m_sData[y * width + x] = c;
}

void Level::SetTile(const olc::vi2d& id, char c)
{
	SetTile(id.x, id.y, c);
}

void Level::Load(std::vector<Object*>& obj, const std::string& mode, bool gameStart)
{
	if (!gameStart)
		ChangeLevel();
	
	ReadFile("assets/levels/" + mode + "/" + strID + ".lvl");
	
	for (int i = 0; i < m_sData.size(); i++)
	{
		if (m_sData[i] == 'q' || m_sData[i] == 'Q')
		{
			bHasFloorSwap = true;
			break;
		}

		if (i == m_sData.size() - 1)
			bHasFloorSwap = false;
	}
	
	if (!gameStart)
		Reposition();
	
	game->player->color = olc::BLUE;	

	if (!obj.empty())
		obj.clear();	

	if (bHasFloorSwap)
	{
		mSwap1->ClearPositions();
		mSwap2->ClearPositions();

		mSwap1->Init(true);
		mSwap2->Init(false);
	}		

	for (int x = 0; x < width; x++)
		for (int y = 0; y < height; y++)
		{
			char tileID = GetTile(olc::vi2d(x, y));
			switch (tileID)
			{
			case 'O': obj.push_back(new KeySwitch(olc::vi2d(x, y), olc::vi2d(0, 0), '|'));	break;
			case 'o': obj.push_back(new KeySwitch(olc::vi2d(x, y), olc::vi2d(0, 1), 'l'));	break;					
			case 'r': obj.push_back(new ColorSwitch(olc::vi2d(x, y), olc::vi2d(0, 0)));		break;
			case 'g': obj.push_back(new ColorSwitch(olc::vi2d(x, y), olc::vi2d(1, 0)));		break;
			case 'b': obj.push_back(new ColorSwitch(olc::vi2d(x, y), olc::vi2d(2, 0)));		break;
			case 'R': obj.push_back(new ColorFloor(olc::vi2d(x, y), olc::RED));				break;
			case 'G': obj.push_back(new ColorFloor(olc::vi2d(x, y), olc::GREEN));				break;
			case 'B': obj.push_back(new ColorFloor(olc::vi2d(x, y), olc::BLUE));				break;
			case '_': obj.push_back(new BrigdeSwitch(olc::vi2d(x, y)));						break;
			case 'q': mSwap1->AddPosition(olc::vi2d(x, y));							                        break;
			case 'Q': mSwap2->AddPosition(olc::vi2d(x, y));							                        break;
			}
		}

	
}

bool Level::IsSolid(const olc::vi2d& id)
{
	return GetTile(id) == '#' ||
		   GetTile(id) == '|' ||
		   GetTile(id) == 'l' ||
		   GetTile(id) == 'L';
}

void Level::ChangeLevel()
{
	if (game->content == GameContent::NORMAL)
	{
		if (game->player->position.x < 0.0f || game->player->position.y > game->ScreenHeight() / 16)
		{
			nID--;
		}
		else if (game->player->position.x > game->ScreenWidth() / 16 || game->player->position.y < 0.0f)
		{			
			nID++;
		}
	}
	else if (game->content == GameContent::ENCORE)
	{
		if (game->player->position.x < 0.0f)	/// GOING LEFT
		{
			if (strID == "7") 
			{
				mBranchID = 'a';
				branched = true;
			}
			else if (strID == "7b")
			{
				strID = "7";
				branched = false;
			}
			else if (strID == "13c")
			{
				mBranchID = 'a';
			}
			else if (strID == "13")
			{
				mBranchID = 'c';
				branched = true;
			}
		}
		else if (game->player->position.y < 0.0f)	//// GOING UP
		{
			if (strID == "7")
			{
				mBranchID = 'c';
				branched = true;
			}
			else if (strID == "13b")
			{
				strID = "13";
				branched = false;
			}
			else
			{
				nID++;
			}
		}
		else if (game->player->position.x > game->ScreenWidth() / 16)	/// GOING RIGHT
		{
			if (strID == "7")
			{
				mBranchID = 'b';
				branched = true;
			}
			else if (strID == "7a")
			{
				strID = "7";
				branched = false;
			}
			else if (strID == "13a")
			{
				mBranchID = 'c';
			}
			else if (strID == "13c")
			{
				strID = "13";
				branched = false;
			}
			else
			{
				nID++;
			}
		}
		else if (game->player->position.y > game->ScreenHeight() / 16)	/// GOING DOWN
		{
			if (strID == "7")
			{
				strID = "6";
				nID = 6;
			}
			else if (strID == "7c")
			{
				strID = "7";
				branched = false;
			}
			else if (strID == "13")
			{
				mBranchID = 'b';
				branched = true;
			}
		}

		if ((nID >= 4 && nID <= 6) || (nID >= 10 && nID <= 12))
			colorFloorLockLevel = true;		
	}

	if (nID >= 1 && nID <= 16)
	{
		if (branched)
			strID = (nID != 16) ? std::to_string(nID) + mBranchID : "final";
		else
			strID = (nID != 16) ? std::to_string(nID) : "final";
	}

	if (colorFloorLockLevel && game->content == ENCORE)
	{
		switch (nID)
		{
		case 4: ColorFloor::sStartingClearedFloorCount = 0;		ColorFloor::sMaxClearedFloorCount = 30;		break;
		case 5: ColorFloor::sStartingClearedFloorCount = 30;	ColorFloor::sMaxClearedFloorCount = 70;		break;
		case 6: ColorFloor::sStartingClearedFloorCount = 70;	ColorFloor::sMaxClearedFloorCount = 120;	break;
		case 7: case 13: ColorFloor::sClearedFloorCount = 0;	colorFloorLockLevel = false;				break;
		case 10: ColorFloor::sStartingClearedFloorCount = 0;	ColorFloor::sMaxClearedFloorCount = 50;		break;
		case 11: ColorFloor::sStartingClearedFloorCount = 50;	ColorFloor::sMaxClearedFloorCount = 150;	break;
		case 12: ColorFloor::sStartingClearedFloorCount = 150;	ColorFloor::sMaxClearedFloorCount = 250;	break;
		}
	}
}

void Level::Reposition()
{
	if (game->content == NORMAL)
	{
		if (game->player->position.x > game->ScreenWidth() / 16) // GOING RIGHT
		{			
			game->player->position = game->player->initPosition;
		}
		else if (game->player->position.x < 0.0f) // GOING LEFT
		{
			game->player->position = { 29.0f, 11.0f };
		}
	}
	else
	{
		if (game->player->position.x < 0.0f)							// GOING LEFT
		{
			game->player->position.x = game->ScreenWidth() / 16 - 1.0f;
			for (int y = height - 1; y >= 0; y--)
			{
				if (GetTile(game->player->position.x, y) == '0')
				{
					game->player->position.y = y;
					break;
				}
			}
		}
		else if (game->player->position.x >= game->ScreenWidth() / 16) // GOING RIGHT
		{
			game->player->position.x = 0.0f;
			for (int y = height - 1; y >= 0; y--)
			{
				if (GetTile(game->player->position.x, y) == '0')
				{
					game->player->position.y = y;
					break;
				}
			}			
		}
		else if (game->player->position.y < 0.0f)						// GOING UP
		{
			if (nID == 7)
				game->player->position = {14.0f, 19.0f};
			else
			{
				game->player->position.y = game->ScreenHeight() / 16 - 1.0f;
				for (int x = 0; x < width; x++)
				{
					if (GetTile(x, game->player->position.y) == '0')
					{
						game->player->position.x = x;
						break;
					}
				}
			}			
		}
		else if (game->player->position.y >= game->ScreenHeight() / 16)	// GOING DOWN
		{
			game->player->position.y = 0.0f;
			for (int x = 0; x < width; x++)
			{
				if (GetTile(x, game->player->position.y) == '0')
				{
					game->player->position.x = x;
					break;
				}
			}
		} 
		
		game->player->initPosition = game->player->position;
	}
	
}

void Level::Draw()
{
	for (int x = 0; x < width; x++)
		for (int y = 0; y < height; y++)
		{
			char tileID = GetTile(x, y);
			switch (tileID)
			{
			case '#': game->FillRectDecal(olc::vi2d(x, y) * tileSize, tileSize, olc::DARK_GREY); break;
			case '0': game->FillRectDecal(olc::vi2d(x, y) * tileSize, tileSize, olc::GREY); break;
			case '|': case 'L': mKeyLock->Draw(olc::vi2d(x, y) * tileSize); break;
			case 'l': mKeyLockBlue->sprSheetOffset = { 1 , 0 }; mKeyLockBlue->Draw(olc::vi2d(x, y) * tileSize); break;
			case 'C': game->FillRectDecal(olc::vi2d(x, y) * tileSize, tileSize, olc::CYAN); break;
			case '1': ColorFloor::Draw(olc::vi2d(x, y), olc::RED); break;
			case '2': ColorFloor::Draw(olc::vi2d(x, y), olc::GREEN); break;
			case '3': ColorFloor::Draw(olc::vi2d(x, y), olc::BLUE); break;
			}
		}

	if (nID == 1)
	{
		if (game->content == NORMAL)
		{
			game->DrawStringDecal(olc::vf2d(4.5f, 5.25f) * 16, "W,A,S,D / Arrow keys - Move", olc::DARK_CYAN, olc::vf2d(1.5f, 1.5f));
			game->DrawStringDecal(olc::vf2d(4.5f, 6.25f) * 16, "Left Shift - Sprint", olc::DARK_CYAN, olc::vf2d(1.5f, 1.5f));
		}
		else
		{
			game->DrawStringDecal(olc::vf2d(4.5f, 15.25f) * 16, "W,A,S,D / Arrow keys - Move", olc::DARK_CYAN, olc::vf2d(1.5f, 1.5f));
			game->DrawStringDecal(olc::vf2d(4.5f, 16.25f) * 16, "Left Shift - Sprint", olc::DARK_CYAN, olc::vf2d(1.5f, 1.5f));
		}
	}

	if (colorFloorLockLevel)
		game->DrawStringDecal(olc::vi2d(18, 19) * 16, std::to_string(ColorFloor::sClearedFloorCount) + "/" + std::to_string(ColorFloor::sMaxClearedFloorCount), olc::WHITE, { 2.0f, 2.0f });
}
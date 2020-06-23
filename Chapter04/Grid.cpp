// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
//
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "Grid.h"
#include "Tile.h"
#include "Tower.h"
#include "Enemy.h"
#include <algorithm>

Grid::Grid(class Game* game)
:Actor(game)
,mSelectedTile(nullptr)
{
	// 7 rows, 16 columns
	mTiles.resize(NumRows);
	for (size_t i = 0; i < mTiles.size(); i++)
	{
		mTiles[i].resize(NumCols);
	}

	// Create tiles
	for (size_t i = 0; i < NumRows; i++)
	{
		for (size_t j = 0; j < NumCols; j++)
		{
			mTiles[i][j] = new Tile(GetGame());
			mTiles[i][j]->SetPosition(Vector2(TileSize/2.0f + j * TileSize, StartY + i * TileSize));
		}
	}

	// Set start/end tiles
	GetStartTile()->SetTileState(Tile::EStart);
	GetEndTile()->SetTileState(Tile::EBase);

	// 隣接タイルリスト（上下左右に接しているタイルのリスト）の作成
	for (size_t i = 0; i < NumRows; i++)
	{
		for (size_t j = 0; j < NumCols; j++)
		{
			if (i > 0)
			{
				mTiles[i][j]->mAdjacent.push_back(mTiles[i-1][j]);
			}
			if (i < NumRows - 1)
			{
				mTiles[i][j]->mAdjacent.push_back(mTiles[i+1][j]);
			}
			if (j > 0)
			{
				mTiles[i][j]->mAdjacent.push_back(mTiles[i][j-1]);
			}
			if (j < NumCols - 1)
			{
				mTiles[i][j]->mAdjacent.push_back(mTiles[i][j+1]);
			}
		}
	}

	// パスの検索（逆順）
	FindPath(GetEndTile(), GetStartTile());
	UpdatePathTiles(GetStartTile());

	mNextEnemy = EnemyTime;
}

void Grid::SelectTile(size_t row, size_t col)
{
	// 選択が正しい（開始タイルでもゴールタイルでもない）ことを保証
	Tile::TileState tstate = mTiles[row][col]->GetTileState();
	if (tstate != Tile::EStart && tstate != Tile::EBase)
	{
		// Deselect previous one
		if (mSelectedTile)
		{
			mSelectedTile->ToggleSelect();
		}
		mSelectedTile = mTiles[row][col];
		mSelectedTile->ToggleSelect();
	}
}

void Grid::ProcessClick(int x, int y)
{
	y -= static_cast<int>(StartY - TileSize / 2);
	if (y >= 0)
	{
		x /= static_cast<int>(TileSize);
		y /= static_cast<int>(TileSize);
		if (x >= 0 && x < static_cast<int>(NumCols) && y >= 0 && y < static_cast<int>(NumRows))
		{
			SelectTile(y, x);
		}
	}
}

// A*によるパス探索の実装
bool Grid::FindPath(Tile* start, Tile* goal)
{
	for (size_t i = 0; i < NumRows; i++)
	{
		for (size_t j = 0; j < NumCols; j++)
		{
			mTiles[i][j]->g = 0.0f;
			mTiles[i][j]->mInOpenSet = false;
			mTiles[i][j]->mInClosedSet = false;
		}
	}

	std::vector<Tile*> openSet;

	// Set current node to start, and add to closed set
	Tile* current = start;
	current->mInClosedSet = true;

	do
	{
		// Add adjacent nodes to open set
		for (Tile* neighbor : current->mAdjacent)
		{
			if (neighbor->mBlocked)
			{
				continue;
			}

			// Only check nodes that aren't in the closed set
			if (!neighbor->mInClosedSet)
			{
				if (!neighbor->mInOpenSet)
				{
					// Not in the open set, so set parent
					neighbor->mParent = current;
					neighbor->h = (neighbor->GetPosition() - goal->GetPosition()).Length();
					// g(x) is the parent's g plus cost of traversing edge
					neighbor->g = current->g + TileSize;
					neighbor->f = neighbor->g + neighbor->h;
					openSet.emplace_back(neighbor);
					neighbor->mInOpenSet = true;
				}
				else
				{
					// Compute g(x) cost if current becomes the parent
					float newG = current->g + TileSize;
					if (newG < neighbor->g)
					{
						// Adopt this node
						neighbor->mParent = current;
						neighbor->g = newG;
						// f(x) changes because g(x) changes
						neighbor->f = neighbor->g + neighbor->h;
					}
				}
			}
		}

		// If open set is empty, all possible paths are exhausted
		if (openSet.empty())
		{
			break;
		}

		// Find lowest cost node in open set
		auto iter = std::min_element(openSet.begin(), openSet.end(),
									 [](Tile* a, Tile* b) {
										 return a->f < b->f;
									 });
		// Set to current and move from open to closed
		current = *iter;
		openSet.erase(iter);
		current->mInOpenSet = false;
		current->mInClosedSet = true;
	}
	while (current != goal);

	// Did we find a path?
	return (current == goal) ? true : false;
}

void Grid::UpdatePathTiles(class Tile* start)
{
	// Reset all tiles to normal (except for start/end)
	for (size_t i = 0; i < NumRows; i++)
	{
		for (size_t j = 0; j < NumCols; j++)
		{
			if (!(i == 3 && j == 0) && !(i == 3 && j == 15))
			{
				mTiles[i][j]->SetTileState(Tile::EDefault);
			}
		}
	}

	Tile* t = start->mParent;
	while (t != GetEndTile())
	{
		t->SetTileState(Tile::EPath);
		t = t->mParent;
	}
}

void Grid::BuildTower()
{
	if (mSelectedTile && !mSelectedTile->mBlocked)
	{
		mSelectedTile->mBlocked = true;
		if (FindPath(GetEndTile(), GetStartTile()))
		{
			Tower* t = new Tower(GetGame());
			t->SetPosition(mSelectedTile->GetPosition());
		}
		else
		{
			// ゴールに至るパスがなくなる場合はタワーを建てれない
			// 常にゴールへのパスを作成しておく。
			// 敵はこのパスに沿って進む
			mSelectedTile->mBlocked = false;
			FindPath(GetEndTile(), GetStartTile());
		}
		UpdatePathTiles(GetStartTile());
	}
}

void Grid::DestroyTower(int x, int y)
{
	ProcessClick(x, y);
	mSelectedTile->mBlocked = false;
	FindPath(GetEndTile(), GetStartTile());
	UpdatePathTiles(GetStartTile());
}

// 画面上の位置 x/yのタイルは最適パスに接しているか
bool Grid::IsAdjacent(int x, int y)
{
	bool adjacent = false;
	y -= static_cast<int>(StartY - TileSize / 2);
	if (y >= 0)
	{
		x /= static_cast<int>(TileSize);
		y /= static_cast<int>(TileSize);
		if (x > 0 && mTiles[y][x-1]->GetTileState() == Tile::EPath)	// 左がパス
			adjacent = true;
		if (x < (static_cast<int>(NumCols) - 1) && mTiles[y][x+1]->GetTileState() == Tile::EPath) // 右がパス
			adjacent = true;
		if (y > 0 && mTiles[y-1][x]->GetTileState() == Tile::EPath)  // 上がパス
			adjacent = true;
		if ((y < static_cast<int>(NumRows) - 1) && mTiles[y+1][x]->GetTileState() == Tile::EPath)   // 下がパス
			adjacent = true;
		if (x > 0 && y > 0 && mTiles[y-1][x-1]->GetTileState() == Tile::EPath)	// 左上がパス
			adjacent = true;
		if (y > 0 && (x < static_cast<int>(NumCols) - 1) && mTiles[y-1][x+1]->GetTileState() == Tile::EPath) // 右上がパス
			adjacent = true;
		if (x > 0 && (y < static_cast<int>(NumRows) - 1) && mTiles[y+1][x-1]->GetTileState() == Tile::EPath) // 左下がパス
			adjacent = true;
		if ((x < static_cast<int>(NumCols) - 1) && (y < static_cast<int>(NumRows) - 1)	// 右下がパスか
		     && mTiles[y+1][x+1]->GetTileState() == Tile::EPath)
			adjacent = true;
		if (x > 1 && mTiles[y][x-2]->GetTileState() == Tile::EPath)	// 2つ左がパス
			adjacent = true;
		if (x < (static_cast<int>(NumCols) - 2) && mTiles[y][x+2]->GetTileState() == Tile::EPath) // 2つ右がパス
			adjacent = true;
		if (y > 1 && mTiles[y-2][x]->GetTileState() == Tile::EPath)  // 2つ上がパス
			adjacent = true;
		if ((y < static_cast<int>(NumRows) - 2) && mTiles[y+2][x]->GetTileState() == Tile::EPath)   // 2つ下がパス
			adjacent = true;

	}
	return adjacent;
}


// 開始タイルは x/y = 0/3 とする
Tile* Grid::GetStartTile()
{
	return mTiles[3][0];
}

// ゴールタイルは x/y = 15/3 とする
Tile* Grid::GetEndTile()
{
	return mTiles[3][15];
}

void Grid::UpdateActor(float deltaTime)
{
	Actor::UpdateActor(deltaTime);

	// Is it time to spawn a new enemy?
	mNextEnemy -= deltaTime;
	if (mNextEnemy <= 0.0f)
	{
		new Enemy(GetGame());
		mNextEnemy += EnemyTime;
	}
}

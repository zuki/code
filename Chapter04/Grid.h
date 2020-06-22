// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
//
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#pragma once
#include "Actor.h"
#include <vector>

class Grid : public Actor
{
public:
	Grid(class Game* game);

	// 画面上の位置 x/yにおけるマウスクリックを処理する
	void ProcessClick(int x, int y);

	// A*を使ってパスを探す
	bool FindPath(class Tile* start, class Tile* goal);

	// タワーを作成を試みる
	void BuildTower();

	// 開始/終了タイルを取得
	class Tile* GetStartTile();
	class Tile* GetEndTile();

	void UpdateActor(float deltaTime) override;
private:
	// 指定したタイルを選択する
	void SelectTile(size_t row, size_t col);

	// パス上のタイルのテクスチャを更新する
	void UpdatePathTiles(class Tile* start);

	// 現在選択中のタイル
	class Tile* mSelectedTile;

	// グリッド内のタイルの2Dベクタ
	std::vector<std::vector<class Tile*>> mTiles;

	// 次の敵が登場するまでの時間
	float mNextEnemy;

	// グリッドの行/列数
	const size_t NumRows = 7;
	const size_t NumCols = 16;
	// 開始時のy位置（左上角）
	const float StartY = 192.0f;
	// 各タイルの幅/高さ
	const float TileSize = 64.0f;
	// 敵との時間
	const float EnemyTime = 1.5f;
};

// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
//
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "TileMapComponent.h"
#include "Actor.h"
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>

TileMapComponent::TileMapComponent(class Actor* owner, int drawOrder)
	:SpriteComponent(owner, drawOrder)
{
}

void TileMapComponent::Update(float deltaTime)
{
	SpriteComponent::Update(deltaTime);
}

void TileMapComponent::Draw(SDL_Renderer* renderer)
{
	// TileMapSetは1タイル 32x32, 1行 8タイル, 24行
	// Screenは1タイル 32x32, 1行 32タイル, 24行
	const int tileX = 32;
	const int tileY = 32;
	const int colTIle = 8;
	const int colScreen = 32;

	int idx = 0;
	for (auto tile : mTMLayout)
	{
		if (tile == -1)
		{
			idx++;
			continue;
		}

		SDL_Rect fromR;
		int fromRow = tile / colTIle;
		int fromCol = tile % colTIle;
		fromR.x = fromCol * tileX;
		fromR.y = fromRow * tileY;
		fromR.w = tileX;
		fromR.h = tileY;

		SDL_Rect toR;
		int toRow = idx / colScreen;
		int toCol = idx % colScreen;
		toR.x = toCol * tileX;
		toR.y = toRow * tileY;
		toR.w = tileX;
		toR.h = tileY;

		// Draw this background
		SDL_RenderCopyEx(renderer,
			mTileMapSet,
			&fromR,
			&toR,
			0, nullptr, SDL_FLIP_NONE
		);
		idx++;
	}
}

void TileMapComponent::SetTMLayout(const std::string& fileName)
{
	std::fstream filestream(fileName);
	if (!filestream.is_open())
	{
		SDL_Log("Failed to open file %s", fileName.c_str());
		return;
	}

	while (!filestream.eof())
	{
		// １行読み込む
		std::string buffer;
		filestream >> buffer;

		// ファイルから読み込んだ１行の文字列を区切り文字で分けてリストに追加する
		std::istringstream streambuffer(buffer); // 文字列ストリーム
		std::string token;                       // １セル分の文字列
		while (getline(streambuffer, token, ','))
		{
			// １セル分の文字列をintに変換してリストに追加する
			mTMLayout.push_back(std::stoi(token));
		}
	}
}

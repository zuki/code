// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
//
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#pragma once
#include "SpriteComponent.h"
#include <string>
#include <vector>
#include "MathLocal.h"
class TileMapComponent : public SpriteComponent
{
public:
	// Set tile map set
	void SetTileMapSet(SDL_Texture* tex) { mTileMapSet = tex; };
	// Set draw order to default to lower
	TileMapComponent(class Actor* owner, int drawOrder = 10);
	// Update/draw overriden from parent
	void Update(float deltaTime) override;
	void Draw(SDL_Renderer* renderer) override;
	// Set the textures used for the background
	void SetTMLayout(const std::string& fileName);
	// Get/set screen size and scroll speed
	void SetScreenSize(const Vector2& size) { mScreenSize = size; }
private:
	std::vector<int> mTMLayout;
	Vector2 mScreenSize;
	SDL_Texture* mTileMapSet;
};

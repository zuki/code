// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
//
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#pragma once
#include "SpriteComponent.h"
#include <vector>
class AnimSpriteComponent : public SpriteComponent
{
public:
	AnimSpriteComponent(class Actor* owner, int drawOrder = 100);
	// Update animation every frame (overriden from component)
	void Update(float deltaTime) override;
	// Set the textures used for animation
	void SetAnimTextures(const std::vector<SDL_Texture*>& textures);
	// Set the animation
	void SetAnimation(int start, int size, bool loop = true);
	// Set/get the animation FPS
	float GetAnimFPS() const { return mAnimFPS; }
	void SetAnimFPS(float fps) { mAnimFPS = fps; }
private:
	// All textures in the animation
	std::vector<SDL_Texture*> mAnimTextures;
	// Current frame displayed
	float mCurrFrame;
	// Animation frame rate
	float mAnimFPS;
	// Start frame index to current animation
	int mStartFrame;
	// Size of frames for current animation
	int mFrameSize;
	// Whether do loop frames
	bool mLoop;
};

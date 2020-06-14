// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
//
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "AnimSpriteComponent.h"
#include "MathLocal.h"

AnimSpriteComponent::AnimSpriteComponent(Actor* owner, int drawOrder)
	:SpriteComponent(owner, drawOrder)
	, mCurrFrame(0.0f)
	, mAnimFPS(24.0f)
{
}

void AnimSpriteComponent::Update(float deltaTime)
{
	SpriteComponent::Update(deltaTime);

	if (mAnimTextures.size() > 0)
	{
		// Update the current frame based on frame rate
		// and delta time
		mCurrFrame += mAnimFPS * deltaTime;

		// Wrap current frame if needed
		if (mLoop)
		{
			while (mCurrFrame >= mStartFrame + mFrameSize)
			{
				mCurrFrame -= mFrameSize;
			}
		}
		else if (mCurrFrame >= mStartFrame + mFrameSize)
		{
			mCurrFrame = mStartFrame + mFrameSize - 1;
		}
		int idx = static_cast<int>(mCurrFrame);
		SDL_Log("idx: %d", idx);
		// Set the current texture
		SetTexture(mAnimTextures[idx]);
	}
}

void AnimSpriteComponent::SetAnimation(int start, int size, bool loop)
{
	if (mAnimTextures.size() > 0)
	{
		mCurrFrame = start;
		mStartFrame = start;
		mFrameSize = size;
		mLoop = loop;
		SetTexture(mAnimTextures[start]);
	}
}

void AnimSpriteComponent::SetAnimTextures(const std::vector<SDL_Texture*>& textures)
{
	mAnimTextures = textures;
	if (mAnimTextures.size() > 0)
	{
		// Set the active texture to first frame
		mCurrFrame = 0.0f;
		SetTexture(mAnimTextures[0]);
	}
}

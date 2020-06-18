// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
//
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "Ship.h"
#include "SpriteComponent.h"
#include "InputComponent.h"
#include "CircleComponent.h"
#include "Game.h"
#include "Laser.h"
#include "Asteroid.h"

Ship::Ship(Game* game)
	:Actor(game)
	,mLaserCooldown(0.0f)
	,mMissingPeriod(0.0f)
{
	// Create a sprite component
	SpriteComponent* sc = new SpriteComponent(this, 150);
	sc->SetTexture(game->GetTexture("Assets/Ship.png"));

	// Create an input component and set keys/speed
	InputComponent* ic = new InputComponent(this);
	ic->SetForwardKey(SDL_SCANCODE_F);
	ic->SetBackKey(SDL_SCANCODE_A);
	ic->SetClockwiseKey(SDL_SCANCODE_D);
	ic->SetCounterClockwiseKey(SDL_SCANCODE_S);
	ic->SetMass(5.0f);
	ic->SetVelocity(Vector2(20.0f, 10.0f));
	ic->SetWrapping(false);

	// Create a circle component (for collision)
	mCircle = new CircleComponent(this);
	mCircle->SetRadius(30.0f);
}

void Ship::UpdateActor(float deltaTime)
{
	mLaserCooldown -= deltaTime;
	mMissingPeriod -= deltaTime;
	if (GetState() == EPaused && mMissingPeriod <= 0.0f)
	{
		SetState(EActive);
		SetPosition(Vector2(512.0f, 384.0f));
		SetRotation(Math::PiOver2); // 0.0fに設定してしまうとNearZero判定にひっかかる
	}
}

void Ship::ActorInput(const uint8_t* keyState)
{
	if (keyState[SDL_SCANCODE_SPACE] && mLaserCooldown <= 0.0f)
	{
		// レーザーの位置、回転角は作成時に指定する
		//（Actorのデフォルト値では宇宙船の位置、方向に合わない）
		new Laser(GetGame(), GetPosition(), GetRotation());

		// Reset laser cooldown (half second)
		mLaserCooldown = 0.5f;
	}

	for (auto ast : GetGame()->GetAsteroids())
	{
		if (Intersect(*mCircle, *(ast->GetCircle())))
		{
			// The first asteroid we intersect with,
			// set ourselves and the asteroid to dead
			SetState(EPaused);
			SetPosition(Vector2(-100.0f, -100.0f));
			mMissingPeriod = 1.0f;
			break;
		}
	}
}

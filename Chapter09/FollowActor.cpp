// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
//
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "FollowActor.h"
#include "MeshComponent.h"
#include "Game.h"
#include "Renderer.h"
#include "FollowCamera.h"
#include "MoveComponent.h"

FollowActor::FollowActor(Game* game)
	:Actor(game)
	,mRightButton(false)
{
	mMeshComp = new MeshComponent(this);
	mMeshComp->SetMesh(game->GetRenderer()->GetMesh("Assets/RacingCar.gpmesh"));
	SetPosition(Vector3(0.0f, 0.0f, -100.0f));

	mMoveComp = new MoveComponent(this);
	mCameraComp = new FollowCamera(this);
	mCameraComp->SnapToIdeal();
}

void FollowActor::ActorInput(const uint8_t* keys)
{
	float forwardSpeed = 0.0f;
	float angularSpeed = 0.0f;
	// wasd movement
	if (keys[SDL_SCANCODE_W])
	{
		forwardSpeed += 400.0f;
	}
	if (keys[SDL_SCANCODE_S])
	{
		forwardSpeed -= 400.0f;
	}
	if (keys[SDL_SCANCODE_A])
	{
		angularSpeed -= Math::Pi;
	}
	if (keys[SDL_SCANCODE_D])
	{
		angularSpeed += Math::Pi;
	}

	mMoveComp->SetForwardSpeed(forwardSpeed);
	mMoveComp->SetAngularSpeed(angularSpeed);

	// Adjust horizontal distance of camera based on speed
	if (!Math::NearZero(forwardSpeed))
	{
		mCameraComp->SetHorzDist(500.0f);
	}
	else
	{
		mCameraComp->SetHorzDist(350.0f);
	}

	// 課題9.1
	// マウスの動きでピッチとヨーを設定
	int x, y;
	Uint32 buttons = SDL_GetRelativeMouseState(&x, &y);
	// 右ボタンを押している間だけピッチとヨーを追加
	if (buttons & SDL_BUTTON(SDL_BUTTON_RIGHT))
	{
		const int maxMouseSpeed = 500;
		const float maxOrbitSpeed = Math::Pi * 8;
		float yawSpeed = 0.0f;
		if (x != 0)
		{
			yawSpeed = static_cast<float>(x) / maxMouseSpeed;
			yawSpeed *= maxOrbitSpeed;
		}
		mCameraComp->SetYawSpeed(-yawSpeed);

		float pitchSpeed = 0.0f;
		if (y != 0)
		{
			pitchSpeed = static_cast<float>(y) / maxMouseSpeed;
			pitchSpeed *= maxOrbitSpeed;
		}
		mCameraComp->SetPitchSpeed(pitchSpeed);

		mRightButton = true;
	}
	else if (mRightButton)
	{
		mCameraComp->SetYawSpeed(0.0f);
		mCameraComp->SetPitchSpeed(0.0f);
		mRightButton = false;
	}
}

void FollowActor::SetVisible(bool visible)
{
	mMeshComp->SetVisible(visible);
}

// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
//
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#pragma once
#include "CameraComponent.h"

class FollowCamera : public CameraComponent
{
public:
	FollowCamera(class Actor* owner);

	void Update(float deltaTime) override;

	void SnapToIdeal();

	void SetHorzDist(float dist) { mHorzDist = dist; }
	void SetVertDist(float dist) { mVertDist = dist; }
	void SetTargetDist(float dist) { mTargetDist = dist; }
	void SetSpringConstant(float spring) { mSpringConstant = spring; }

	// 課題9.1
	float GetPitchSpeed() const { return mPitchSpeed; }
	float GetYawSpeed() const { return mYawSpeed; }
	void SetPitchSpeed(float speed) { mPitchSpeed = speed; }
	void SetYawSpeed(float speed) { mYawSpeed = speed; }
private:
	Vector3 ComputeCameraPos() const;

	// Actual position of camera
	Vector3 mActualPos;
	// Velocity of actual camera
	Vector3 mVelocity;
	// Horizontal follow distance
	float mHorzDist;
	// Vertical follow distance
	float mVertDist;
	// Target distance
	float mTargetDist;
	// Spring constant (higher is more stiff)
	float mSpringConstant;
	// 課題0.1
	// カメラの上ベクトル
	Vector3 mUp;
	// ピッチの回転速度
	float mPitchSpeed;
	// ヨーの回転速度
	float mYawSpeed;

};

// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
//
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#pragma once
#include "MoveComponent.h"
#include <cstdint>

class InputComponent : public MoveComponent
{
public:
	// Lower update order to update first
	InputComponent(class Actor* owner);

	void ProcessInput(const uint8_t* keyState) override;

	// Getters/setters for private variables
	float GetMaxForce() const { return mMaxForce; }
	float GetMaxAngular() const { return mMaxAngularSpeed; }
	int GetForwardKey() const { return mForwardKey; }
	int GetBackKey() const { return mBackKey; }
	int GetClockwiseKey() const { return mClockwiseKey; }
	int GetCounterClockwiseKey() const { return mCounterClockwiseKey; }

	void SetMaxForce(float force) { mMaxForce = force; }
	void SetMaxAngularSpeed(float speed) { mMaxAngularSpeed = speed; }
	void SetForwardKey(int key) { mForwardKey = key; }
	void SetBackKey(int key) { mBackKey = key; }
	void SetClockwiseKey(int key) { mClockwiseKey = key; }
	void SetCounterClockwiseKey(int key) { mCounterClockwiseKey = key; }
private:
	// 進行方向の力の増減単位
	float mMaxForce;
	// 回転角の増減単位
	float mMaxAngularSpeed;
	// 進行方向に前進するためのキー
	int mForwardKey;
	// 進行方向に後進するためのキー
	int mBackKey;
	// 回転角を時計回りに回転するためのキー
	int mClockwiseKey;
	// 回転角を反時計回りに回転するためのキー
	int mCounterClockwiseKey;
};

// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
//
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#pragma once
#include "Component.h"
#include "MathLocal.h"

class MoveComponent : public Component
{
public:
	// updateOrder値が小さいほど先に更新される
	// 0割り算にならないように初期値を設定
	MoveComponent(class Actor* owner, int updateOrder = 10, float mass = 1.0f);

	void Update(float deltaTime) override;

	float GetMass() const { return mMass; }
	Vector2 GetSumOfForce() const { return mSumOfForce; }
	Vector2 GetVelocity() const { return mVelocity; }
	bool GetWrapping() const { return mWrapping; }

	void SetMass(float weight) { mMass = weight; }
	void AddForce(Vector2 force) { mSumOfForce += force; }
	void ClearForce() { mSumOfForce = Vector2::Zero; }
	void SetVelocity(Vector2 velocity) { mVelocity = velocity; }
	void SetAngularSpeed(float speed) { mAngularSpeed = speed; }
	void SetWrapping(bool wrap) { mWrapping = wrap; }
	void AddForce(Vector2 force, bool impulse);

private:
	// 質量
	float mMass;
	// 力の合計
	Vector2 mSumOfForce;
	// 定常力
	Vector2 mBaseForce;
	// 速度
	Vector2 mVelocity;
	// 回転角を制御（ラジアン/秒）
	float mAngularSpeed;
	// このゲームの最大速度
	float mMaxSpeed;
	// このゲームの最大加速度
	float mMaxAcceleration;
	// ラッピング表示の有無
	bool mWrapping;

	// 速度、加速度の絶対値の制限
	float mSignedMaxFloat(float newValue, float absoluteMax);
};

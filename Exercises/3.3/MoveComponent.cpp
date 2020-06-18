// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
//
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "MoveComponent.h"
#include "Actor.h"

MoveComponent::MoveComponent(class Actor* owner, int updateOrder, float mass)
:Component(owner, updateOrder)
,mMass(mass)
,mAngularSpeed(0.0f)
,mMaxSpeed(1000.0f)
,mMaxAcceleration(1000.0f)
,mWrapping(true)
{

}

void MoveComponent::Update(float deltaTime)
{
	Vector2 position = mOwner->GetPosition();

	// ドラッグをシミュレーションするために
	// (1) ほとんど動いていない時は静止させる
	if (Math::NearZero(mVelocity.x))
	{
		mVelocity.x = 0.0f;
	}
	if (Math::NearZero(mVelocity.y))
	{
		mVelocity.y = 0.0f;
	}

	// (2) 力が働いている時は減速させる
	if (!Math::NearZero(mSumOfForce.x))
	{
		mVelocity.x *= 0.95f;
	}
	if (!Math::NearZero(mSumOfForce.y))
	{
		mVelocity.y *= 0.95f;
	}

	// ニュートン物理学の適用
	if (mMass != 0.0f)
	{
		Vector2 acceleration;
		acceleration.x = mSignedMaxFloat(mSumOfForce.x / mMass, mMaxAcceleration);
		acceleration.y = mSignedMaxFloat(mSumOfForce.y / mMass, mMaxAcceleration);

		mVelocity.x = mSignedMaxFloat(mVelocity.x + acceleration.x * deltaTime, mMaxSpeed);
		mVelocity.y = mSignedMaxFloat(mVelocity.y + acceleration.y * deltaTime, mMaxSpeed);

		position.x += mVelocity.x * deltaTime;
		position.y += mVelocity.y * deltaTime;
	}

	// ラッピング処理
	if (mWrapping)
	{
		// (Screen wrapping code only for asteroids)
		if (position.x < 0.0f) { position.x = 1022.0f; }
		else if (position.x > 1024.0f) { position.x = 2.0f; }

		if (position.y < 0.0f) { position.y = 766.0f; }
		else if (position.y > 768.0f) { position.y = 2.0f; }
	}

	mSumOfForce = mBaseForce;

	// 新たな位置を設定
	mOwner->SetPosition(position);

	// 新たな回転角の設定
	if (!Math::NearZero(mAngularSpeed))
	{
		float rot = mOwner->GetRotation();
		rot += mAngularSpeed * deltaTime;
		mOwner->SetRotation(rot);

	}
}

void MoveComponent::AddForce(Vector2 force, bool impulse)
{
	if (!impulse)
	{
		mBaseForce = force;
	}
	mSumOfForce += force;
}

float MoveComponent::mSignedMaxFloat(float newValue, float absoluteMax)
{
	if (abs(newValue) > absoluteMax)
	{
		newValue = (newValue >=0) ? absoluteMax : -absoluteMax;
	}
	return newValue;
}
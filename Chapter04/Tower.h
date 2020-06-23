// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
//
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#pragma once
#include "Actor.h"

class Tower : public Actor
{
public:
	Tower(class Game* game);
	void UpdateActor(float deltaTime) override;

	class SpriteComponent* GetSprite() const {return mSprite; }
	// 攻撃可能時間か
	bool IsAttackTime();
	// 近くに敵がいるか
	bool ExitEnemy();
	// 攻撃
	void Attack();
	// 攻撃可能時間を設定
	void SetAttackTime();
	// 最適パスから外れたか
	bool IsUseless();
	// タワーを廃棄
	void Destroy();
private:
	class SpriteComponent* mSprite;
	class MoveComponent* mMove;
	class AIComponent* mAI;
	// 次に攻撃するまでの時間
	float mNextAttack;
	// 攻撃のタイムスパン
	const float AttackTime = 2.5f;
	// 攻撃する範囲
	const float AttackRange = 130.0f;
};

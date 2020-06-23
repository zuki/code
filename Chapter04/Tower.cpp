// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
//
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "Tower.h"
#include "SpriteComponent.h"
#include "MoveComponent.h"
#include "AIComponent.h"
#include "AIState.h"
#include "Game.h"
#include "Grid.h"
#include "Enemy.h"
#include "Bullet.h"

Tower::Tower(class Game* game)
:Actor(game)
{
	mSprite = new SpriteComponent(this, 200);
	mSprite->SetTexture(game->GetTexture("Assets/Tower.png"));

	mMove = new MoveComponent(this);
	//mMove->SetAngularSpeed(Math::Pi);

	mAI = new AIComponent(this);
	// Register states with AIComponent
	mAI->RegisterState(new AIWait(mAI));
	mAI->RegisterState(new AIAlert(mAI));
	mAI->RegisterState(new AIAttack(mAI));
	mAI->RegisterState(new AIDestroy(mAI));
	// Start in patrol state
	mAI->ChangeState("Wait");

	mNextAttack = AttackTime;
}

void Tower::UpdateActor(float deltaTime)
{
	Actor::UpdateActor(deltaTime);

	mNextAttack -= deltaTime;
}

bool Tower::IsAttackTime()
{
	return (mNextAttack <= 0.0f);
}

bool Tower::ExitEnemy()
{
	Enemy* e = GetGame()->GetNearestEnemy(GetPosition());
	return (e != nullptr);
}

void Tower::Attack()
{
	Enemy* e = GetGame()->GetNearestEnemy(GetPosition());
	if (e != nullptr)
	{
		// 自分から敵へのベクトル
		Vector2 dir = e->GetPosition() - GetPosition();
		float dist = dir.Length();
		if (dist < AttackRange)
		{
			// 敵に正対する
			SetRotation(Math::Atan2(-dir.y, dir.x));
			// 敵に縦断を放つ
			Bullet* b = new Bullet(GetGame());
			b->SetPosition(GetPosition());
			b->SetRotation(GetRotation());
		}
	}
	// 次の攻撃時間を設定
	SetAttackTime();
}

bool Tower::IsUseless()
{
	Grid* g = GetGame()->GetGrid();
	return !g->IsAdjacent(GetPosition().x, GetPosition().y);
}

void Tower::SetAttackTime()
{
	mNextAttack += AttackTime;
}

void Tower::Destroy()
{
	Grid* g = GetGame()->GetGrid();
	g->DestroyTower(GetPosition().x, GetPosition().y);
	SetState(EDead);
}
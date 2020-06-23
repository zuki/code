// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
//
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "AIState.h"
#include "AIComponent.h"
#include "Tower.h"
#include "Enemy.h"
#include "Bullet.h"
#include "Game.h"
#include "SpriteComponent.h"
#include "Random.h"
#include <SDL/SDL_log.h>

void AIWait::Update(float deltaTime)
{
	auto t = static_cast<class Tower*>(mOwner->GetOwner());
	if (t->IsAttackTime())
	{
		mOwner->ChangeState("Alert");
	}
	if (t->IsUseless())
	{
		mOwner->ChangeState("Destroy");
	}
}

void AIWait::OnEnter()
{
	auto t = static_cast<class Tower*>(mOwner->GetOwner());
	t->GetSprite()->SetTexture(t->GetGame()->GetTexture("Assets/Tower.png"));
}

void AIWait::OnExit()
{
	//SDL_Log("Exiting %s state", GetName());
}

void AIAlert::Update(float deltaTime)
{
	auto t = static_cast<class Tower*>(mOwner->GetOwner());
	// 最も近い敵が攻撃範囲内にあれば攻撃する
	if (t->ExitEnemy())
	{
		mOwner->ChangeState("Attack");
	}
	else if (Random::GetFloat() >= 0.8f)	// 確率80%以上でWaitへ移行
	{
		// 次の攻撃時間まで待機
		t->SetAttackTime();
		mOwner->ChangeState("Wait");
	}										// 確率20%で攻撃態勢で敵を待つ
}

void AIAlert::OnEnter()
{
	auto t = static_cast<class Tower*>(mOwner->GetOwner());
	t->GetSprite()->SetTexture(t->GetGame()->GetTexture("Assets/TowerAttaking.png"));
}

void AIAlert::OnExit()
{
	//SDL_Log("Exiting %s state", GetName());
}

void AIAttack::Update(float deltaTime)
{
	auto t = static_cast<class Tower*>(mOwner->GetOwner());
	t->Attack();
	mOwner->ChangeState("Wait");
}

void AIAttack::OnEnter()
{
	//SDL_Log("Entering %s state", GetName());
}

void AIAttack::OnExit()
{
	//SDL_Log("Exiting %s state", GetName());
}

void AIDestroy::Update(float deltaTime)
{
	auto t = static_cast<class Tower*>(mOwner->GetOwner());
	t->Destroy();
}

void AIDestroy::OnEnter()
{
	//SDL_Log("Entering %s state", GetName());
}

void AIDestroy::OnExit()
{
	//SDL_Log("Exiting %s state", GetName());
}

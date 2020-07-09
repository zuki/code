// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
//
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "Game.h"
#include <algorithm>
#include <sstream>
#include <string>
#include "PhysWorld.h"
#include "Actor.h"
#include "ObjectActor.h"

Game::Game()
:mPhysWorld(nullptr)
{

}

bool Game::Initialize()
{
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
		return false;
	}
	// Create the physics world
	mPhysWorld = new PhysWorld(this);

	LoadData();

	mPhysWorld->TestSweepAndPrune(PrintActors);

	return true;
}

void Game::AddObject(ObjectActor* obj)
{
	mObjects.emplace_back(obj);
}

void Game::RemoveObject(ObjectActor* obj)
{
	auto iter = std::find(mObjects.begin(), mObjects.end(), obj);
	mObjects.erase(iter);
}

void Game::LoadData()
{
	// 作業用アクターの作成
	ObjectActor* a = nullptr;

	// 交差をチェックする物体を設置
	Vector3 min1(Vector3(0.0f, 0.0f, 0.0f));
	Vector3 max1(Vector3(40.0f, 40.0f, 40.0f));
	a = new ObjectActor(this, min1, max1);
	a->SetPosition(Vector3(20.0f, 20.0f, 20.0f));
	a->SetName("obj-1");

	Vector3 min2(Vector3(20.0f, 20.0f, 20.0f));
	Vector3 max2(Vector3(60.0f, 60.0f, 60.0f));
	a = new ObjectActor(this, min2, max2);
	a->SetPosition(Vector3(40.0f, 40.0f, 40.0f));
	a->SetName("obj-2");

	Vector3 min3(Vector3(80.0f, 20.0f, 20.0f));
	Vector3 max3(Vector3(120.0f, 60.0f, 60.0f));
	a = new ObjectActor(this, min3, max3);
	a->SetPosition(Vector3(100.0f, 40.0f, 40.0f));
	a->SetName("obj-3");

	Vector3 min4(Vector3(20.0f, 80.0f, 20.0f));
	Vector3 max4(Vector3(60.0f, 120.0f, 60.0f));
	a = new ObjectActor(this, min4, max4);
	a->SetPosition(Vector3(40.0f, 100.0f, 40.0f));
	a->SetName("obj-4");

	Vector3 min5(Vector3(20.0f, 20.0f, 80.0f));
	Vector3 max5(Vector3(60.0f, 60.0f, 120.0f));
	a = new ObjectActor(this, min5, max5);
	a->SetPosition(Vector3(40.0f, 40.0f, 100.0f));
	a->SetName("obj-5");
}

void Game::UnloadData()
{
	// Delete actors
	// Because ~Actor calls RemoveActor, have to use a different style loop
	while (!mActors.empty())
	{
		delete mActors.back();
	}
}

void Game::Shutdown()
{
	UnloadData();
	delete mPhysWorld;
	SDL_Quit();
}

void Game::AddActor(Actor* actor)
{
	mActors.emplace_back(actor);
}

void Game::RemoveActor(Actor* actor)
{
	// Is it in actors?
	auto iter = std::find(mActors.begin(), mActors.end(), actor);
	if (iter != mActors.end())
	{
		// Swap to end of vector and pop off (avoid erase copies)
		std::iter_swap(iter, mActors.end() - 1);
		mActors.pop_back();
	}
}

void Game::PrintActors(class Actor* a, class Actor* b)
{
	SDL_Log("%s and %s intersect.",
		(static_cast<ObjectActor*>(a))->GetName().c_str(),
		(static_cast<ObjectActor*>(b))->GetName().c_str());
}

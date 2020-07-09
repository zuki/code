// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
//
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#pragma once
#include <SDL/SDL.h>
#include <unordered_map>
#include <string>
#include <vector>
#include "MathLocal.h"

class Game
{
public:
	Game();
	bool Initialize();
	void Shutdown();

	void AddActor(class Actor* actor);
	void RemoveActor(class Actor* actor);

	class PhysWorld* GetPhysWorld() { return mPhysWorld; }

	// Game-specific
	void AddObject(class ObjectActor* obj);
	void RemoveObject(class ObjectActor* obj);
	std::vector<class ObjectActor*>& GetObjects() { return mObjects; }
private:
	void LoadData();
	void UnloadData();

	// All the actors in the game
	std::vector<class Actor*> mActors;

	class PhysWorld* mPhysWorld;

	// Game-specific code
	std::vector<class ObjectActor*> mObjects;

	static void PrintActors(class Actor* a, class Actor* b);

};

// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
//
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#pragma once
#include <vector>
#include <functional>
#include <unordered_map>
#include "MathLocal.h"
#include "Collision.h"

class PhysWorld
{
public:
	PhysWorld(class Game* game);

	// Used to give helpful information about collision results
	struct CollisionInfo
	{
		// Point of collision
		Vector3 mPoint;
		// Normal at collision
		Vector3 mNormal;
		// Component collided with
		class BoxComponent* mBox;
		// Owning actor of componnet
		class Actor* mActor;
	};

	// Test collisions using sweep and prune
	void TestSweepAndPrune(std::function<void(class Actor*, class Actor*)> f);

	// Add/remove box components from world
	void AddBox(class BoxComponent* box);
	void RemoveBox(class BoxComponent* box);
private:
	class Game* mGame;
	std::vector<class BoxComponent*> mBoxes;
	//課題10.2
	std::vector<class BoxComponent*> mBoxesY;
	std::vector<class BoxComponent*> mBoxesZ;
	void RemoveBoxFromVector(class BoxComponent* box, std::vector<class BoxComponent*>& boxes);
	void SortBoxes(std::vector<class BoxComponent*>& boxes, const char axis);
	void SweepAndPrune(std::vector<class BoxComponent*>& boxes,const char axis,
		std::unordered_multimap<class BoxComponent*, class BoxComponent*>&);
};

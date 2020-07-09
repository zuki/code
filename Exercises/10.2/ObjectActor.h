// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
//
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#pragma once
#include "Actor.h"
#include "MathLocal.h"
#include <string>

class ObjectActor : public Actor
{
public:
	ObjectActor(class Game* game, Vector3 min, Vector3 man);
	~ObjectActor();
	class BoxComponent* GetBox() { return mBox; }
	std::string GetName() const { return mName; }
	void SetName(const std::string& name) { mName = name; }
private:
	class BoxComponent* mBox;
	std::string mName;
};

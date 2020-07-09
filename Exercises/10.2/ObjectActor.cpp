// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
//
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "ObjectActor.h"
#include "Game.h"
#include "Collision.h"
#include "BoxComponent.h"

ObjectActor::ObjectActor(Game* game, Vector3 min, Vector3 max)
	:Actor(game)
{
	mBox = new BoxComponent(this);
	mBox->SetObjectBox(AABB(min, max));
	mBox->OnUpdateWorldTransform();
	game->AddObject(this);
}

ObjectActor::~ObjectActor()
{
	GetGame()->RemoveObject(this);
}

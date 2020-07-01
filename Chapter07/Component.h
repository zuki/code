// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
//
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#pragma once
#include <cstdint>

class Component
{
public:
	// 課題7.1 Actorが所有するComponentを区別する
	enum Type
	{
		EAudio,
		ECircle,
		EMesh,
		EMove,
		ESprite
	};
	// Constructor
	// (the lower the update order, the earlier the component updates)
	Component(class Actor* owner, int updateOrder = 100);
	// Destructor
	virtual ~Component();
	// Update this component by delta time
	virtual void Update(float deltaTime);
	// Process input for this component
	virtual void ProcessInput(const uint8_t* keyState) {}
	// Called when world transform changes
	virtual void OnUpdateWorldTransform() { }
	// 課題7.1
	void SetType(Type type) { mType = type; }
	Type GetType() { return mType; }

	int GetUpdateOrder() const { return mUpdateOrder; }
protected:
	// Owning actor
	class Actor* mOwner;
	// Update order of component
	int mUpdateOrder;
	// このコンポーネントのタイプ
	Type mType;
};

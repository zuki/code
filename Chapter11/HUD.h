// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
//
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#pragma once
#include "UIScreen.h"
#include <vector>

class HUD : public UIScreen
{
public:
	// (Lower draw order corresponds with further back)
	HUD(class Game* game);
	~HUD();

	void Update(float deltaTime) override;
	void Draw(class Shader* shader) override;

	void AddTargetComponent(class TargetComponent* tc);
	void RemoveTargetComponent(class TargetComponent* tc);

	// 課題11.3
	void SetArrowTarget(class ArrowTarget* arrow) { mArrow = arrow; }
protected:
	void UpdateCrosshair(float deltaTime);
	void UpdateRadar(float deltaTime);
	// 課題11.3
	void UpdateArrow(float deltaTime);

	class Texture* mHealthBar;
	class Texture* mRadar;
	class Texture* mCrosshair;
	class Texture* mCrosshairEnemy;
	// 課題11.2
	class Texture* mBlipTex;
	class Texture* mBlipDownTex;
	class Texture* mBlipUpTex;
	class Texture* mRadarArrow;
	// 課題11.3
	class Texture* mArrowTex;

	// All the target components in the game
	std::vector<class TargetComponent*> mTargetComps;
	// 課題11.2
	// 3次元とし、x軸とy軸はレーダー相対のblipの2Dオフセット、ｚ軸はプレーヤより上下かで -1, 0, +1 の3値とする
	std::vector<Vector3> mBlips;
	// Adjust range of radar and radius
	float mRadarRange;
	float mRadarRadius;
	// Whether the crosshair targets an enemy
	bool mTargetEnemy;
	// 課題11.3
	class ArrowTarget* mArrow;
	float mArrowAngle;
};

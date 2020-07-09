// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
//
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "PhysWorld.h"
#include <algorithm>
#include <iterator>
#include "BoxComponent.h"
//#include <SDL/SDL.h>
//#include "ObjectActor.h"

PhysWorld::PhysWorld(Game* game)
	:mGame(game)
{
}

void PhysWorld::TestSweepAndPrune(std::function<void(Actor*, Actor*)> f)
{
	// X軸で重なるboxのペアの連想配列
	std::unordered_multimap<class BoxComponent*, class BoxComponent*> lapboxesX;
	SweepAndPrune(mBoxes, 'x', lapboxesX);
	// Y軸で重なるboxのペアの連想配列
	std::unordered_multimap<class BoxComponent*, class BoxComponent*> lapboxesY;
	SweepAndPrune(mBoxesY, 'y', lapboxesY);
	// Z軸で重なるboxのペアの連想配列
	std::unordered_multimap<class BoxComponent*, class BoxComponent*> lapboxesZ;
	SweepAndPrune(mBoxesZ, 'z', lapboxesZ);
	// X軸で重なるペアについて
	for (auto boxes : lapboxesX)
	{
		auto a = boxes.first;
		auto b = boxes.second;
		// Y軸での重なりをチェック
		auto res = lapboxesY.equal_range(a);
		// キーにaがないのでY軸には重なりなし
		if (res.first == lapboxesY.end() && res.second == lapboxesY.end())
		{
			continue;
		}
		// キーにaがあったので、バリューにbがあるかチェック
		bool found = false;
		for (auto it = res.first; it != res.second; ++it)
		{
			// バリューにbあり。Y軸も重なり
			if ((*it).second == b)
			{
				found = true;
				break;
			}
		}
		// バリューにbがなかったので、Y軸には重なりなし
		if (!found)
		{
			continue;
		}
		// Z軸での重なりをチェック(ロジックはY軸と同じ)
		res = lapboxesZ.equal_range(a);
		if (res.first == lapboxesZ.end() && res.second == lapboxesZ.end())
		{
			continue;
		}
		found = false;
		for (auto it = res.first; it != res.second; ++it)
		{
			if ((*it).second == b)
			{
				found = true;
				break;
			}
		}
		// 3軸で重なり。aとbは交差するので指定の関数を実行する
		if (found)
		{
			f(a->GetOwner(), b->GetOwner());
		}
	}
}

void PhysWorld::AddBox(BoxComponent* box)
{
	mBoxes.emplace_back(box);
	mBoxesY.emplace_back(box);
	mBoxesZ.emplace_back(box);
}

void PhysWorld::RemoveBox(BoxComponent* box)
{
	RemoveBoxFromVector(box, mBoxes);
	RemoveBoxFromVector(box, mBoxesY);
	RemoveBoxFromVector(box, mBoxesZ);
}


void PhysWorld::SweepAndPrune(std::vector<class BoxComponent*>& boxes, const char axis,
	std::unordered_multimap<class BoxComponent*, class BoxComponent*>& lapboxes)
{
	SortBoxes(boxes, axis);
	for (size_t i = 0; i < boxes.size(); i++)
	{
		// Get max.x for current box
		BoxComponent* a = boxes[i];
		float max;
		if (axis == 'x')
			max = a->GetWorldBox().mMax.x;
		else if (axis == 'y')
			max = a->GetWorldBox().mMax.y;
		else
			max = a->GetWorldBox().mMax.z;

		for (size_t j = i + 1; j < boxes.size(); j++)
		{
			BoxComponent* b = boxes[j];
			float min;
			if (axis == 'x')
				min = b->GetWorldBox().mMin.x;
			else if (axis == 'y')
				min = b->GetWorldBox().mMin.y;
			else
				min = b->GetWorldBox().mMin.z;
			//SDL_Log("min: %5.1f, max: %5.1f", min, max);
			// このifは比較回数を減らすために必須
			if (min > max)
			{
				break;
			}
			else
			{
			/*
				SDL_Log("SP: %s and %s",
					(static_cast<ObjectActor*>(a->GetOwner()))->GetName().c_str(),
					(static_cast<ObjectActor*>(b->GetOwner()))->GetName().c_str());
			*/
				lapboxes.emplace(a, b);
			}
		}
	}
}

void PhysWorld::SortBoxes(std::vector<class BoxComponent*>& boxes, const char axis)
{
	// Sort by min.(axis)
	std::sort(boxes.begin(), boxes.end(),
		[axis](BoxComponent* a, BoxComponent* b) {
			if (axis == 'x')
			{
				return a->GetWorldBox().mMin.x < b->GetWorldBox().mMin.x;
			}
			else if (axis == 'y')
			{
				return a->GetWorldBox().mMin.y < b->GetWorldBox().mMin.y;
			}
			else
			{
				return a->GetWorldBox().mMin.z < b->GetWorldBox().mMin.z;
			}
	});
}

void PhysWorld::RemoveBoxFromVector(class BoxComponent* box, std::vector<class BoxComponent*>& boxes)
{
	auto iter = std::find(boxes.begin(), boxes.end(), box);
	if (iter != boxes.end())
	{
		// Swap to end of vector and pop off (avoid erase copies)
		std::iter_swap(iter, boxes.end() - 1);
		boxes.pop_back();
	}
}

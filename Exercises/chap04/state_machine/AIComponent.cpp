#include <string>
#include "Actor.h"
#include "AIComponent.h"
#include "AIState.h"
#include "SDL/SDL.h"

AIComponent::AIComponent(class Actor* owner)
:Component(owner)
,mCurrentState(nullptr)
{

}

void AIComponent::Update(float deltaTIme)
{
    if (mCurrentState != nullptr)
    {
        mCurrentState->Update(deltaTIme);
    }
}

void AIComponent::ChangeState(const std::string& name)
{
    // 現在の状態を抜け出す
    if (mCurrentState != nullptr)
    {
        mCurrentState->OnExit();
    }
    // 新しい状態を連想配列から探す
    auto iter = mStateMap.find(name);
    if (iter != mStateMap.end())
    {
        mCurrentState = iter->second;
        // 新しい状態に入る
        mCurrentState->OnEnter();
    }
    else
    {
        SDL_Log("AIState %s の状態はありません", name.c_str());
        mCurrentState = nullptr;
        mOwner->SetState(Actor::EDead);
    }
}

void AIComponent::RegisterState(class AIState* state)
{
    mStateMap.emplace(state->GetName(), state);
}
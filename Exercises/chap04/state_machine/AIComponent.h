#pragma once
#include <string>
#include <unordered_map>
#include "Actor.h"
#include "Component.h"
#include "AIState.h"

class AIComponent : public Component
{
public:
    AIComponent(class Actor* owner);

    void Update(float deltaTIme) override;
    void ChangeState(const std::string& name);

    // 新たな状態を連想配列に登録する
    void RegisterState(class AIState* state);

private:
    // 状態の名前とAIStateのインスタンスを対応付ける
    std::unordered_map<std::string, class AIState*> mStateMap;
    // 現在の状態
    class AIState* mCurrentState;
};
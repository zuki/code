#include "AIState.h"
#include "Random.h"
#include "SDL/SDL.h"

void AIPatrol::Update(float deltaTime)
{
    if (Random::GetFloat() < 0.3f)
    {
        mOwner->ChangeState("Death");
    }
    else if (Random::GetFloat() < 0.7f)
    {
        mOwner->ChangeState("Attack");
    }
    else
    {
        SDL_Log("AIPatrol: nothig occured.");
    }
}

void AIPatrol::OnEnter()
{
    SDL_Log("AIPatrol: entered.");
}

void AIPatrol::OnExit()
{
    SDL_Log("AIPatrol: exited.");
}

void AIDeath::Update(float deltaTime)
{
    if (Random::GetFloat() < 0.3f)
    {
        SDL_Log("AIDeath: really dead.");
        mOwner->ChangeState("Quit");
    }
    else
    {
        SDL_Log("AIDeath: survived as a zombie.");
    }
}

void AIDeath::OnEnter()
{
    SDL_Log("AIDeath: entered.");
}

void AIDeath::OnExit()
{
    SDL_Log("AIDeath: exited.");
}

void AIAttack::Update(float deltaTime)
{
    if (Random::GetFloat() < 0.3f)
    {
        mOwner->ChangeState("Death");
    }
    else
    {
        SDL_Log("AIAttack: nothig occured.");
    }
}

void AIAttack::OnEnter()
{
    SDL_Log("AIAttack: entered.");
}

void AIAttack::OnExit()
{
    SDL_Log("AIAttack: exited.");
}

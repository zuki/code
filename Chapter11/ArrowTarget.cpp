#include "ArrowTarget.h"
#include "Game.h"
#include "HUD.h"
#include "Renderer.h"

ArrowTarget::ArrowTarget(Game* game)
    :Actor(game)
{
    GetGame()->GetHUD()->SetArrowTarget(this);
}
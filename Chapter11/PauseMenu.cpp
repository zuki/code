// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
//
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "PauseMenu.h"
#include "Game.h"
#include "DialogBox.h"
#include "MainMenu.h"
#include <SDL/SDL.h>

PauseMenu::PauseMenu(Game* game)
	:UIScreen(game)
{
	//SDL_Log("Create PauseMenu.");
	mGame->SetState(Game::EPaused);
	SetRelativeMouseMode(false);
	SetTitle("PauseTitle");
	AddButton("ResumeButton", [this]() {
		Close();
	});
	AddButton("RestartButton", [this]() {
		new DialogBox(mGame, "RestartText",
			[this]() {
				mGame->RemoveAllActors();
				mGame->SetState(Game::ERestart);
				Close();
		});
	});
	//SDL_Log("PauseMenu created.");
}

PauseMenu::~PauseMenu()
{
	//SDL_Log("Delete PauseMenu.");
	SetRelativeMouseMode(true);
	if (mGame->GetState() == Game::EPaused)
	{
		mGame->SetState(Game::EGameplay);
	}
	//SDL_Log("PauseMenu deleted.");
}

void PauseMenu::HandleKeyPress(int key)
{
	UIScreen::HandleKeyPress(key);

	if (key == SDLK_ESCAPE)
	{
		Close();
	}
}

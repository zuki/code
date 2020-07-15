// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
//
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "DialogBox.h"
#include "Game.h"
#include "Renderer.h"
#include "MainMenu.h"
#include "UIScreen.h"
#include <SDL/SDL.h>
#include <typeinfo>

DialogBox::DialogBox(Game* game, const std::string& text,
	std::function<void()> onOK)
	:UIScreen(game)
{
	//SDL_Log("Create DialogBox.");
	// Adjust positions for dialog box
	mBGPos = Vector2(0.0f, 0.0f);
	mTitlePos = Vector2(0.0f, 100.0f);
	mNextButtonPos = Vector2(0.0f, 0.0f);

	mBackground = mGame->GetRenderer()->GetTexture("Assets/DialogBG.png");
	SetTitle(text, Vector3::Zero, 30);
	AddButton("OKButton", [this, onOK]() {
		onOK();
		Close();
	});
	AddButton("CancelButton", [this]() {
		Close();
	});
	//SDL_Log("DialogBox created.");
}

DialogBox::~DialogBox()
{
	//SDL_Log("Delete DialogBox.");
	//SDL_Log("DialogBox deleted.");
}

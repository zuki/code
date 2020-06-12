// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
//
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "Game.h"

const int thickness = 15;
const float paddleH = 100.0f;
const int ballNum = 2;

Game::Game()
:mWindow(nullptr)
,mRenderer(nullptr)
,mTicksCount(0)
,mIsRunning(true)
,mPaddle1Dir(0)
,mPaddle2Dir(0)
{

}

bool Game::Initialize()
{
	// Initialize SDL
	int sdlResult = SDL_Init(SDL_INIT_VIDEO);
	if (sdlResult != 0)
	{
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
		return false;
	}

	// Create an SDL Window
	mWindow = SDL_CreateWindow(
		"Game Programming in C++ (Chapter 1)", // Window title
		100,	// Top left x-coordinate of window
		100,	// Top left y-coordinate of window
		1024,	// Width of window
		768,	// Height of window
		0		// Flags (0 for no flags set)
	);

	if (!mWindow)
	{
		SDL_Log("Failed to create window: %s", SDL_GetError());
		return false;
	}

	//// Create SDL renderer
	mRenderer = SDL_CreateRenderer(
		mWindow, // Window to create renderer for
		-1,		 // Usually -1
		SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
	);

	if (!mRenderer)
	{
		SDL_Log("Failed to create renderer: %s", SDL_GetError());
		return false;
	}
	//
	mPaddle1Pos.x = 10.0f;
	mPaddle1Pos.y = 768.0f/2.0f;
	mPaddle2Pos.x = 999.0f;          // 1024.0f - 10.0f - 15.0f
	mPaddle2Pos.y = 768.0f/2.0f;
	for (int i=0; i<ballNum; i++)
	{
		Ball ball = {
			{1024.0f/2.0f + i * 30.f, 768.0f/2.0f - i * 30.f},
			{(i%2 ? 200.f : -200.0f), 235.0f}
		};
		mBalls.push_back(ball);
	}
	return true;
}

void Game::RunLoop()
{
	while (mIsRunning)
	{
		ProcessInput();
		UpdateGame();
		GenerateOutput();
	}
}

void Game::ProcessInput()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
			// If we get an SDL_QUIT event, end loop
			case SDL_QUIT:
				mIsRunning = false;
				break;
		}
	}

	// Get state of keyboard
	const Uint8* state = SDL_GetKeyboardState(NULL);
	// If escape is pressed, also end loop
	if (state[SDL_SCANCODE_ESCAPE])
	{
		mIsRunning = false;
	}

	// Update player 1's paddle direction based on W/S keys
	mPaddle1Dir = 0;
	if (state[SDL_SCANCODE_W])
	{
		mPaddle1Dir -= 1;
	}
	if (state[SDL_SCANCODE_S])
	{
		mPaddle1Dir += 1;
	}

	// Update player 2's paddle direction based on I/K keys
	mPaddle2Dir = 0;
	if (state[SDL_SCANCODE_I])
	{
		mPaddle2Dir -= 1;
	}
	if (state[SDL_SCANCODE_K])
	{
		mPaddle2Dir += 1;
	}
}

void Game::UpdateGame()
{
	// Wait until 16ms has elapsed since last frame
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16))
		;

	// Delta time is the diff1erence in ticks from last frame
	// (converted to seconds)
	float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;

	// Clamp maximum delta time value
	if (deltaTime > 0.05f)
	{
		deltaTime = 0.05f;
	}

	// Update tick counts (for next frame)
	mTicksCount = SDL_GetTicks();

	// Update player 1's paddle position based on direction
	if (mPaddle1Dir != 0)
	{
		mPaddle1Pos.y += mPaddle1Dir * 300.0f * deltaTime;
		// Make sure paddle doesn't move off screen!
		if (mPaddle1Pos.y < (paddleH/2.0f + thickness))
		{
			mPaddle1Pos.y = paddleH/2.0f + thickness;
		}
		else if (mPaddle1Pos.y > (768.0f - paddleH/2.0f - thickness))
		{
			mPaddle1Pos.y = 768.0f - paddleH/2.0f - thickness;
		}
	}

	// Update player 2's paddle position based on direction
	if (mPaddle2Dir != 0)
	{
		mPaddle2Pos.y += mPaddle2Dir * 300.0f * deltaTime;
		// Make sure paddle doesn't move off screen!
		if (mPaddle2Pos.y < (paddleH/2.0f + thickness))
		{
			mPaddle2Pos.y = paddleH/2.0f + thickness;
		}
		else if (mPaddle2Pos.y > (768.0f - paddleH/2.0f - thickness))
		{
			mPaddle2Pos.y = 768.0f - paddleH/2.0f - thickness;
		}
	}

	// Update each ball position based on the ball velocity
	for (auto& ball : mBalls)
	{
		ball.pos.x += ball.vel.x * deltaTime;
		ball.pos.y += ball.vel.y * deltaTime;

		// Bounce if needed
		// Did we intersect with the player 1's paddle?
		float diff1 = mPaddle1Pos.y - ball.pos.y;
		// Did we intersect with the player 2's paddle?
		float diff2 = mPaddle2Pos.y - ball.pos.y;
		// Take absolute value of diff1erence
		diff1 = (diff1 > 0.0f) ? diff1 : -diff1;
		diff2 = (diff2 > 0.0f) ? diff2 : -diff2;
		if (
			// Our y-diff1erence is small enough
			diff1 <= paddleH / 2.0f &&
			// We are in the correct x-position
			ball.pos.x <= 25.0f && ball.pos.x >= 20.0f &&
			// The ball is moving to the left
			ball.vel.x < 0.0f)
		{
			ball.vel.x *= -1.0f;
		}
		else if (
			// Our y-diff1erence is small enough
			diff2 <= paddleH / 2.0f &&
			// We are in the correct x-position
			ball.pos.x <= (1024.0f - 20.0f) && ball.pos.x >= (1024.0f - 25.0f) &&
			// The ball is moving to the left
			ball.vel.x > 0.0f)
		{
			ball.vel.x *= -1.0f;
		}
		// Did the ball go off the screen? (if so, end game)
		else if (ball.pos.x <= 0.0f || ball.pos.x >= 1024.f)
		{
			mIsRunning = false;
		}

		// Did the ball collide with the top wall?
		if (ball.pos.y <= thickness && ball.vel.y < 0.0f)
		{
			ball.vel.y *= -1;
		}
		// Did the ball collide with the bottom wall?
		else if (ball.pos.y >= (768 - thickness) &&
			ball.vel.y > 0.0f)
		{
			ball.vel.y *= -1;
		}
	}
}

void Game::GenerateOutput()
{
	// Set draw color to blue
	SDL_SetRenderDrawColor(
		mRenderer,
		0,		// R
		0,		// G
		255,	// B
		255		// A
	);

	// Clear back buffer
	SDL_RenderClear(mRenderer);

	// Draw walls
	SDL_SetRenderDrawColor(mRenderer, 255, 255, 255, 255);

	// Draw top wall
	SDL_Rect wall{
		0,			// Top left x
		0,			// Top left y
		1024,		// Width
		thickness	// Height
	};
	SDL_RenderFillRect(mRenderer, &wall);

	// Draw bottom wall
	wall.y = 768 - thickness;
	SDL_RenderFillRect(mRenderer, &wall);

	// Draw player 1's paddle
	SDL_Rect paddle1{
		static_cast<int>(mPaddle1Pos.x),
		static_cast<int>(mPaddle1Pos.y - paddleH/2),
		thickness,
		static_cast<int>(paddleH)
	};
	SDL_RenderFillRect(mRenderer, &paddle1);

	// Draw player 2's paddle
	SDL_Rect paddle2{
		static_cast<int>(mPaddle2Pos.x),
		static_cast<int>(mPaddle2Pos.y - paddleH/2),
		thickness,
		static_cast<int>(paddleH)
	};
	SDL_RenderFillRect(mRenderer, &paddle2);

	// Draw ball
	for (const auto& ball : mBalls)
	{
		SDL_Rect ballRect{
			static_cast<int>(ball.pos.x - thickness/2),
			static_cast<int>(ball.pos.y - thickness/2),
			thickness,
			thickness
		};
		SDL_RenderFillRect(mRenderer, &ballRect);
	}

	// Swap front buffer and back buffer
	SDL_RenderPresent(mRenderer);
}

void Game::Shutdown()
{
	SDL_DestroyRenderer(mRenderer);
	SDL_DestroyWindow(mWindow);
	SDL_Quit();
}

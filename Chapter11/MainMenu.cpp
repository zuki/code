#include "MainMenu.h"
#include "Game.h"
#include "DialogBox.h"
#include <SDL/SDL.h>

MainMenu::MainMenu(Game* game)
    :UIScreen(game)
{
    //SDL_Log("Create MainMenu.");
    mGame->SetState(Game::EMainMenu);
    SetRelativeMouseMode(false);
    SetTitle("MainTitle");
    AddButton("StartButton", [this]() {
        Close();
    });
    AddButton("QuitButton", [this]() {
        new DialogBox(mGame, "QuitText",
            [this]() {
                mGame->SetState(Game::EQuit);
        });
    });
    //SDL_Log("MainMenu created.");
}

MainMenu::~MainMenu()
{
    //SDL_Log("Delete MainMenu.");
    SetRelativeMouseMode(true);
    mGame->SetState(Game::EGameplay);
    //SDL_Log("MainMenu deleted.");
}

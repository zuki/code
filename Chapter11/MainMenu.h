#pragma once
#include "UIScreen.h"

class MainMenu : public UIScreen
{
public:
    MainMenu(class Game* game);
    ~MainMenu();

    void HandleKeyPress(int key) override;
};
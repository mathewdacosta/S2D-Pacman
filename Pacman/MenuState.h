#pragma once

enum class GameState
{
    MAIN_MENU,
    PLAYING,
    PAUSED,
    GAME_OVER
};

struct MenuState
{
    GameState state;

    bool pKeyDown;

    Texture2D* mainMenuBackground;
    Texture2D* overlayBackground;
    Rect* overlayRect;

    Texture2D* textTexture;
    Rect* pausedSourceRect;
    Rect* pausedDestRect;
};

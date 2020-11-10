#pragma once

// TODO: this handles game loop state and two separate menus, make this nicer
struct MenuState
{
    bool started;
    bool spaceKeyDown;
    bool paused;
    bool pKeyDown;
    
    Texture2D* background;
    Rect* rectangle;

    Texture2D* textTexture;
    Rect* pausedSourceRect;
    Rect* pausedDestRect;
    Rect* logoSourceRect;
    Rect* logoDestRect;
    Vector2* helpPosition;
};

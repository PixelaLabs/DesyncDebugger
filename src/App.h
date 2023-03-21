#pragma once

#include "Common.h"

#include "SDL3/SDL.h"

class AppUI;
class AppState;

class App
{
public:
    bool Run(AppUI& AppUI, AppState& AppState);
    bool Init(uint2 windowSize);
    void Shutdown();

private:
    SDL_Window* _window;
    SDL_Renderer* _renderer;
    SDL_Texture* _backBuffer;
    SDL_Texture* _lockableBackBuffer;
    SDL_GLContext _glContext;

    uint2 _windowSize;

    bool _showProfiler;
    bool _showScanRows;
    bool _showTargets;

    bool _quit;
};

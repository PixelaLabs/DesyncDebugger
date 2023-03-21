#pragma once

#include "SDL3/SDL.h"

class AppUI;
class AppState;

class App
{
public:
    bool Run(AppUI& AppUI, AppState& AppState);
    bool Init(uint32_t InWindowWidth, uint32_t InWindowHeight);
    void Shutdown();

private:
    SDL_Window* _window;
    SDL_Renderer* _renderer;
    SDL_Texture* _backBuffer;
    SDL_Texture* _lockableBackBuffer;
    SDL_GLContext _glContext;

    uint32_t WindowWidth;
    uint32_t WindowHeight;

    bool _showProfiler;
    bool _showScanRows;
    bool _showTargets;

    bool _quit;
};

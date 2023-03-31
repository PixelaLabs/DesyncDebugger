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
    SDL_Window* Window;
    SDL_Renderer* Renderer;
    SDL_Texture* BackBuffer;
    SDL_Texture* LockableBackBuffer;
    SDL_GLContext GLContext;

    uint32_t WindowWidth;
    uint32_t WindowHeight;

    bool ShowProfiler;
    bool ShowScanRows;
    bool ShowTargets;

    bool Quit;
};

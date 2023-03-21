#include "App.h"

#include "AppUI.h"

#include <iostream>

#include "SDL3/SDL.h"

#include "imgui.h"
#include "backends/imgui_impl_sdl3.h"
#include "backends/imgui_impl_sdlrenderer.h"

bool App::Run(AppUI& AppUI, AppState& AppState)
{
    while (!_quit)
    {
        SDL_Event event;

        if (SDL_PollEvent(&event))
        {
            ImGui_ImplSDL3_ProcessEvent(&event);

            if (event.type == SDL_EVENT_QUIT)
            {
                _quit = true;
            }

            if (event.type == SDL_EVENT_KEY_DOWN)
            {
            }
        }

        ImGui_ImplSDLRenderer_NewFrame();
        ImGui_ImplSDL3_NewFrame();
        ImGui::NewFrame();

        SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 0);
        SDL_RenderClear(_renderer);

        AppUI.DrawUI(AppState);

        ImGui::Render();
        ImGui_ImplSDLRenderer_RenderDrawData(ImGui::GetDrawData());

        SDL_RenderPresent(_renderer);
    }

    return true;
}

bool App::Init(uint32_t InWindowWidth, uint32_t InWindowHeight)
{
    SDL_SetHintWithPriority(SDL_HINT_RENDER_DRIVER, "opengl", SDL_HINT_OVERRIDE);

    if (SDL_Init(SDL_INIT_TIMER | SDL_INIT_VIDEO) != 0)
    {
        std::cout << "Failed to initialise SDL\n";
        return false;
    }

    WindowWidth = InWindowWidth;
    WindowHeight = InWindowHeight;

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    
    _window = SDL_CreateWindow("DesyncDebugger",
        WindowWidth, WindowHeight,
        SDL_WINDOW_OPENGL | SDL_RENDERER_PRESENTVSYNC | SDL_WINDOW_RESIZABLE | SDL_WINDOW_MAXIMIZED);

    if (_window == nullptr)
    {
        SDL_Log("Could not create a window: %s", SDL_GetError());
        return false;
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

    _renderer = SDL_CreateRenderer(_window, nullptr, SDL_RENDERER_ACCELERATED);

    if (_renderer == nullptr)
    {
        SDL_Log("Could not create a renderer: %s", SDL_GetError());
        return false;
    }

    SDL_RendererInfo renderInfo;
    if (SDL_GetRendererInfo(_renderer, &renderInfo) == 0)
    {
        SDL_Log("Renderer driver [%s] has been created", renderInfo.name);
    }

    _backBuffer = SDL_CreateTexture(_renderer, SDL_PIXELFORMAT_RGBA8888,
        SDL_TEXTUREACCESS_TARGET, int(InWindowWidth), int(WindowHeight));

    _lockableBackBuffer = SDL_CreateTexture(_renderer, SDL_PIXELFORMAT_RGBA8888,
        SDL_TEXTUREACCESS_STREAMING, int(InWindowWidth), int(WindowHeight));

    _glContext = SDL_GL_CreateContext(_window);

    SDL_GL_MakeCurrent(_window, _glContext);

    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    
    if (!ImGui_ImplSDL3_InitForOpenGL(_window, _glContext))
    {
        SDL_Log("Could not create OpenGL renderer for SDL2");
        return false; 
    }

    if (!ImGui_ImplSDLRenderer_Init(_renderer))
    {
        SDL_Log("Could not create OpenGL3 renderer for ImGui");
        return false;   
    }

    _quit = false;
    _showProfiler = true;
    _showScanRows = false;

    return true;
}

void App::Shutdown()
{
    ImGui_ImplSDLRenderer_Shutdown();
    ImGui_ImplSDL3_Shutdown();
    ImGui::DestroyContext();

    SDL_DestroyTexture(_lockableBackBuffer);
    SDL_DestroyTexture(_backBuffer);
    SDL_DestroyRenderer(_renderer);
    SDL_DestroyWindow(_window);

    SDL_Quit();
}

#include "App.h"

#include "AppUI.h"

#include <iostream>

#include "SDL3/SDL.h"

#include "imgui.h"
#include "implot.h"

#include "backends/imgui_impl_sdl3.h"
#include "backends/imgui_impl_sdlrenderer.h"

bool App::Run(AppUI& AppUI, AppState& AppState)
{
    while (!Quit)
    {
        SDL_Event event;

        if (SDL_PollEvent(&event))
        {
            ImGui_ImplSDL3_ProcessEvent(&event);

            if (event.type == SDL_EVENT_QUIT)
            {
                Quit = true;
            }

            if (event.type == SDL_EVENT_KEY_DOWN)
            {
            }
        }

        ImGui_ImplSDLRenderer_NewFrame();
        ImGui_ImplSDL3_NewFrame();
        ImGui::NewFrame();

        SDL_SetRenderDrawColor(Renderer, 0, 0, 0, 0);
        SDL_RenderClear(Renderer);

        AppUI.DrawUI(AppState);

        ImGui::Render();
        ImGui_ImplSDLRenderer_RenderDrawData(ImGui::GetDrawData());

        SDL_RenderPresent(Renderer);
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
    
    Window = SDL_CreateWindow("DesyncDebugger",
        WindowWidth, WindowHeight,
        SDL_WINDOW_OPENGL | SDL_RENDERER_PRESENTVSYNC | SDL_WINDOW_RESIZABLE | SDL_WINDOW_MAXIMIZED);

    if (Window == nullptr)
    {
        SDL_Log("Could not create a window: %s", SDL_GetError());
        return false;
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

    Renderer = SDL_CreateRenderer(Window, nullptr, SDL_RENDERER_ACCELERATED);

    if (Renderer == nullptr)
    {
        SDL_Log("Could not create a renderer: %s", SDL_GetError());
        return false;
    }

    SDL_RendererInfo renderInfo;
    if (SDL_GetRendererInfo(Renderer, &renderInfo) == 0)
    {
        SDL_Log("Renderer driver [%s] has been created", renderInfo.name);
    }

    BackBuffer = SDL_CreateTexture(Renderer, SDL_PIXELFORMAT_RGBA8888,
        SDL_TEXTUREACCESS_TARGET, int(InWindowWidth), int(WindowHeight));

    LockableBackBuffer = SDL_CreateTexture(Renderer, SDL_PIXELFORMAT_RGBA8888,
        SDL_TEXTUREACCESS_STREAMING, int(InWindowWidth), int(WindowHeight));

    GLContext = SDL_GL_CreateContext(Window);

    SDL_GL_MakeCurrent(Window, GLContext);

    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    
    if (!ImGui_ImplSDL3_InitForOpenGL(Window, GLContext))
    {
        SDL_Log("Could not create OpenGL renderer for SDL2");
        return false; 
    }

    if (!ImGui_ImplSDLRenderer_Init(Renderer))
    {
        SDL_Log("Could not create OpenGL3 renderer for ImGui");
        return false;   
    }

    ImPlot::CreateContext();

    Quit = false;
    ShowProfiler = true;
    ShowScanRows = false;

    return true;
}

void App::Shutdown()
{
    ImGui_ImplSDLRenderer_Shutdown();
    ImGui_ImplSDL3_Shutdown();
    ImGui::DestroyContext();

    SDL_DestroyTexture(LockableBackBuffer);
    SDL_DestroyTexture(BackBuffer);
    SDL_DestroyRenderer(Renderer);
    SDL_DestroyWindow(Window);

    SDL_Quit();
}

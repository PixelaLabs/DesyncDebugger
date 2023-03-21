#include <iostream>
#include <memory.h>

#include "SDL3/SDL.h"
#undef main

#include "App.h"

#include "AppState.h"
#include "AppUI.h"

const uint32_t AppWidth = 1920;
const uint32_t AppHeight = 1080;

void ParseCmdLineArguments(int argc, char** argv)
{
    if (argc <= 1)
    {
        return;
    }
}

int main(int argc, char** argv)
{
    App App;

    if (!App.Init(AppWidth, AppHeight))
    {
        return -1;
    }

    ParseCmdLineArguments(argc, argv);

    AppUI AppUI;
    AppState AppState;

    App.Run(AppUI, AppState);

    App.Shutdown();
}

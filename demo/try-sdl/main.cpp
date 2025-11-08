#include "Boot/Boot.h"
#include "Log/Log.h"

#define SDL_MAIN_HANDLED
#include <SDL3/SDL.h>
#include <SDL3/SDL_version.h>

int main(int argc, char** argv)
{
    Boot::LogInfo(argc, argv);
    Log::Info("SDL3 try demo 2");
    int version = SDL_GetVersion();
    int major = SDL_VERSIONNUM_MAJOR(version);
    int minor = SDL_VERSIONNUM_MINOR(version);
    int patch = SDL_VERSIONNUM_MICRO(version);
    
    Log::InfoF("SDL version: {}.{}.{}", major, minor, patch);
    return 0;
}

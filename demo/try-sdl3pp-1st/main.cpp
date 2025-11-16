#include "Boot/Boot.h"
#include "Log/Log.h"

#include <mutex>
#include <SDL3pp/SDL3pp.h>

int main(int argc, char** argv)
{
    Boot::LogHeader(argc, argv);
    Log::Info("SDL3pp try demo 1");
    int version = SDL::GetVersion();
    int major = SDL_VERSIONNUM_MAJOR(version);
    int minor = SDL_VERSIONNUM_MINOR(version);
    int patch = SDL_VERSIONNUM_MICRO(version);
    Log::Info("SDL version: {}.{}.{}", major, minor, patch);
    return 0;
}

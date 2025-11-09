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

    // 1. Инициализация SDL3
    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO)) {
        SDL_Log("SDL_Init failed: %s", SDL_GetError());
        return 1;
    }

    // 2. Создание окна
    SDL_Window *window = SDL_CreateWindow(
        "SDL3 Window",
        800, 600,
        SDL_WINDOW_RESIZABLE
    );
    if (!window) {
        SDL_Log("Window creation failed: %s", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    // 3. Создание рендерера
    SDL_Renderer *renderer = SDL_CreateRenderer(window, NULL);
    if (!renderer) {
        SDL_Log("Renderer creation failed: %s", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // 4. Основной цикл
    bool running = true;
    SDL_Event event;

    while (running) {
        Uint32 frameStart = SDL_GetTicks();

        // Обработка событий
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                running = false;
            }
            else if (event.type == SDL_EVENT_KEY_DOWN) {
                if (event.key.key == SDLK_ESCAPE) {
                    running = false;
                }
            }            
        }

        // Рендеринг
        SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255);
        SDL_RenderClear(renderer);

        // Здесь ваш код рисования
        SDL_SetRenderDrawColor(renderer, 255, 100, 100, 255);
        SDL_FRect rect = {100, 100, 200, 150};
        SDL_RenderFillRect(renderer, &rect);

        SDL_RenderPresent(renderer);

        // Ограничение FPS
        Uint32 frameTime = SDL_GetTicks() - frameStart;
        if (frameTime < 16) {
            SDL_Delay(16 - frameTime);
        }
    }

    // 5. Очистка
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    
    return 0;
}

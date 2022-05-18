#include <iostream>

#include <SDL.h>

SDL_Window *initSDL() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cout << "Failed to initialize the SDL2 library\n";
        exit(-1);
    }

    SDL_Window *window = SDL_CreateWindow("SDL2 Window",
                                          SDL_WINDOWPOS_CENTERED,
                                          SDL_WINDOWPOS_CENTERED,
                                          680, 480,
                                          SDL_WINDOW_SHOWN);

    if (!window) {
        std::cout << "Failed to create window\n";
        exit(-1);
    }

    return window;

}

int main(int argc, char *argv[]) {
    (void) argc;
    (void) argv;

    auto window = initSDL();

    bool run = true;
    while (run) {

        SDL_Event event;
        if (SDL_PollEvent(&event)) {
            switch (event.type) {
                case ::SDL_QUIT:
                    run = false;
                    break;
                default:
                    break;
            }
        }

    }

    SDL_DestroyWindow(window);

    return 0;

}
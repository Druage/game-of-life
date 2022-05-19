#include <iostream>

#include <SDL.h>

#include <GameOfLife_T.h>
#include "include/seeds.h"
#include "include/colors.h"

using GameOfLife = GameOfLife_T<100, 100>;

std::pair<SDL_Window *, SDL_Renderer *> initSDL() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cout << "Failed to initialize the SDL2 library\n";
        exit(-1);
    }

    SDL_Window *window = SDL_CreateWindow("The Game Of Life",
                                          SDL_WINDOWPOS_CENTERED,
                                          SDL_WINDOWPOS_CENTERED,
                                          640,
                                          480,
                                          SDL_WINDOW_SHOWN);

    if (!window) {
        std::cout << "Failed to create window\n";
        exit(-1);
    }

    auto *renderer = SDL_CreateRenderer(window, -1,
                                        SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE);

    return std::make_pair(window, renderer);
}

void paint(SDL_Renderer *renderer, uint32_t *pixelStreamBuffer, int width, int height) {

    auto *surface = SDL_CreateRGBSurfaceWithFormatFrom(pixelStreamBuffer,
                                                       width,
                                                       height,
                                                       sizeof(uint32_t),
                                                       static_cast<int>(sizeof(uint32_t) * width),
                                                       SDL_PIXELFORMAT_RGB888);

    if (!surface) {
        std::cout << SDL_GetError() << std::endl;
    }

    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    // clear screen
    SDL_RenderClear(renderer);

    // render texture to screen
    SDL_RenderCopy(renderer, texture, nullptr, nullptr);

    // update screen
    SDL_RenderPresent(renderer);
}

void copyToPixelBuffer(GameOfLife &gameOfLife, uint32_t *pixelStreamBuffer) {
    for (int row = 0; row < gameOfLife.rows(); ++row) {
        for (int col = 0; col < gameOfLife.cols(); ++col) {
            pixelStreamBuffer[(row * gameOfLife.cols()) + col] =
                    gameOfLife[row][col] == 1 ? COLOR_WHITE : COLOR_BLACK;
        }
    }
}

int main(int argc, char *argv[]) {
    (void) argc;
    (void) argv;

    auto [window, renderer] = initSDL();

    GameOfLife gameOfLife;
    gameOfLife.seed(INTERESTING_SEED);

    uint32_t pixelStreamBuffer[gameOfLife.width() * gameOfLife.height()];

    bool run = true;
    while (run) {

        copyToPixelBuffer(gameOfLife, pixelStreamBuffer);

        paint(renderer, pixelStreamBuffer, gameOfLife.width(), gameOfLife.width());

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

        SDL_Delay(100);
        gameOfLife.tick();
    }

    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);

    return 0;

}
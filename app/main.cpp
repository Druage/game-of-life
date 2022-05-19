#include <iostream>

#include <SDL.h>

#include <GameOfLife_T.h>

using GameOfLife = GameOfLife_T<100, 100>;

const int WIN_WIDTH = 680;
const int WIN_HEIGHT = 480;

const uint32_t COLOR_BLACK = 0x000000;
const uint32_t COLOR_WHITE = 0xFFFFFF;

SDL_Window *initSDL() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cout << "Failed to initialize the SDL2 library\n";
        exit(-1);
    }

    SDL_Window *window = SDL_CreateWindow("SDL2 Window",
                                          SDL_WINDOWPOS_CENTERED,
                                          SDL_WINDOWPOS_CENTERED,
                                          WIN_WIDTH,
                                          WIN_HEIGHT,
                                          SDL_WINDOW_SHOWN);

    if (!window) {
        std::cout << "Failed to create window\n";
        exit(-1);
    }

    return window;

}

// TODO - create a white image that can be shown on screen using the Mono pixel format
void paint(SDL_Renderer *renderer, uint32_t *pixelStreamBuffer, size_t width, size_t height) {

//    SDL_Surface *bmpSurf = SDL_CreateRGBSurfaceWithFormatFrom((void *) buffCopy,
//                                                              width,
//                                                              height,
//                                                              depth,
//                                                              depth * width,
//                                                              SDL_PIXELFORMAT_INDEX1MSB);

    auto *surface = SDL_CreateRGBSurfaceWithFormatFrom(pixelStreamBuffer,
                                                       width,
                                                       height,
                                                       sizeof(uint32_t),
                                                       sizeof(uint32_t) * width,
                                                       SDL_PIXELFORMAT_RGB888);

    if (!surface) {
        std::cout << SDL_GetError() << std::endl;
    }

    SDL_Texture *bmpTex = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    //Make a target texture to render too
    SDL_Texture *texTarget = SDL_CreateTexture(renderer,
                                               SDL_PIXELFORMAT_RGB888,
                                               SDL_TEXTUREACCESS_TARGET,
                                               WIN_WIDTH,
                                               WIN_HEIGHT);

    if (!texTarget) {
        std::cout << SDL_GetError() << std::endl;
    }
    //Now render to the texture
    SDL_SetRenderTarget(renderer, texTarget);
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, bmpTex, NULL, NULL);
    //Detach the texture
    SDL_SetRenderTarget(renderer, NULL);

    //Now render the texture target to our screen, but upside down
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texTarget, NULL, NULL);
    SDL_RenderPresent(renderer);

}

int main(int argc, char *argv[]) {
    (void) argc;
    (void) argv;

    auto window = initSDL();
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1,
                                                SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE);

    GameOfLife gameOfLife;
    gameOfLife.seed({
                            {2, 1},

                            {3, 2},

                            {1, 3},
                            {2, 3},
                            {3, 3},

                    });

    size_t pixelBuffWidth = gameOfLife.cols();
    size_t pixelBuffHeight = gameOfLife.rows();

    uint32_t pixelStreamBuffer[pixelBuffWidth * pixelBuffHeight];

    bool run = true;
    while (run) {

        for (int row = 0; row < gameOfLife.rows(); ++row) {
            for (int col = 0; col < gameOfLife.cols(); ++col) {
                pixelStreamBuffer[(row * gameOfLife.cols()) + col] =
                        gameOfLife[row][col] == 1 ? COLOR_WHITE : COLOR_BLACK;
            }
        }

        paint(renderer, pixelStreamBuffer, pixelBuffWidth, pixelBuffHeight);

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
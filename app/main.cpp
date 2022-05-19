#include <iostream>
#include <random>
#include <sstream>
#include <SDL.h>

#include <GameOfLife_T.h>
#include <fstream>
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

void fillRandomSeeds(GameOfLife &gameOfLife) {
    std::random_device dev;
    std::mt19937 rng(dev());

    std::uniform_int_distribution<std::mt19937::result_type> distRow(1, gameOfLife.rows() - 1);
    std::uniform_int_distribution<std::mt19937::result_type> distCol(1, gameOfLife.cols() - 1);
    std::uniform_int_distribution<std::mt19937::result_type> distBlob(1, 8);

    std::vector<std::pair<int, int>> createdSeeds;
    for (int i = 0; i < 15; ++i) {
        auto pair = std::make_pair(distRow(rng), distCol(rng));
        createdSeeds.emplace_back(pair);

        // Generate a blob with the starting coords.
        // Start from top left corner and trace around the image like a CRT scanline.
        auto edgesToDraw = distBlob(rng);
        for (auto e = distBlob.min(); e < edgesToDraw; ++e) {
            if (e == 1) {
                createdSeeds.emplace_back(std::make_pair(pair.first - e,  pair.second - e));
            } else if (e == 2) {
                createdSeeds.emplace_back(std::make_pair(pair.first,  pair.second - e));
            } else if (e == 3) {
                createdSeeds.emplace_back(std::make_pair(pair.first + e,  pair.second - e));
            } else if (e == 4) {
                createdSeeds.emplace_back(std::make_pair(pair.first - e,  pair.second));
            } else if (e == 5) {
                createdSeeds.emplace_back(std::make_pair(pair.first + e,  pair.second));
            } else if (e == 6) {
                createdSeeds.emplace_back(std::make_pair(pair.first - e,  pair.second + e));
            } else if (e == 7) {
                createdSeeds.emplace_back(std::make_pair(pair.first,  pair.second + e));
            } else if (e == 8) {
                createdSeeds.emplace_back(std::make_pair(pair.first + e,  pair.second + e));
            }
        }
    }

    gameOfLife.seed(createdSeeds.begin(), createdSeeds.end());

    std::stringstream jsonStringBuilder;

    jsonStringBuilder << "{\n";
    jsonStringBuilder << "\t\"seeds\": [\n";
    for (int i=0; i < createdSeeds.size(); ++i ) {
        auto seed = createdSeeds[i];
        jsonStringBuilder << "\t\t{" <<
        "\"x\": " << "\"" << seed.first << "\"" << ", "
        << "\"y\": " << "\"" << seed.second << "\""
        << "}";

        if (i < createdSeeds.size() - 1 ) {
            jsonStringBuilder << ",";
        }

        jsonStringBuilder << "\n";
    }

    jsonStringBuilder << "\t]\n";
    jsonStringBuilder << "}";

    auto jsonString = jsonStringBuilder.str();

    std::ofstream outStream("seeds.json", std::ios_base::out);

    outStream.write(jsonString.c_str(), jsonString.size());
    outStream.close();

}

int main(int argc, char *argv[]) {
    (void) argc;
    (void) argv;

    auto [window, renderer] = initSDL();

    GameOfLife gameOfLife;
    fillRandomSeeds(gameOfLife);

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
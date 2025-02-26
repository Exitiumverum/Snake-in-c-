#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

// compile code
//  g++ synth.cpp -o synth -lSDL2 -lSDL2_ttf
//./synth

// Forward declare renderer as global (not ideal but works for this example)
SDL_Renderer *renderer = nullptr;
const int SCREEN_WIDTH = 840;
const int SCREEN_HEIGHT = 620;

// Function declaration

void drawRect(int x, int y)
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // Draw shapes
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_Rect rect = {x, y, 200, 150};
    SDL_RenderFillRect(renderer, &rect);
}

// class Player
// {
//     int health = 100;
//     int appleCount = 0;
// };

int main()
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cout << "Failed to initialize the SDL2 lib: " << SDL_GetError() << "\n";
        return -1;
    }

    if (TTF_Init() < 0)
    {
        std::cout << "Failed to initialize SDL_ttf: \n"
                  << TTF_GetError() << "\n";
        SDL_Quit();
        return -1;
    }

    SDL_Window *window = SDL_CreateWindow(
        "My synth",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        SCREEN_WIDTH, SCREEN_HEIGHT,
        SDL_WINDOW_SHOWN);

    if (!window)
    {
        std::cout << "Failed to create the window: " << SDL_GetError() << "\n";
        SDL_Quit();
        return -1;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer)
    {
        std::cout << "Failed to create renderer: " << SDL_GetError() << "\n";
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return -1;
    }

    bool isVertical = true;
    bool running = true;
    SDL_Event event;

    int x = 100;
    int y = 100;

    int diff = 2;

    while (running)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                running = false;
            }

            if (event.type == SDL_KEYDOWN)
            {
                switch (event.key.keysym.sym)
                {
                case SDLK_UP:
                    isVertical = false;
                    if (diff > 0)
                        diff *= (-1);
                    break;
                case SDLK_DOWN:
                    isVertical = false;
                    if (diff < 0)
                        diff *= (-1);

                    break;
                case SDLK_RIGHT:
                    isVertical = true;
                    if (diff < 0)
                        diff *= (-1);
                    break;
                case SDLK_LEFT:
                    isVertical = true;
                    if (diff > 0)
                        diff *= (-1);
                    break;
                }
            }
        }

        if (isVertical)
            x += diff;
        else
            y += diff;

        // std::cout << diff;
        drawRect(x, y);
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();

    return 0;
}

#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <cstdlib>
#include <ctime>

// compile code
//  g++ synth.cpp -o synth -lSDL2 -lSDL2_ttf
//./synth

SDL_Renderer *renderer = nullptr;
const int SCREEN_WIDTH = 840;
const int SCREEN_HEIGHT = 620;

const int SNAKE_WIDTH = 22;
const int SNAKE_HEIGHT = 22;

const int APPLE_WIDTH = 14;
const int APPLE_HEIGHT = 14;

int snakeRgb[] = {255, 0, 0, 255};

float segmentPositions[100][2];

void changeSnakeRgb(int *array, int size)
{
    for (int i = 0; i < size; i++)
    {
        array[i] += 35;
    }
}

void drawApple(int x, int y)
{
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    SDL_Rect rect = {x, y, APPLE_WIDTH, APPLE_HEIGHT};
    SDL_RenderFillRect(renderer, &rect);
}

void drawSnake(int lives, bool isVertical, bool isPositive)
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    int gap = 30; // Define the gap between squares

    // Draw shapes
    for (int i = 0; i < lives; i++)
    {
        SDL_SetRenderDrawColor(renderer, snakeRgb[0], snakeRgb[1], snakeRgb[2], snakeRgb[3]);
        int offset = gap * i; // Calculate the offset based on the gap and segment index
        std::cout << offset;
        if (isVertical)
        {
            if (isPositive)
            {
                SDL_Rect rect = {static_cast<int>(segmentPositions[i][0] - offset), static_cast<int>(segmentPositions[i][1]), SNAKE_HEIGHT, SNAKE_WIDTH}; // Adjust Y position
                SDL_RenderFillRect(renderer, &rect);
            }
            else
            {
                SDL_Rect rect = {static_cast<int>(segmentPositions[i][0] + offset), static_cast<int>(segmentPositions[i][1] - SNAKE_HEIGHT), SNAKE_HEIGHT, SNAKE_WIDTH}; // Adjust Y position
                SDL_RenderFillRect(renderer, &rect);
            }
        }
        else
        {
            if (isPositive)
            {
                SDL_Rect rect = {static_cast<int>(segmentPositions[i][0]), static_cast<int>(segmentPositions[i][1] + offset), SNAKE_WIDTH, SNAKE_HEIGHT}; // Adjust X position
                SDL_RenderFillRect(renderer, &rect);
            }
            else
            {
                SDL_Rect rect = {static_cast<int>(segmentPositions[i][0] - SNAKE_WIDTH), static_cast<int>(segmentPositions[i][1] - offset), SNAKE_WIDTH, SNAKE_HEIGHT}; // Adjust X position
                SDL_RenderFillRect(renderer, &rect);
            }
        }
    }
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
    bool isPositive = true;
    bool running = true;
    bool isAppleEaten = true; // Initialize to true to generate the first apple

    SDL_Event event;

    int x = 100;
    int y = 100;

    int diff = 7;
    int lives = 2;

    int gap = 5;

    for (int i = 0; i < 100; i++)
    {
        segmentPositions[i][0] = x;
        segmentPositions[i][1] = y;
    }

    srand(static_cast<unsigned int>(time(0)));

    while (running)
    {
        int appleX, appleY;
        if (isAppleEaten) // Only generate a new apple if the previous one was eaten
        {
            appleX = rand() % (SCREEN_WIDTH - APPLE_WIDTH);
            appleY = rand() % (SCREEN_HEIGHT - APPLE_HEIGHT);
            isAppleEaten = false; // Reset the flag after generating a new apple
        }
        if (segmentPositions[0][0] < appleX + APPLE_WIDTH &&
            segmentPositions[0][0] + SNAKE_WIDTH > appleX &&
            segmentPositions[0][1] < appleY + APPLE_HEIGHT &&
            segmentPositions[0][1] + SNAKE_HEIGHT > appleY)
        {
            std::cout << "Apple eaten" << std::endl;
            lives++;
            isAppleEaten = true; // Set the flag to true when the apple is eaten
        }
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
                    isPositive = true;
                    if (diff > 0)
                        diff *= (-1);
                    break;
                case SDLK_DOWN:
                    isVertical = false;
                    isPositive = false;
                    if (diff < 0)
                        diff *= (-1);
                    break;
                case SDLK_RIGHT:
                    isVertical = true;
                    isPositive = true;
                    if (diff < 0)
                        diff *= (-1);
                    break;
                case SDLK_LEFT:
                    isVertical = true;
                    isPositive = false;
                    if (diff > 0)
                        diff *= (-1);
                    break;
                }
            }
        }

        SDL_Delay(16);

        // std::cout << diff;
        // Update segment positions
        for (int i = lives - 1; i > 0; i--)
        {
            segmentPositions[i][0] = segmentPositions[i - 1][0];
            segmentPositions[i][1] = segmentPositions[i - 1][1];
        }
        if (isVertical)
            segmentPositions[0][0] += diff;
        else
            segmentPositions[0][1] += diff;

        // Wrap around logic
        if (segmentPositions[0][0] + SNAKE_WIDTH >= SCREEN_WIDTH)
            segmentPositions[0][0] = 0;
        else if (segmentPositions[0][0] <= 0)
            segmentPositions[0][0] = SCREEN_WIDTH - SNAKE_WIDTH;
        if (segmentPositions[0][1] <= 0)
            segmentPositions[0][1] = SCREEN_HEIGHT - SNAKE_HEIGHT;
        else if (segmentPositions[0][1] >= SCREEN_HEIGHT)
            segmentPositions[0][1] = 0;

        // changeSnakeRgb(snakeRgb, sizeof(snakeRgb[0]));
        drawSnake(lives, isVertical, isPositive);
        if (!isAppleEaten)
            drawApple(appleX, appleY);
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();

    return 0;
}

#include <iostream>
#include <stdio.h>
#include "/opt/homebrew/opt/sdl2/include/SDL2/SDL.h"

#define sdl_ SDL_

SDL_Window *gWindow = NULL;
SDL_Surface *gScreenSurface = NULL;
SDL_Surface *gImage = NULL;

const int SCREEN_WIDTH = 1024;
// const int SCREEN_WIDTH = 1280;
// const int SCREEN_HEIGHT = 720;
const int SCREEN_HEIGHT = 1024;

enum KEY_PRESS
{
    KEY_PRESS_DEFAULT,
    KEY_PRESS_UP,
    KEY_PRESS_DOWN,
    KEY_PRESS_LEFT,
    KEY_PRESS_RIGHT,
    // KEY_PRESS_COUNT
};

bool init()
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf(":( error = %s\n", SDL_GetError());
    }
    else
    {
        gWindow = SDL_CreateWindow("ascyWindow", 200, 100, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if (gWindow == NULL)
        {
            printf(":( error = %s\n", SDL_GetError());
            return false;
        }
        else
        {
            gScreenSurface = SDL_GetWindowSurface(gWindow);
        }
    }
    return true;
}

bool loadMedia(KEY_PRESS val)
{
    // printf("load")
    std::cout << "loadMedia: " << val << std::endl;
    // printf("val = %v", val);
    // gImage = SDL_LoadBMP("media/hello_world.bmp");
    // gImage = SDL_LoadBMP("media/city.bmp");
    switch (val)
    {
    case KEY_PRESS_UP:
        gImage = SDL_LoadBMP("media/key_presses/up.bmp");
        break;

    case KEY_PRESS_DOWN:
        gImage = SDL_LoadBMP("media/key_presses/down.bmp");
        break;

    case KEY_PRESS_LEFT:
        gImage = SDL_LoadBMP("media/key_presses/left.bmp");
        break;

    case KEY_PRESS_RIGHT:
        gImage = SDL_LoadBMP("media/key_presses/right.bmp");
        break;

    case KEY_PRESS_DEFAULT:
        gImage = SDL_LoadBMP("media/key_presses/default.bmp");
        break;
    }
    if (gImage == NULL)
    {
        printf(":( error = %s\n", SDL_GetError());
        return false;
    }
    return true;
}

void close()
{
    SDL_DestroyWindow(gWindow);
    gWindow = NULL;
    SDL_FreeSurface(gImage);
    gImage = NULL;
    SDL_FreeSurface(gScreenSurface);
    gScreenSurface = NULL;
    SDL_Quit();
}

void runGameLoop()
{
    bool quitGameLoop = false;
    SDL_Event e;
    while (!quitGameLoop)
    {
        while (SDL_PollEvent(&e) == 1)
        {
            if (e.type == SDL_QUIT)
                quitGameLoop = true;
            else if (e.type == SDL_KEYDOWN)
            {
                // std::cout << e.key.keysym.sym << std::endl;
                switch (e.key.keysym.sym)
                {
                case SDLK_UP:
                    loadMedia(KEY_PRESS_UP);
                    break;

                case SDLK_DOWN:
                    loadMedia(KEY_PRESS_DOWN);
                    break;

                case SDLK_LEFT:
                    loadMedia(KEY_PRESS_LEFT);
                    break;

                case SDLK_RIGHT:
                    loadMedia(KEY_PRESS_RIGHT);
                    break;

                default:
                    loadMedia(KEY_PRESS_DEFAULT);
                    break;
                }
            }
        }
        SDL_BlitSurface(gImage, NULL, gScreenSurface, NULL);
        SDL_UpdateWindowSurface(gWindow);
    }
}

int main()
{
    if (!init())
    {
        printf(":( failed to initialize = %s\n", SDL_GetError());
    }
    else
    {
        if (!loadMedia(KEY_PRESS_DEFAULT))
        {
            printf(":( failed to load image = %s\n", SDL_GetError());
        }
        else
        {
            runGameLoop();
        }
    }
    close();
    return 0;
}
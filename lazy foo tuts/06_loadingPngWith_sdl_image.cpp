
#include <iostream>
#include <stdio.h>
// #include "/opt/homebrew/opt/sdl2_image/include/SDL2/SDL_image.h"
#include "SDL.h"
#include "SDL_image.h"
// #include "/opt/homebrew/opt/sdl2/include/SDL2/SDL.h"
// #include "SDL.h"

// #define sdl_ SDL_

SDL_Window *gWindow = NULL;
SDL_Surface *gScreenSurface = NULL;
SDL_Surface *gImage = NULL;
SDL_Surface *optimizedSurface = NULL; // the gImage could be of any format like 24-bit, 1-bit, etc. we need to convert it to the format of the window's screen :)
SDL_Rect *dest_rect = new SDL_Rect;

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

void printFormat(SDL_Surface *surface, std::string surfName)
{
    std::cout << "format of " << surfName << std::endl;
    std::cout << surface->format->format << std::endl;
    std::cout << surface->format->BitsPerPixel << std::endl;
    std::cout << surface->format->BytesPerPixel << std::endl;
}

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
            if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG))
            {
                printf(":( error = %s\n", IMG_GetError());
                return false;
            }
            else
                gScreenSurface = SDL_GetWindowSurface(gWindow);

            // printFormat(gScreenSurface, "gScreenSurface");
        }
    }
    return true;
}

bool loadMedia(KEY_PRESS val)
{
    switch (val)
    {
    case KEY_PRESS_UP:
        gImage = IMG_Load("media/key_presses/up.bmp");
        break;

    case KEY_PRESS_DOWN:
        gImage = IMG_Load("media/key_presses/down.bmp");
        break;

    case KEY_PRESS_LEFT:
        gImage = IMG_Load("media/key_presses/left.bmp");
        break;

    case KEY_PRESS_RIGHT:
        gImage = IMG_Load("media/key_presses/right.bmp");
        break;

    case KEY_PRESS_DEFAULT:
        gImage = IMG_Load("media/cartoonZombieBackground01.webp");
        break;
    }
    if (gImage == NULL)
    {
        printf(":( error = %s\n", SDL_GetError());
        return false;
    }
    // printFormat(gImage, "gImage");
    optimizedSurface = SDL_ConvertSurface(gImage, gScreenSurface->format, 0);
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
    SDL_FreeSurface(optimizedSurface);
    optimizedSurface = NULL;

    delete (dest_rect);

    IMG_Quit();
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
            {
                quitGameLoop = true;
            }
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

        dest_rect->x = 100;
        dest_rect->y = 50;
        dest_rect->h = SCREEN_HEIGHT - 100;
        dest_rect->w = SCREEN_WIDTH - 200;

        SDL_BlitScaled(optimizedSurface, NULL, gScreenSurface, dest_rect);
        // SDL_BlitSurface(gImage, NULL, gScreenSurface, NULL);
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
            printf(":) started gameLoop \n");
            runGameLoop();
        }
    }
    close();
    return 0;
}
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

bool loadMedia()
{
    // gImage = SDL_LoadBMP("media/hello_world.bmp");
    // gImage = SDL_LoadBMP("media/city.bmp");
    gImage = SDL_LoadBMP("media/background01.bmp");
    if (gImage == NULL)
    {
        printf(":( error = %s\n", SDL_GetError());
        return false;
    }
    return true;
}

bool close()
{
    SDL_DestroyWindow(gWindow);
    gWindow = NULL;
    SDL_FreeSurface(gImage);
    gImage = NULL;
    SDL_FreeSurface(gScreenSurface);
    gScreenSurface = NULL;
    SDL_Quit();
}

int main()
{
    if (!init())
    {
        printf(":( failed to initialize = %s\n", SDL_GetError());
    }
    else
    {
        if (!loadMedia())
        {
            printf(":( failed to load image = %s\n", SDL_GetError());
        }
        else
        {
            SDL_BlitSurface(gImage, NULL, gScreenSurface, NULL);
            SDL_UpdateWindowSurface(gWindow);

            SDL_Event e;
            bool quitGameLoop = false;
            while (!quitGameLoop)
            {
                while (SDL_PollEvent(&e))
                {
                    if (e.type == SDL_QUIT)
                        quitGameLoop = true;
                }
            }
        }
    }
    close();
    return 0;
}
// #include <SDL.h>
#include "/opt/homebrew/opt/sdl2/include/SDL2/SDL.h"
// #include <SDL.h>
#include <stdio.h>

const int SCREEN_HEIGHT = 480;
const int SCREEN_WIDTH = 640;

int main(int argc, char *args[])
{
    SDL_Window *window = NULL;
    SDL_Surface *screenSurface = NULL;

    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf(":( error initializing sdl. error = %s\n", SDL_GetError());
    }
    else
    {
        window = SDL_CreateWindow("ascyWindow", 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if (window == NULL)
        {
            printf(":( error creating the window. error = %s\n", SDL_GetError());
        }
        else
        {
            screenSurface = SDL_GetWindowSurface(window);
            SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, 0x00, 0xf0, 0xf0));
            SDL_UpdateWindowSurface(window);

            SDL_Event e;
            bool quitWindow = false;
            while (!quitWindow)
            {
                while (SDL_PollEvent(&e))
                {
                    if (e.type == SDL_QUIT)
                        quitWindow = true;
                }
            }
        }
    }

    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
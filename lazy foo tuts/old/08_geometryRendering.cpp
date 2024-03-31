
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
// SDL_Rect *dest_rect = new SDL_Rect;
SDL_Texture *gTexture = NULL;
SDL_Renderer *gRenderer = NULL;

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
            if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG))
            {
                printf(":( error = %s\n", IMG_GetError());
                return false;
            }
            else
            {
                // gScreenSurface = SDL_GetWindowSurface(gWindow);
                gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
                if (gRenderer == NULL)
                {
                    printf(":( error = %s\n", SDL_GetError());
                    return false;
                }
                else
                {
                    SDL_SetRenderDrawColor(gRenderer, 0xff, 0xf0, 0x34, 0xff);
                }
            }

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

    gTexture = SDL_CreateTextureFromSurface(gRenderer, gImage);

    SDL_FreeSurface(gImage);
    gImage = NULL;

    if (gTexture == NULL)
    {
        printf(":( error = %s\n", SDL_GetError());
        return false;
    }

    // SDL_RenderCopy(gRenderer, gTexture, NULL, dest_rect);

    return true;
}

void close()
{
    SDL_DestroyWindow(gWindow);
    gWindow = NULL;

    SDL_FreeSurface(gScreenSurface);
    gScreenSurface = NULL;
    SDL_FreeSurface(optimizedSurface);
    optimizedSurface = NULL;

    // delete (dest_rect);

    SDL_DestroyTexture(gTexture);
    gTexture = NULL;
    SDL_DestroyRenderer(gRenderer);
    gRenderer = NULL;

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

                    // case SDLK_r:
                    //     renderFilledRect(dest_rect);
                    //     break;

                default:
                    loadMedia(KEY_PRESS_DEFAULT);
                    break;
                }
            }
        }

        // CLEAR rendering target with the set draw color
        SDL_SetRenderDrawColor(gRenderer, 0x00, 0xff, 0xff, 0xff);
        SDL_RenderClear(gRenderer);

        // RENDER filled rect
        SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0x11);
        SDL_Rect fillRect = {100, 50, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2};
        SDL_RenderFillRect(gRenderer, &fillRect);

        // RENDER empty rect
        SDL_Rect emptyRect = {fillRect.x + 100, fillRect.y + 100, fillRect.w, fillRect.h};
        SDL_SetRenderDrawColor(gRenderer, 0xff, 0x00, 0x00, 0x11);
        SDL_RenderDrawRect(gRenderer, &emptyRect);

        // RENDER line
        SDL_SetRenderDrawColor(gRenderer, 0xff, 0x00, 0x00, 0x11);
        SDL_RenderDrawLine(gRenderer, 50, 400, SCREEN_WIDTH - 50, SCREEN_HEIGHT - 50);

        // RENDER points
        SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0x11);
        for (int i = 0; i < SCREEN_HEIGHT; i++)
        {
            SDL_RenderDrawPoint(gRenderer, 400, i);
        }

        // update screen
        SDL_RenderPresent(gRenderer);
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
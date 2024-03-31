

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
SDL_Surface *surfFadeIn = NULL;
SDL_Surface *surfFadeOut = NULL;
SDL_Surface *optimizedSurface = NULL; // the gImage could be of any format like 24-bit, 1-bit, etc. we need to convert it to the format of the window's screen :)
// SDL_Rect *dest_rect = new SDL_Rect;
SDL_Texture *textureFadeIn = NULL, *textureFadeOut = NULL;
SDL_Renderer *gRenderer = NULL;

const int SCREEN_WIDTH = 640;
// const int SCREEN_WIDTH = 1280;
// const int SCREEN_HEIGHT = 720;
const int SCREEN_HEIGHT = 480;

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
                    SDL_SetRenderDrawColor(gRenderer, 0xff, 0xff, 0xff, 0xff);
                }
            }

            // printFormat(gScreenSurface, "gScreenSurface");
        }
    }
    return true;
}

bool loadMedia(KEY_PRESS val)
{
    surfFadeIn = IMG_Load("media/fadein.png");
    surfFadeOut = IMG_Load("media/fadeout.png");
    if (surfFadeIn == NULL || surfFadeOut == NULL)
    {
        printf(":( error = %s\n", SDL_GetError());
        return false;
    }

    // color keying to remove colored background from sprites
    // SDL_SetColorKey(surfFadeIn, SDL_TRUE, SDL_MapRGB(surfFadeIn->format, 0x00, 0xff, 0xff));

    textureFadeIn = SDL_CreateTextureFromSurface(gRenderer, surfFadeIn);
    textureFadeOut = SDL_CreateTextureFromSurface(gRenderer, surfFadeOut);

    SDL_FreeSurface(surfFadeIn);
    surfFadeIn = NULL;
    SDL_FreeSurface(surfFadeOut);
    surfFadeOut = NULL;

    if (textureFadeIn == NULL || textureFadeOut == NULL)
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

    SDL_FreeSurface(gScreenSurface);
    gScreenSurface = NULL;
    SDL_FreeSurface(optimizedSurface);
    optimizedSurface = NULL;

    // delete (dest_rect);

    SDL_DestroyTexture(textureFadeIn);
    textureFadeIn = NULL;
    SDL_DestroyRenderer(gRenderer);
    gRenderer = NULL;

    IMG_Quit();
    SDL_Quit();
}

void runGameLoop()
{
    bool quitGameLoop = false;
    SDL_Event e;

    uint8_t r = 255, g = 255, b = 255;

    while (!quitGameLoop)
    {
        while (SDL_PollEvent(&e) == 1)
        {
            if (e.type == SDL_QUIT)
            {
                quitGameLoop = true;
            }
            else
            {
                int32_t pressedKey = e.key.keysym.sym;
                if (pressedKey == SDLK_q)
                {
                    if (r + 25 <= 255)
                        r += 25;
                }
                else if (pressedKey == SDLK_w)
                {
                    if (g + 25 <= 255)
                        g += 25;
                }
                else if (pressedKey == SDLK_e)
                {
                    if (b + 25 <= 255)
                        b += 25;
                }
                else if (pressedKey == SDLK_a)
                {
                    if (r - 25 >= 0)
                        r -= 25;
                }
                else if (pressedKey == SDLK_s)
                {
                    if (g - 25 >= 0)
                        g -= 25;
                }
                else if (pressedKey == SDLK_d)
                {
                    if (b - 25 >= 0)
                        b -= 25;
                }
            }
        }
        // clear screen
        SDL_RenderClear(gRenderer);

        SDL_SetTextureBlendMode(textureFadeOut, SDL_BLENDMODE_BLEND);
        // SDL_SetTextureBlendMode(textureFadeIn, SDL_BLENDMODE_BLEND);

        SDL_SetTextureAlphaMod(textureFadeOut, g);
        // SDL_SetTextureAlphaMod(textureFadeIn, 255 - g);


        // SDL_SetTextureColorMod(textureFadeIn, r, g, b);

        SDL_RenderCopy(gRenderer, textureFadeIn, NULL, NULL);
        SDL_RenderCopy(gRenderer, textureFadeOut, NULL, NULL);

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


#include <iostream>
#include <stdio.h>
// #include "/opt/homebrew/opt/sdl2_image/include/SDL2/SDL_image.h"
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
// #include "/opt/homebrew/opt/sdl2/include/SDL2/SDL.h"
// #include "SDL.h"

// #define sdl_ SDL_

SDL_Window *gWindow = NULL;
SDL_Surface *gScreenSurface = NULL;
SDL_Texture *textureText = NULL;
SDL_Renderer *gRenderer = NULL;
int frameNo = 0;

SDL_Rect *frameRects[4];

// const int SCREEN_WIDTH = 640;
const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;
// const int SCREEN_HEIGHT = 480;

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
                printf(":( failed to initialize SDL_IMG. error = %s\n", IMG_GetError());
                return false;
            }
            else if (TTF_Init() == -1)
            {
                printf(":( failed to initialize SDL_TTF. error = %s\n", TTF_GetError());
                return false;
            }
            else
            {
                // gScreenSurface = SDL_GetWindowSurface(gWindow);
                gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
                if (gRenderer == NULL)
                {
                    printf(":( failed to create a renderer. error = %s\n", SDL_GetError());
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

bool loadMedia()
{

    std::string text = "Ascyrax is gonna develop a cool rts game";
    SDL_Surface *surfText = NULL;
    TTF_Font *textFont = NULL;
    textFont = TTF_OpenFont("media/lazy.ttf", 28);
    if (textFont == NULL)
    {
        printf(":( could not load font. error = %s\n", TTF_GetError());
        return false;
    }
    SDL_Color textColor = {255, 255, 255, 255};

    surfText = TTF_RenderText_Solid(textFont, text.c_str(), textColor);
    if (surfText == NULL)
    {
        printf(":( error = %s\n", SDL_GetError());
        return false;
    }

    // color keying to remove colored background from sprites
    // SDL_SetColorKey(surfText, SDL_TRUE, SDL_MapRGB(surfText->format, 0x00, 0xff, 0xff));

    textureText = SDL_CreateTextureFromSurface(gRenderer, surfText);

    SDL_FreeSurface(surfText);
    surfText = NULL;

    if (textureText == NULL)
    {
        printf(":( failed to create a texture from the surface. error = %s\n", SDL_GetError());
        return false;
    }

    return true;
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
        }
        // clear screen
        SDL_SetRenderDrawColor(gRenderer, 0x0f, 0x1f, 0xaf, 1);
        SDL_RenderClear(gRenderer);

        int textureW, textureH;
        SDL_QueryTexture(textureText, NULL, NULL, &textureW, &textureH);
        SDL_Rect dest_rect = {0, 0, textureW, textureH};
        SDL_RenderCopy(gRenderer, textureText, NULL, &dest_rect);

        // update screen
        SDL_RenderPresent(gRenderer);
    }
}

void close()
{
    SDL_DestroyWindow(gWindow);
    gWindow = NULL;

    SDL_FreeSurface(gScreenSurface);
    gScreenSurface = NULL;

    // delete (dest_rect);

    SDL_DestroyTexture(textureText);
    textureText = NULL;
    SDL_DestroyRenderer(gRenderer);
    gRenderer = NULL;

    for (int i = 0; i < 4; i++)
    {
        delete frameRects[i];
        frameRects[i] = NULL;
    }

    IMG_Quit();
    SDL_Quit();
}

int main()
{
    if (!init())
    {
        printf(":( failed to initialize SDL. error = %s\n", SDL_GetError());
    }
    else
    {
        if (!loadMedia())
        {
            printf(":( failed to load image. error = %s\n", SDL_GetError());
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

// texture ? surface ? what are fonts binaryily?? how do they work??


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
SDL_Texture *textureButton = NULL;
SDL_Renderer *gRenderer = NULL;
int frameNo = 0;

SDL_Rect srcRects[4];
SDL_Rect destRects[4];
int mouseEvents[4] = {0, 0, 0, 0};

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

    SDL_Surface *surfButton = IMG_Load("media/button.png");
    if (surfButton == NULL)
    {
        printf(":( error = %s\n", SDL_GetError());
        return false;
    }

    // color keying to remove colored background from sprites
    // SDL_SetColorKey(surfButton, SDL_TRUE, SDL_MapRGB(surfButton->format, 0x00, 0xff, 0xff));

    textureButton = SDL_CreateTextureFromSurface(gRenderer, surfButton);

    SDL_FreeSurface(surfButton);
    surfButton = NULL;

    if (textureButton == NULL)
    {
        printf(":( failed to create a texture from the surface. error = %s\n", SDL_GetError());
        return false;
    }

    int textureW, textureH;
    SDL_QueryTexture(textureButton, NULL, NULL, &textureW, &textureH);
    for (int i = 0; i < 4; i++)
    {
        srcRects[i] = {0, 0, textureW, textureH / 4};
        destRects[i] = {((i % 2 == 0) ? 0 : SCREEN_WIDTH - textureW), ((i <= 1) ? 0 : (SCREEN_HEIGHT - textureH / 4)), textureW, textureH / 4};
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
            else
            {
                int ex, ey;
                SDL_GetMouseState(&ex, &ey);
                for (int i = 0; i < 4; i++)
                {
                    int dx = destRects[i].x;
                    int dy = destRects[i].y;
                    int dw = destRects[i].w;
                    int dh = destRects[i].h;
                    if (ex >= dx && ex <= (dx + dw) && ey >= dy && ey <= (dy + dh))
                    {
                        srcRects[i] = {0, 200, dw, dh};
                        if (e.type == SDL_MOUSEBUTTONDOWN)
                        {
                            srcRects[i] = {0, 400, dw, dh};
                        }
                        else if (e.type == SDL_MOUSEBUTTONUP)
                        {
                            srcRects[i] = {0, 600, dw, dh};
                        }
                    }
                    else
                    {
                        srcRects[i] = {0, 0, dw, dh};
                    }
                }
            }
        }
        // clear screen
        SDL_SetRenderDrawColor(gRenderer, 0x0, 0x0, 0x0, 1);
        SDL_RenderClear(gRenderer);

        int textureW, textureH;
        SDL_QueryTexture(textureButton, NULL, NULL, &textureW, &textureH);

        for (int i = 0; i < 4; i++)
        {
            SDL_RenderCopy(gRenderer, textureButton, &srcRects[i], &destRects[i]);
        }

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

    SDL_DestroyTexture(textureButton);
    textureButton = NULL;
    SDL_DestroyRenderer(gRenderer);
    gRenderer = NULL;

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


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
SDL_Surface *gImgDots = NULL;
SDL_Surface *gImgPlayer = NULL;
SDL_Surface *optimizedSurface = NULL; // the gImage could be of any format like 24-bit, 1-bit, etc. we need to convert it to the format of the window's screen :)
// SDL_Rect *dest_rect = new SDL_Rect;
SDL_Texture *gTextureDots = NULL, *gTexturePlayer = NULL;
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
    gImgDots = IMG_Load("media/dots.png");
    gImgPlayer = IMG_Load("media/foo.png");
    if (gImgDots == NULL || gImgPlayer == NULL)
    {
        printf(":( error = %s\n", SDL_GetError());
        return false;
    }

    SDL_SetColorKey(gImgDots, SDL_TRUE, SDL_MapRGB(gImgDots->format, 0x00, 0xff, 0xff));

    gTextureDots = SDL_CreateTextureFromSurface(gRenderer, gImgDots);
    gTexturePlayer = SDL_CreateTextureFromSurface(gRenderer, gImgPlayer);

    SDL_FreeSurface(gImgDots);
    gImgDots = NULL;
    SDL_FreeSurface(gImgPlayer);
    gImgPlayer = NULL;

    if (gTextureDots == NULL || gTexturePlayer == NULL)
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

    SDL_DestroyTexture(gTextureDots);
    gTextureDots = NULL;
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
        }
        // clear screen
        SDL_RenderClear(gRenderer);

        int imgDotsWidth, imgDotsHeight;
        SDL_QueryTexture(gTextureDots, NULL, NULL, &imgDotsWidth, &imgDotsHeight);

        SDL_Rect topLeft = {0, 0, imgDotsWidth / 2, imgDotsHeight / 2};
        SDL_Rect dest_rect_topLeft = {0, 0, imgDotsWidth / 2, imgDotsHeight / 2};
        SDL_RenderCopy(gRenderer, gTextureDots, &topLeft, &dest_rect_topLeft);

        SDL_Rect topRight = {imgDotsWidth / 2, 0, imgDotsWidth / 2, imgDotsHeight / 2};
        SDL_Rect dest_rect_topRight = {SCREEN_WIDTH - imgDotsWidth / 2, 0, imgDotsWidth / 2, imgDotsHeight / 2};
        SDL_RenderCopy(gRenderer, gTextureDots, &topRight, &dest_rect_topRight);

        SDL_Rect bottomLeft = {0, imgDotsHeight / 2, imgDotsWidth / 2, imgDotsHeight / 2};
        SDL_Rect dest_rect_bottomLeft = {0, SCREEN_HEIGHT - imgDotsHeight / 2, imgDotsWidth / 2, imgDotsHeight / 2};
        SDL_RenderCopy(gRenderer, gTextureDots, &bottomLeft, &dest_rect_bottomLeft);

        SDL_Rect bottomRight = {imgDotsWidth / 2, imgDotsHeight / 2, imgDotsWidth / 2, imgDotsHeight / 2};
        SDL_Rect dest_rect_bottomRight = {SCREEN_WIDTH - imgDotsWidth / 2, SCREEN_HEIGHT - imgDotsHeight / 2, imgDotsWidth / 2, imgDotsHeight / 2};
        SDL_RenderCopy(gRenderer, gTextureDots, &bottomRight, &dest_rect_bottomRight);

        int gImgPlayer_width, gImgPlayer_height;
        SDL_QueryTexture(gTexturePlayer, NULL, NULL, &gImgPlayer_width, &gImgPlayer_height);

        SDL_Rect rect_player = {350, 160, gImgPlayer_width, gImgPlayer_height};
        SDL_RenderCopy(gRenderer, gTexturePlayer, NULL, &rect_player);

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
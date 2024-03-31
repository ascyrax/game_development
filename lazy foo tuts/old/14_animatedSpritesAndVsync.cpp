

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
SDL_Surface *surfFrames = NULL;
SDL_Surface *optimizedSurface = NULL; // the gImage could be of any format like 24-bit, 1-bit, etc. we need to convert it to the format of the window's screen :)
// SDL_Rect *dest_rect = new SDL_Rect;
SDL_Texture *textureFrames = NULL;
SDL_Renderer *gRenderer = NULL;
int frameNo = 0;

SDL_Rect *frameRects[4];

const int SCREEN_WIDTH = 640;
// const int SCREEN_WIDTH = 1280;
// const int SCREEN_HEIGHT = 720;
const int SCREEN_HEIGHT = 480;

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

bool loadMedia()
{

    surfFrames = IMG_Load("media/foo.png");
    if (surfFrames == NULL)
    {
        printf(":( error = %s\n", SDL_GetError());
        return false;
    }

    // color keying to remove colored background from sprites
    // SDL_SetColorKey(surfFrames, SDL_TRUE, SDL_MapRGB(surfFrames->format, 0x00, 0xff, 0xff));

    textureFrames = SDL_CreateTextureFromSurface(gRenderer, surfFrames);

    SDL_FreeSurface(surfFrames);
    surfFrames = NULL;

    if (textureFrames == NULL)
    {
        printf(":( error = %s\n", SDL_GetError());
        return false;
    }

    return true;
}

bool generateFrameRects()
{
    int textureW, textureH;
    if (SDL_QueryTexture(textureFrames, NULL, NULL, &textureW, &textureH) < 0)
    {
        printf(":( error getting the texture width or height. error = %s\n", SDL_GetError());
        return false;
    }

    int frameW = textureW / 4;
    int frameH = textureH;

    for (int i = 0; i < 4; i++)
    {
        int x = i * frameW;
        int y = 0;
        int w = frameW;
        int h = frameH;

        frameRects[i] = new SDL_Rect{x, y, w, h};
    }

    // printf("%d %d\n", textureW, textureH);
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
        SDL_RenderClear(gRenderer);

        // SDL_RenderCopy(gRenderer, textureFrames, NULL, NULL);
        int frameSpeed = 150;
        // SDL_RenderCopy(gRenderer, textureFrames, frameRects[frameNo / frameSpeed], NULL);
        SDL_RenderCopyEx(gRenderer, textureFrames, frameRects[frameNo / frameSpeed], NULL, 0, NULL, SDL_FLIP_VERTICAL);
        frameNo++;
        if (frameNo / frameSpeed == 4)
            frameNo = 0;
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
    SDL_FreeSurface(optimizedSurface);
    optimizedSurface = NULL;

    // delete (dest_rect);

    SDL_DestroyTexture(textureFrames);
    textureFrames = NULL;
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
            printf(":) generating SDL_rects to render the textureFrame's different parts generating an animation: \n");
            generateFrameRects();
            printf(":) started gameLoop \n");
            runGameLoop();
        }
    }
    close();
    return 0;
}
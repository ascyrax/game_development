
#include <iostream>
#include <stdio.h>
// #include "/opt/homebrew/opt/sdl2_image/include/SDL2/SDL_image.h"
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "SDL_mixer.h"
// #include "/opt/homebrew/opt/sdl2/include/SDL2/SDL.h"
// #include "SDL.h"

// #define sdl_ SDL_

SDL_Window *gWindow = NULL;
SDL_Surface *gScreenSurface = NULL;
SDL_Texture *textureFont = NULL;
SDL_Renderer *gRenderer = NULL;
int frameNo = 0;

TTF_Font *font = NULL;

SDL_Rect srcRects[4];
SDL_Rect destRects[4];
int mouseEvents[4] = {0, 0, 0, 0};

const int LEVEL_WIDTH = 1280;
const int LEVEL_HEIGHT = 960;
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

Mix_Music *music = NULL;
Mix_Chunk *scratch = NULL;
Mix_Chunk *beat = NULL;
Mix_Chunk *high = NULL;
Mix_Chunk *low = NULL;
Mix_Chunk *medium = NULL;

uint64_t startTime = SDL_GetTicks64();
int nFrames = 0;

SDL_Surface *surfDot = NULL;
SDL_Surface *surfBg = NULL;
SDL_Texture *textBg = NULL;
SDL_Texture *textDot = NULL;

bool init()
{
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
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
            else if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
            {
                printf(":( failed to initialize SDL_MIXER. error = %s\n", Mix_GetError());
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

int dotX = SCREEN_WIDTH / 2, dotY = SCREEN_HEIGHT / 2, dotW, dotH;

void updateTexture()
{
    textBg = SDL_CreateTextureFromSurface(gRenderer, surfBg);
    textDot = SDL_CreateTextureFromSurface(gRenderer, surfDot);

    // TTF_CloseFont(text)

    SDL_QueryTexture(textDot, NULL, NULL, &dotW, &dotH);
    int MUL = 2;
    dotW *= MUL;
    dotH *= MUL;

    // free surface
    SDL_FreeSurface(surfBg);
    surfBg = NULL;
    SDL_FreeSurface(surfDot);
    surfDot = NULL;
}

bool loadMedia()
{
    surfDot = IMG_Load("media/dot.bmp");
    if (surfDot == NULL)
    {
        printf(":( could not load the dot.bmp file. error = %s\n", IMG_GetError());
        return false;
    }

    SDL_SetColorKey(surfDot, SDL_TRUE, SDL_MapRGB(surfDot->format, 0x00, 0xff, 0xff));

    surfBg = IMG_Load("media/background01.webp");
    if (surfBg == NULL)
    {
        printf(":( could not load the background image file. error = %s\n", IMG_GetError());
        return false;
    }

    // color keying to remove colored background from sprites
    // SDL_SetColorKey(surfPrompt, SDL_TRUE, SDL_MapRGB(surfPrompt->format, 0x00, 0xff, 0xff));
    updateTexture();

    return true;
}

void runGameLoop()
{
    bool quitGameLoop = false;
    SDL_Event e;

    uint8_t r = 255, g = 255, b = 255;

    while (!quitGameLoop)
    {
        nFrames++;
        while (SDL_PollEvent(&e) == 1)
        {
            if (e.type == SDL_QUIT)
            {
                quitGameLoop = true;
            }
            else if (e.type == SDL_KEYDOWN)
            {
                int MUL = 30;
                switch (e.key.keysym.sym)
                {
                case SDLK_UP:
                    dotY -= MUL;
                    if (dotY < 0)
                        dotY = 0;
                    break;
                case SDLK_DOWN:
                    dotY += MUL;
                    if (dotY + dotH > SCREEN_HEIGHT)
                        dotY = SCREEN_HEIGHT - dotH;
                    break;
                case SDLK_LEFT:
                    dotX -= MUL;
                    if (dotX < 0)
                        dotX = 0;
                    break;
                case SDLK_RIGHT:
                    dotX += MUL;
                    if (dotX + dotW > SCREEN_WIDTH)
                        dotX = SCREEN_WIDTH - dotW;
                    break;
                default:
                    break;
                }
            }
        }

        SDL_SetRenderDrawColor(gRenderer, 0x0f, 0xff, 0xff, 0xff);
        SDL_RenderClear(gRenderer);

        int textureWid, textureHei;
        SDL_QueryTexture(textBg, NULL, NULL, &textureWid, &textureHei);

        SDL_Rect srcRect = {dotX - SCREEN_WIDTH / 2, dotY - SCREEN_HEIGHT / 2, SCREEN_WIDTH, SCREEN_HEIGHT};
        if (srcRect.x < 0)
            srcRect.x = 0;
        if (srcRect.x + SCREEN_WIDTH > LEVEL_WIDTH)
            srcRect.x = LEVEL_WIDTH - SCREEN_WIDTH;

        if (srcRect.y < 0)
            srcRect.y = 0;
        if (srcRect.y + SCREEN_HEIGHT > LEVEL_HEIGHT)
            srcRect.y = LEVEL_HEIGHT - SCREEN_HEIGHT;

        SDL_RenderCopy(gRenderer, textBg, &srcRect, NULL);

        SDL_Rect destRect = {dotX, dotY, dotW, dotH};
        SDL_RenderCopy(gRenderer, textDot, NULL, &destRect);

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

    SDL_DestroyTexture(textureFont);
    textureFont = NULL;
    SDL_DestroyRenderer(gRenderer);
    gRenderer = NULL;

    TTF_CloseFont(font);
    font = NULL;

    IMG_Quit();
    SDL_Quit();
    TTF_Quit();
    Mix_Quit();
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

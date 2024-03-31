

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
SDL_Texture *texturePrompt = NULL;
SDL_Renderer *gRenderer = NULL;
int frameNo = 0;

SDL_Rect srcRects[4];
SDL_Rect destRects[4];
int mouseEvents[4] = {0, 0, 0, 0};

// const int SCREEN_WIDTH = 640;
const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;
// const int SCREEN_HEIGHT = 480;

Mix_Music *music = NULL;
Mix_Chunk *scratch = NULL;
Mix_Chunk *beat = NULL;
Mix_Chunk *high = NULL;
Mix_Chunk *low = NULL;
Mix_Chunk *medium = NULL;

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

bool loadMedia()
{

    SDL_Surface *surfPrompt = IMG_Load("media/prompt.png");
    music = Mix_LoadMUS("media/audio/beat.wav");
    scratch = Mix_LoadWAV("media/audio/scratch.wav");
    beat = Mix_LoadWAV("media/audio/beat.wav");
    high = Mix_LoadWAV("media/audio/high.wav");
    medium = Mix_LoadWAV("media/audio/medium.wav");
    low = Mix_LoadWAV("media/audio/low.wav");

    if (surfPrompt == NULL)
    {
        printf(":( error = %s\n", SDL_GetError());
        return false;
    }

    // color keying to remove colored background from sprites
    // SDL_SetColorKey(surfPrompt, SDL_TRUE, SDL_MapRGB(surfPrompt->format, 0x00, 0xff, 0xff));

    texturePrompt = SDL_CreateTextureFromSurface(gRenderer, surfPrompt);

    SDL_FreeSurface(surfPrompt);
    surfPrompt = NULL;

    if (texturePrompt == NULL)
    {
        printf(":( failed to create a texture from the surface. error = %s\n", SDL_GetError());
        return false;
    }

    int textureW, textureH;
    SDL_QueryTexture(texturePrompt, NULL, NULL, &textureW, &textureH);
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
                if (e.key.keysym.sym == SDLK_1)
                {
                    Mix_PlayChannel(-1, high, 0);
                }
                else if (e.key.keysym.sym == SDLK_2)
                {
                    Mix_PlayChannel(-1, medium, 0);
                }
                else if (e.key.keysym.sym == SDLK_3)
                {
                    Mix_PlayChannel(-1, low, 0);
                }
                else if (e.key.keysym.sym == SDLK_4)
                {
                    Mix_PlayChannel(-1, scratch, 0);
                }
                else if (e.key.keysym.sym == SDLK_9)
                {
                    if (Mix_PlayingMusic() == 0)
                    {
                        // not playing
                        Mix_PlayMusic(music, -1);
                    }
                    else
                    {
                        if (Mix_PausedMusic() == 1)
                        {
                            Mix_ResumeMusic();
                        }
                        else
                            Mix_PauseMusic();
                    }
                }
                else if (e.key.keysym.sym == SDLK_0)
                {
                    Mix_HaltMusic();
                }
            }
        }

        SDL_SetRenderDrawColor(gRenderer, 0x0f, 0x0f, 0x0f, 0xff);
        SDL_RenderClear(gRenderer);

        SDL_Rect destRect = {100, 100, 800, 400};

        SDL_RenderCopy(gRenderer, texturePrompt, NULL, &destRect);

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

    SDL_DestroyTexture(texturePrompt);
    texturePrompt = NULL;
    SDL_DestroyRenderer(gRenderer);
    gRenderer = NULL;

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

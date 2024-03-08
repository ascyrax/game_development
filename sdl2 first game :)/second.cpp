// parallelrealities.co.uk
#include <SDL.h>
#include <SDL_image.h>

#define SCREEN_WIDTH 1080
#define SCREEN_HEIGHT 720

typedef struct
{
    SDL_Window *window;
    SDL_Renderer *renderer;
} App;

typedef struct
{
    int x;
    int y;
    SDL_Texture *texture;
} Entity;

Entity player;
App app;

void cleanup(void)
{
    if (app.renderer)
    {
        SDL_DestroyRenderer(app.renderer);
    }
    if (app.window)
    {
        SDL_DestroyWindow(app.window);
    }
    SDL_Quit();
}

void initSDL(void)
{
    IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG);

    int renderFlags, windowFlags;
    windowFlags = 0;
    renderFlags = SDL_RENDERER_ACCELERATED;

    if (SDL_InitSubSystem(SDL_INIT_VIDEO) < 0)
    {
        printf("error: couldn't initialize SDL: %s\n", SDL_GetError());
        exit(1);
    }

    app.window = SDL_CreateWindow("Shooter 02", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, windowFlags);

    if (!app.window)
    {
        printf("error: couldn't open %d x %d window: %s\n", SCREEN_WIDTH, SCREEN_HEIGHT, SDL_GetError());
        exit(1);
    }

    SDL_SetHint("linear", SDL_HINT_RENDER_SCALE_QUALITY);

    app.renderer = SDL_CreateRenderer(app.window, -1, renderFlags);

    if (!app.renderer)
    {
        printf("error: failed to create renderer: %s\n", SDL_GetError());
        exit(1);
    }
}

void doInput(void)
{
    SDL_Event event;

    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_QUIT:
            exit(0);
            break;

        default:
            break;
        }
    }
}

void prepareScene(void)
{
    SDL_SetRenderDrawColor(app.renderer, 96, 128, 255, 255);
    SDL_RenderClear(app.renderer);
}

void presentScene(void)
{
    SDL_RenderPresent(app.renderer);
}

SDL_Texture *loadTexture(char *filename)
{
    SDL_Texture *texture;

    SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "Loading %s", filename);

    texture = IMG_LoadTexture(app.renderer, filename);

    return texture;
}

void blit(SDL_Texture *texture, int x, int y)
{
    SDL_Rect dest;

    dest.x = x;
    dest.y = y;
    SDL_QueryTexture(texture, NULL, NULL, &dest.w, &dest.h);

    SDL_RenderCopy(app.renderer, texture, NULL, &dest);
}

int main(int argc, char *argv[])
{

    memset(&app, 0, sizeof(App));
    memset(&player, 0, sizeof(Entity));

    initSDL();

    player.x = 100;
    player.y = 100;
    // char * filename = "gfx/player_sc2_protoss.png"
    player.texture = loadTexture("gfx/player_sc2_protoss.png");

    atexit(cleanup);

    while (1)
    {
        prepareScene();

        doInput();

        blit(player.texture, player.x, player.y);

        presentScene();

        SDL_Delay(16);
    }

    return 0;
}
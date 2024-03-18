#include <SDL.h>
#include <iostream>

int main(int argc, char **args[])
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cout << "SDL could not be initialized! SDL error = " << SDL_GetError() << std::endl;
        return 1;
    }
    return 0;
}
// chatgpt4

#include <SDL.h>
#include <iostream>

int main(int argc, char* argv[]) {
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return -1;
    }

    // Create a window
    SDL_Window* window = SDL_CreateWindow(
        "SDL2 Basic Window",         // window title
        SDL_WINDOWPOS_UNDEFINED,     // initial x position
        SDL_WINDOWPOS_UNDEFINED,     // initial y position
        800,                         // width, in pixels
        600,                         // height, in pixels
        SDL_WINDOW_SHOWN             // flags - window is visible
    );

    if (window == NULL) {
        std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return -1;
    }

    // Main loop flag
    bool quit = false;

    // Event handler
    SDL_Event e;

    // While application is running
    while (!quit) {
        // Handle events on queue
        while (SDL_PollEvent(&e) != 0) {
            // User requests quit
            if (e.type == SDL_QUIT) {
                quit = true;
            }
        }
    }

    // Destroy window
    SDL_DestroyWindow(window);

    // Quit SDL subsystems
    SDL_Quit();

    return 0;
}

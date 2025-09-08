#include "graphics.h"
#include <SDL3/SDL.h>

Graphics::Graphics() {
    // Constructor implementation
    SDL_CreateWindowAndRenderer("Game", 640, 480, 0, &window, &renderer);
}

Graphics::~Graphics() {
    // Destructor implementation
    if (renderer) {
        SDL_DestroyRenderer(renderer);
    }
    if (window) {
        SDL_DestroyWindow(window);
    }
}

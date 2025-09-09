#include "graphics.h"
#include "globals.h"
#include <string> 
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <iostream>

Graphics::Graphics() {
    // Constructor implementation
    SDL_CreateWindowAndRenderer("Game", globals::SCREEN_WIDTH, globals::SCREEN_HEIGHT, 0, &window, &renderer);
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

SDL_Surface* Graphics::loadImage(const string &filePath) {
    if (_spriteSheets.count(filePath) == 0) {
        _spriteSheets[filePath] = IMG_Load(filePath.c_str());
    }
    return _spriteSheets[filePath];
}

void Graphics::blitSurface(SDL_Texture* source, SDL_FRect* sourceRectangle, SDL_FRect* destinationRectangle) {
    SDL_RenderTexture(renderer, source, sourceRectangle, destinationRectangle);
}

void Graphics::flip() {
    SDL_RenderPresent(renderer);
}

void Graphics::clear() {
    SDL_RenderClear(renderer);
}

SDL_Renderer* Graphics::getRenderer() const {
    return renderer;
}
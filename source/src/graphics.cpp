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
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
}

SDL_Texture* Graphics::loadImage(const string &filePath) {
    // Return cached surface if it exists
    if (this->_surfaces.count(filePath) == 0) {
		this->_surfaces[filePath] = IMG_LoadTexture(renderer, filePath.c_str());
	}
	return _surfaces[filePath];
}

void Graphics::blitSurface(SDL_Texture* source, SDL_FRect* sourceRectangle, SDL_FRect* destinationRectangle) {
    // Make sure this uses SDL_RenderTexture, not SDL_BlitSurface
    if (SDL_RenderTexture(renderer, source, sourceRectangle, destinationRectangle) != 0) {
        //cout << "Failed to render texture: " << SDL_GetError() << endl;
    }
    SDL_RenderTexture(renderer, source, sourceRectangle, destinationRectangle);
}

void Graphics::blitSurfaceFlipped(SDL_Texture* source, SDL_FRect* sourceRectangle, SDL_FRect* destinationRectangle, bool flipHorizontal) {
    if (flipHorizontal) {
        SDL_RenderTextureRotated(renderer, source, sourceRectangle, destinationRectangle, 0.0, nullptr, SDL_FLIP_HORIZONTAL);
    } else {
        SDL_RenderTexture(renderer, source, sourceRectangle, destinationRectangle);
    }
}

void Graphics::clear() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Black background
    SDL_RenderClear(renderer);
}

void Graphics::flip() {
    SDL_RenderPresent(renderer);
}

SDL_Renderer* Graphics::getRenderer() const {
    return renderer;
}

SDL_Surface* Graphics::loadSurface(const string &filePath) {
    // Load surface without caching - for one-time use like backgrounds
    SDL_Surface* surface = IMG_Load(filePath.c_str());
    if (!surface) {
        cout << "Failed to load surface: " << filePath << endl;
        cout << "IMG_Error: " << SDL_GetError() << endl;
    } else {
        cout << "Successfully loaded surface: " << filePath << " (" << surface->w << "x" << surface->h << ")" << endl;
    }
    return surface;
}

SDL_Texture* Graphics::loadTexture(const string &filePath) {
    // Load and cache textures for sprites that will be reused
    if (_spriteSheets.count(filePath) == 0) {
        SDL_Surface* surface = IMG_Load(filePath.c_str());
        if (surface) {
            _spriteSheets[filePath] = SDL_CreateTextureFromSurface(renderer, surface);
            SDL_DestroySurface(surface); // Clean up surface immediately
            
            if (_spriteSheets[filePath]) {
                cout << "Successfully loaded and cached texture: " << filePath << endl;
            } else {
                cout << "Failed to create texture from surface: " << SDL_GetError() << endl;
                _spriteSheets[filePath] = nullptr;
            }
        } else {
            cout << "Failed to load image: " << filePath << endl;
            cout << "IMG_Error: " << SDL_GetError() << endl;
            _spriteSheets[filePath] = nullptr;
        }
    }
    return _spriteSheets[filePath];
}
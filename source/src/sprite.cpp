#include "sprite.h"
#include "graphics.h"
#include "globals.h"
#include <SDL3/SDL.h>
#include <iostream>

using namespace std;

Sprite::Sprite() {

}

Sprite::Sprite(Graphics& graphics, const string& filePath, int sourceX, int sourceY, int width, int height, float posX, float posY) {
    _x = posX;
    _y = posY;

    _sourceRect.x = sourceX;
    _sourceRect.y = sourceY;
    _sourceRect.w = width;
    _sourceRect.h = height;

    SDL_Surface* surface = graphics.loadImage(filePath);
    if (surface == nullptr) {
        cerr << "Failed to load image: " << filePath << endl;
        cout << "Failed to load image: " << filePath << endl;
        _spriteSheet = nullptr;
    } else {
        _spriteSheet = SDL_CreateTextureFromSurface(graphics.getRenderer(), surface);
        if (_spriteSheet == nullptr) {
            cerr << "Failed to create texture from surface: " << SDL_GetError() << endl;
            cout << "Failed to create texture from surface: " << SDL_GetError() << endl;
        }
    }
}

Sprite::~Sprite() {
    
}

void Sprite::draw(Graphics &graphics, int x, int y) {
    SDL_FRect destinationRect;
    destinationRect.x = x;
    destinationRect.y = y;
    destinationRect.w = _sourceRect.w * globals::SPRITE_SCALE;
    destinationRect.h = _sourceRect.h * globals::SPRITE_SCALE;

    graphics.blitSurface(_spriteSheet, &_sourceRect, &destinationRect);
}

void Sprite::update() {

}


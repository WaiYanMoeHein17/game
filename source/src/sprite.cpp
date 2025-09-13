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

    SDL_Surface* surface = graphics.loadSurface(filePath);
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

    _boundingBox = Rectangle(_x, _y, width * globals::SPRITE_SCALE, height * globals::SPRITE_SCALE);

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
    // Update bounding box position
    _boundingBox = Rectangle(_x, _y, _sourceRect.w * globals::SPRITE_SCALE, _sourceRect.h * globals::SPRITE_SCALE);
}

const Rectangle Sprite::getBoundingBox() const {
    return _boundingBox;
}

const sides::Side Sprite::getCollisionSide(const Rectangle &other) const {
    int amtRight, amtLeft, amtTop, amtBottom;
    amtRight = getBoundingBox().getRight() - other.getLeft();
    amtLeft = other.getRight() - getBoundingBox().getLeft();
    amtTop = other.getBottom() - getBoundingBox().getTop();
    amtBottom = getBoundingBox().getBottom() - other.getTop();
    int minAmt = min(min(amtRight, amtLeft), min(amtTop, amtBottom));

    if (minAmt == amtRight) {
        return sides::RIGHT;
    } else if (minAmt == amtLeft) {
        return sides::LEFT;
    } else if (minAmt == amtTop) {
        return sides::TOP;
    } else if (minAmt == amtBottom) {
        return sides::BOTTOM;
    } else {
        return sides::NONE;
    }
}

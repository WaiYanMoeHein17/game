#include "tile.h"
#include "graphics.h"
#include "globals.h"
#include <SDL3/SDL.h>

using namespace std;

Tile::Tile() {

}

Tile::Tile(SDL_Texture* tileset, Vector2D size, Vector2D tilesetPosition, Vector2D position) :
    _tileset(tileset),
    _size(size),
    _tilesetPosition(tilesetPosition),
    _position(Vector2D(position.x * globals::SPRITE_SCALE, position.y * globals::SPRITE_SCALE))
    {
    // Constructor implementation


}

void Tile::update(int elapsedTime) {

}

void Tile::draw(Graphics &graphics) {
    SDL_FRect destRect = { _position.x, _position.y, _size.x * globals::SPRITE_SCALE, _size.y * globals::SPRITE_SCALE }; 
    SDL_FRect sourceRect = { _tilesetPosition.x, _tilesetPosition.y, _size.x, _size.y };
    graphics.blitSurface(_tileset, &sourceRect, &destRect);
}

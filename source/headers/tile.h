#ifndef TILE_H
#define TILE_H

#include "globals.h"

class Graphics;
struct SDL_Texture; 

class Tile {
    public:
        Tile();
        Tile(SDL_Texture* texture, Vector2D size, Vector2D tilesetPosition, Vector2D position);
        void update(int elapsedTime);
        void draw(Graphics &graphics);
    protected:
    private:
        SDL_Texture* _tileset;
        Vector2D _size; 
        Vector2D _tilesetPosition;
        Vector2D _position;
};


#endif //TILE_H
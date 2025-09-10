#ifndef LEVEL_H
#define LEVEL_H

#include <string>
#include <vector>
#include "tile.h"
#include "globals.h"

class Graphics;
struct SDL_Texture;
struct SDL_FRect; 
struct Tileset;

using namespace std;

class Level {
    public:
        Level();
        Level(string mapName, Vector2D spawnPoint, Graphics &graphics);
        ~Level();
        void update(int elapsedTime);
        void draw1(Graphics &graphics);
        void draw2(Graphics &graphics);
    protected:
    private:
        string _mapName; 
        Vector2D _spawnPoint;
        Vector2D _size;
        Vector2D _tileSize; 
        SDL_Texture* _backgroundTexture;

        vector<Tile> _tileList;
        vector<Tileset> _tilesets;

        
        /*
        loads the map from a file
        */
        void loadMap1(string mapName, Graphics &graphics);
        void loadMap2(string mapName, Graphics &graphics);
        void loadMap3(string mapName, Graphics &graphics);
        void loadExternalTileset(const string &source, int firstGid, Graphics &graphics);
        void parseCSVTileData(const string &data, int width, int height, int tileWidth, int tileHeight);

}; 

struct Tileset {
    SDL_Texture* texture;
    int FirstGid; 
    Tileset() {
        FirstGid = -1;
    }
    Tileset(SDL_Texture* tex, int firstGid) {
        texture = tex;
        FirstGid = firstGid;
    }
};




#endif // LEVEL_H
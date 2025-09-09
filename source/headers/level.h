#ifndef LEVEL_H
#define LEVEL_H

#include <string>
#include "globals.h"

class Graphics;
struct SDL_Texture;

using namespace std;

class Level {
    public:
        Level();
        Level(string mapName, Vector2D spawnPoint, Graphics &graphics);
        ~Level();
        void update(int elapsedTime);
        void draw(Graphics &graphics);
    protected:
    private:
        string _mapName; 
        Vector2D _spawnPoint;
        Vector2D _size;
        SDL_Texture* _backgroundTexture;
        
        /*
        loads the map from a file
        */
        void loadMap(string mapName, Graphics &graphics);

}; 




#endif // LEVEL_H
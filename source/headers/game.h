#ifndef GAME_H
#define GAME_H
#include <SDL3/SDL.h>
#include <string>
#include "sprite.h"
#include "animated_sprite.h"
#include "player.h"

using namespace std;

class Graphics;

class Game {
    public:
        Game();
        ~Game();
    private:
        void gameLoop(); 
        void draw(Graphics &graphics);
        void update(float elapsedTime);

        Player _player;
}; 
#endif // GAME_H
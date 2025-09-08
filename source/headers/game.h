#ifndef GAME_H
#define GAME_H
#include <SDL3/SDL.h>

class Graphics;

class Game {
    public:
        Game();
        ~Game();
    private:
        void gameLoop(); 
        void draw(Graphics &graphics);
        void update(float elapsedTime);
}; 
#endif // GAME_H
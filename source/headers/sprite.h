#ifndef SPRITE_H
#define SPRITE_H

#include <SDL3/SDL.h>
#include <string>

using namespace std;

class Graphics; 

class Sprite {
    public:
        Sprite();
        Sprite(Graphics& graphics, const string& filePath, int sourceX, int sourceY, int width, int height, float posX, float posY);
        virtual ~Sprite();
        virtual void update();
        void draw(Graphics& graphics, int x, int y);
    protected:
        SDL_FRect _sourceRect; 
        SDL_Texture* _spriteSheet;
    private:
        float _x, _y;
};

#endif // SPRITE_H
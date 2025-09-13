#ifndef SPRITE_H
#define SPRITE_H

#include "rectangle.h"
#include "globals.h"
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
        const Rectangle getBoundingBox() const;
        const sides::Side getCollisionSide(const Rectangle &other) const;
    protected:
        SDL_FRect _sourceRect; 
        SDL_Texture* _spriteSheet;
        Rectangle _boundingBox;

        float _x, _y;
    private:

};

#endif // SPRITE_H
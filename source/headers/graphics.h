#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <string>
#include <map>


using namespace std; 

struct SDL_Window;
struct SDL_Renderer;

class Graphics {
    public:
        Graphics();
        ~Graphics();

        /*
        Loads an image into the _spriteSheets map and returns a pointer to the SDL_Surface. As a result of this, the image can be accessed later without needing to load it again.
        */
        SDL_Surface* loadImage(const string &filePath);
        
        /*
        Draws a portion of the source surface to the destination rectangle on the window.
        */
        void blitSurface(SDL_Texture* source, SDL_FRect* sourceRectangle, SDL_FRect* destinationRectangle);

        void flip(); // Update the screen with any rendering performed since the previous call

        void clear(); // Clear the screen

        SDL_Renderer* getRenderer() const; // Getter for the renderer

    private:
        SDL_Window* window;
        SDL_Renderer* renderer;

        map<string, SDL_Surface*> _spriteSheets;
};

#endif // GRAPHICS_H
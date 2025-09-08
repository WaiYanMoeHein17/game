#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <SDL3/SDL.h>

using namespace std; 

struct SDL_Window;
struct SDL_Renderer;

class Graphics {
    public:
        Graphics();
        ~Graphics();

    private:
        SDL_Window* window;
        SDL_Renderer* renderer;
};

#endif // GRAPHICS_H
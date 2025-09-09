#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <map>
#include <string>

using namespace std; 

struct SDL_Window;
struct SDL_Renderer;

class Graphics {
public:
    Graphics();
    ~Graphics();
    void clear();
    void flip();
    void blitSurface(SDL_Texture* source, SDL_FRect* sourceRectangle, SDL_FRect* destinationRectangle);
    void blitSurfaceFlipped(SDL_Texture* source, SDL_FRect* sourceRectangle, SDL_FRect* destinationRectangle, bool flipHorizontal = false);
    
    // For loading raw surfaces (backgrounds, one-time use)
    SDL_Surface* loadSurface(const string &filePath);
    
    // For loading and caching textures (sprites, reused multiple times)
    SDL_Texture* loadTexture(const string &filePath);

    SDL_Surface* loadImage(const string &filePath);

    SDL_Renderer* getRenderer() const;

private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    map<string, SDL_Texture*> _spriteSheets; // Cached textures for sprites
    map<string, SDL_Surface*> _surfaces; // Cached surfaces for one-time use
};

#endif // GRAPHICS_H
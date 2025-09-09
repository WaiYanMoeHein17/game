#include <iostream>
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <string>
#include "level.h"
#include "graphics.h"
#include "globals.h"

using namespace std;

Level::Level() {

}

Level::Level(string mapName, Vector2D spawnPoint, Graphics &graphics) 
    : _mapName(mapName), 
    _spawnPoint(spawnPoint), 
    _size(Vector2D(0,0)), 
    _backgroundTexture(nullptr)
    {
    loadMap(mapName, graphics);
}

Level::~Level() {
    if (_backgroundTexture) {
        SDL_DestroyTexture(_backgroundTexture);
    }
}

void Level::loadMap(string mapName, Graphics &graphics) {
    cout << "Attempting to load background..." << endl;
    
    SDL_Surface* surface = graphics.loadImage("../content/background2/PNG/mapTile_001.png");
    if (surface) {
        cout << "Surface loaded successfully: " << surface->w << "x" << surface->h << endl;
        
        int surfaceWidth = surface->w;
        int surfaceHeight = surface->h;

        _backgroundTexture = SDL_CreateTextureFromSurface(graphics.getRenderer(), surface);
        SDL_DestroySurface(surface); 
        
        if (_backgroundTexture) {
            cout << "Texture created successfully!" << endl;
            _size = Vector2D(static_cast<float>(surfaceWidth), static_cast<float>(surfaceHeight));
        } else {
            cout << "Failed to create texture: " << SDL_GetError() << endl;
        }
    } else {
        cout << "Failed to load surface!" << endl;
    }
}

/*void Level::loadMap(string mapName, Graphics &graphics) {
    cout << "Attempting to load: " << mapName << endl;
    cout << "Looking for file at: ../content/background/Samples/colored_castle.png" << endl;

    SDL_Surface* backgroundSurface = graphics.loadImage("..//content//background//Samples//colored_castle.png");
    if (backgroundSurface){
        // Get size from surface before creating texture
        int surfaceWidth = backgroundSurface->w;
        int surfaceHeight = backgroundSurface->h;
        
        _backgroundTexture = SDL_CreateTextureFromSurface(graphics.getRenderer(), backgroundSurface);
        SDL_DestroySurface(backgroundSurface);
        
        if (_backgroundTexture) {
            _size = Vector2D(static_cast<float>(surfaceWidth), static_cast<float>(surfaceHeight));
            cout << "Loaded background texture with size: " << surfaceWidth << "x" << surfaceHeight << endl;
        } else {
            cerr << "Failed to create texture from surface: " << SDL_GetError() << endl;
            _size = Vector2D(800, 600);
        }
    } else {
        cerr << "Failed to load background image: " << SDL_GetError() << endl;
    }
}*/ 

void Level::update(int elapsedTime) {
    // Update level state if needed
}

void Level::draw(Graphics &graphics) {
    if (_backgroundTexture) {
        // Simple full-screen draw first
        SDL_FRect sourceRect = {0, 0, _size.x, _size.y}; // Full source image
        SDL_FRect destRect = {0, 0, (float)globals::SCREEN_WIDTH, (float)globals::SCREEN_HEIGHT};
        graphics.blitSurface(_backgroundTexture, &sourceRect, &destRect);
        //cout << "Drawing background texture" << endl; // Remove this after testing
    } else {
        cout << "No background texture to draw!" << endl;
        // Draw red screen so you know this code path is running
        SDL_SetRenderDrawColor(graphics.getRenderer(), 255, 0, 0, 255);
        SDL_RenderClear(graphics.getRenderer());
    }
    /*
    for (int x = 0; x < _size.x; x += tileWidth) {
        for (int y = 0; y < _size.y; y += tileHeight) {
            SDL_FRect destRect = {static_cast<float>(x), static_cast<float>(y), static_cast<float>(tileWidth), static_cast<float>(tileHeight)};
            graphics.blitSurface(_backgroundTexture, &sourceRect, &destRect);
        }
    }
    */
}




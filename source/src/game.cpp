/*
Holds information for the main game loop
*/

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <algorithm>
#include <iostream>
#include "game.h"
#include "graphics.h"
#include "input.h"
#include "animated_sprite.h"
#include "player.h"

using namespace std;

namespace {
    const int FPS = 50;
    const int MAX_FRAME_TIME = 5 * 1000 / FPS;
    const int MIN_FRAME_TIME = 1000 / FPS;
}

Game::Game() {
    SDL_Init(SDL_INIT_VIDEO);
    gameLoop();
}

Game::~Game() {

}

void Game::gameLoop() {
    Graphics graphics;
    Input input;
    SDL_Event event;
    bool running = true;
    int LAST_UPDATE_TIME = SDL_GetTicks(); // Get the current time in milliseconds
    _player = Player(graphics, 100, 100);
    //_player = AnimatedSprite(graphics, "..//content//spritesheet//Roguelike//roguelikeChar_transparent.png", 0, 0, 16, 16, 100, 100, 100);
    //_player.setUpAnimations();
    //_player.playAnimation("RunRight");

    while (running) {
        input.beginNewFrame();

        if (SDL_PollEvent(&event)) {

            if (event.type == SDL_EVENT_KEY_DOWN) {

                if (event.key.repeat == 0) {
                    // Ignore key repeat events
                    input.keyDownEvent(event);
                } 
                //running = false;
            }

            else if (event.type == SDL_EVENT_KEY_UP) {
                input.keyUpEvent(event);
            }

            else if (event.type == SDL_EVENT_QUIT) {
                running = false;
            }
        }

        if (input.wasKeyPressed(SDL_SCANCODE_ESCAPE)) {
            running = false;
        }

        // Handle movement - use separate if statements instead of else if
        bool isMoving = false;
        
        if (input.isKeyHeld(SDL_SCANCODE_A)) {
            _player.moveLeft();
            isMoving = true;
        }
        if (input.isKeyHeld(SDL_SCANCODE_D)) {
            _player.moveRight();
            isMoving = true;
        }
        if (input.isKeyHeld(SDL_SCANCODE_W)) {
            _player.moveUp();
            isMoving = true;
        }
        if (input.isKeyHeld(SDL_SCANCODE_S)) {
            _player.moveDown();
            isMoving = true;
        }
        
        // Handle sprinting
        if (input.isKeyHeld(SDL_SCANCODE_LSHIFT)) {
            _player.setSprinting(true);
        } else {
            _player.setSprinting(false);
        }
        
        // Stop movement if no movement keys are pressed
        if (!isMoving) {
            _player.stop();
        }

        // Calculate elapsed time
        const int CURRENT_TIME_MS = SDL_GetTicks();
        int ELAPSED_TIME_MS = CURRENT_TIME_MS - LAST_UPDATE_TIME;
        update(min(ELAPSED_TIME_MS, MAX_FRAME_TIME));
        LAST_UPDATE_TIME = CURRENT_TIME_MS;
        
        draw(graphics);
    }
}

void Game::draw(Graphics &graphics) {
    graphics.clear(); 
    _player.draw(graphics);
    graphics.flip();
}

void Game::update(float elapsedTime) {
    _player.update(elapsedTime);
}
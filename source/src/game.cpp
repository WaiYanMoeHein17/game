/*
Holds information for the main game loop
*/

#include <SDL3/SDL.h>
#include <algorithm>
#include <iostream>
#include "game.h"
#include "graphics.h"
#include "input.h"

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

    while (running) {
        input.beginNewFrame();

        if (SDL_PollEvent(&event)) {

            if (event.type == SDL_EVENT_KEY_DOWN) {

                if (event.key.repeat == 0) {
                    // Ignore key repeat events
                    input.keyDownEvent(event);
                } 
                running = false;
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

        const int CURRENT_TIME_MS = SDL_GetTicks();
        int ELAPSED_TIME_MS = CURRENT_TIME_MS - LAST_UPDATE_TIME;
        update(min(ELAPSED_TIME_MS, MAX_FRAME_TIME)); // Convert ms to seconds
        LAST_UPDATE_TIME = CURRENT_TIME_MS;
    }
}

void Game::draw(Graphics &graphics) {

}

void Game::update(float elapsedTime) {

}
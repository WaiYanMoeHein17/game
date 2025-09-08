/*

*/
#include "input.h"
#include <iostream>

using namespace std;

void Input::beginNewFrame() {
    /*
    Function to be called at the beginning of each new frame to reset keys that are no longer relevant
    */
    _pressedKeys.clear();
    _releasedKeys.clear();
}

void Input::keyDownEvent(const SDL_Event &event) {
    /*
    Function to be called when a key is pressed
    */
    _pressedKeys[event.key.scancode] = true;
    _heldKeys[event.key.scancode] = true;
}

void Input::keyUpEvent(const SDL_Event &event) {
    /*
    Function to be called when a key is released
    */
    _releasedKeys[event.key.scancode] = true;
    _heldKeys[event.key.scancode] = false;
}

bool Input::wasKeyPressed(SDL_Scancode key) {
    /*
    Function to check if a key was pressed in the current frame
    */
    return _pressedKeys[key];
}

bool Input::wasKeyReleased(SDL_Scancode key) {
    /*
    Function to check if a key was released in the current frame
    */
    return _releasedKeys[key];
}

bool Input::isKeyHeld(SDL_Scancode key) {
    /*
    Function to check if a key is currently being held down
    */
    return _heldKeys[key];
}

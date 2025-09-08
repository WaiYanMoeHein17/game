#ifndef INPUT_H
#define INPUT_H
#include <SDL3/SDL.h>
#include <map>

using namespace std;

class Input {
    public:
        void beginNewFrame();
        void keyDownEvent(const SDL_Event &event);
        void keyUpEvent(const SDL_Event &event);
        bool wasKeyPressed(SDL_Scancode key);
        bool wasKeyReleased(SDL_Scancode key);
        bool isKeyHeld(SDL_Scancode key);
    private:
        // SDL
        map<SDL_Scancode, bool> _heldKeys;
        map<SDL_Scancode, bool> _pressedKeys;
        map<SDL_Scancode, bool> _releasedKeys;

};


#endif // INPUT_H
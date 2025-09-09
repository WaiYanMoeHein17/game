#ifndef GLOBALS_H
#define GLOBALS_H

namespace globals {
    const int SCREEN_WIDTH = 1820;
    const int SCREEN_HEIGHT = 980;
    const float SPRITE_SCALE = 1.0f;
}

enum Direction {
    NONE,
    LEFT,
    RIGHT,
    UP,
    DOWN
}; 

struct Vector2D {
    float x;
    float y;
    Vector2D(float x = 0, float y = 0) : x(x), y(y) {}
    Vector2D zero() {
        return Vector2D(0, 0);
    }
};

#endif // GLOBALS_H
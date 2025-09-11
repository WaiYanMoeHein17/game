#ifndef RECTANGLE_H
#define RECTANGLE_H

#include "globals.h"

using namespace std;

class Rectangle {

    public: 
        Rectangle() {}

        Rectangle(float x, float y, float width, float height) 
            : _x(x), _y(y), _width(width), _height(height) {}

        const float getCenterX() const {
            return _x + _width / 2.0f;
        }
        const float getCenterY() const {
            return _y + _height / 2.0f;
        }
        const float getLeft() const {
            return _x;
        }
        const float getRight() const {
            return _x + _width;
        }
        const float getTop() const {
            return _y;
        }
        const float getBottom() const {
            return _y + _height;
        }
        const int getWidth() const {
            return _width;
        }
        const int getHeight() const {
            return _height;
        }
        const int getSide(sides::Side side) const {
            switch (side) {
                case sides::TOP: return getTop();
                case sides::BOTTOM: return getBottom();
                case sides::LEFT: return getLeft();
                case sides::RIGHT: return getRight();
                default: return 0;
            }
        }
        bool collidesWith(const Rectangle &other) const {
            return !(getRight() <= other.getLeft() || 
                    getLeft() >= other.getRight() || 
                    getBottom() <= other.getTop() || 
                    getTop() >= other.getBottom());
        }
        const bool isValidRectangle() const {
            return _width > 0 && _height > 0 && _x >= 0 && _y >= 0;
        }

    private:
        float _x, _y, _width, _height;
    }; 
#endif // RECTANGLE_H
#ifndef SLOPE_H
#define SLOPE_H

#include "globals.h"
#include "rectangle.h"
#include <cmath> // for abs()

class Slope {
    public:
        Slope(); 
        Slope(Vector2D p1, Vector2D p2) {
            _p1 = p1; 
            _p2 = p2; 
            if (_p2.x != _p1.x) {
                _slope = (abs(_p2.y) - abs(_p1.y)) / (abs(_p2.x) - abs(_p1.x)); 
            } else {
                _slope = 0; // Vertical line, slope is undefined
            }
        }

        const inline float getSlope() const {
            return _slope; 
        }

        const bool collidesWith(const Rectangle &other) const {
            // Simple bounding box check first
            float minX = std::min(_p1.x, _p2.x);
            float maxX = std::max(_p1.x, _p2.x);
            float minY = std::min(_p1.y, _p2.y);
            float maxY = std::max(_p1.y, _p2.y);

            // there is 16 conditions to check collision with slope

            if (other.getRight() < minX || other.getLeft() > maxX ||
                other.getBottom() < minY || other.getTop() > maxY) {
                return false; // No collision
            }

            // More precise slope collision detection can be added here if needed

            return true; // For now, just return true if bounding boxes overlap
        }

    protected:

    private:
        Vector2D _p1, _p2; 
        float _slope; 
};

#endif // SLOPE_H
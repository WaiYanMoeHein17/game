#ifndef PLAYER_H
#define PLAYER_H

#include "animated_sprite.h"
#include "graphics.h"
#include "globals.h"

class Player : public AnimatedSprite {
public:
    Player();
    Player(Graphics &graphics, float x, float y);
    void setUpAnimations();
    void animationDone(string currentAnimation);
    void update(float elapsedTime);
    void draw(Graphics &graphics);
    
    // Movement methods
    void moveLeft();
    void moveRight();
    void moveUp();
    void moveDown();
    void stopMovingX();
    void stopMovingY();
    void stop();
    
    // Sprint functionality
    void setSprinting(bool sprinting);
    bool isSprinting() const;
    
    Direction getFacing() const;

private:
    float _dx, _dy; // Velocity
    Direction _facing;
    bool _isSprinting;
    
    float getCurrentSpeed() const;
};

#endif
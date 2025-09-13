#ifndef PLAYER_H
#define PLAYER_H

#include "animated_sprite.h"
#include "graphics.h"
#include "globals.h"
#include "level.h"

class Player : public AnimatedSprite {
public:
    Player();
    Player(Graphics &graphics, float x, float y);
    void setUpAnimations();
    void animationDone(string currentAnimation);
    void update(float elapsedTime);
    void update(float elapsedTime, Level& level);
    void draw(Graphics &graphics);
    const float getX() const; 
    const float getY() const;
    void handleTileCollisions(const vector<Rectangle> &others);

    // Movement methods
    void moveLeft();
    void moveRight();
    void moveUp();
    void moveDown();
    void jump();
    void stopMovingX();
    void stopMovingY();
    void stop();
    
    // Sprint functionality
    void setSprinting(bool sprinting);
    bool isSprinting() const;
    
    bool isGrounded() const; // New method to check if player is on the ground
    
    Direction getFacing() const;

private:
    float _dx, _dy; // Velocity
    Direction _facing;
    bool _isSprinting;
    bool _grounded; // true if on ground, false if in air
    
    float getCurrentSpeed() const;
};

#endif
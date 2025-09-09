#include "player.h"
#include "graphics.h"
#include <cmath>
#include <iostream>

using namespace std;

namespace player_constants {
    const float WALK_SPEED = 0.2f; // Speed at which the player moves
    const float SPRINT_SPEED = 0.4f; // Speed at which the player sprints
}

Player::Player() {

}

Player::Player(Graphics &graphics, float x, float y) 
    : AnimatedSprite(graphics, "..//content//spritesheet//Male adventurer//Tilesheet//character_maleAdventurer_sheet.png", 0, 0, 98, 126, x, y, 100) {
    graphics.loadImage("..//content//spritesheet//Male adventurer//Tilesheet//character_maleAdventurer_sheet.png");
        
    // Initialize movement variables
    _dx = 0;
    _dy = 0;
    _facing = RIGHT;
    _isSprinting = false;

    setUpAnimations();
    playAnimation("IdleRight");
}

void Player::setUpAnimations() {
    // I need to properly set up the animations here
    addAnimation(8, 0, 0, "RunRight", 98, 126, Vector2D(0, 0));
    addAnimation(8, 0, 126, "RunLeft", 98, 126, Vector2D(0, 0));
    addAnimation(1, 0, 252, "IdleRight", 98, 126, Vector2D(0, 0));
    addAnimation(1, 0, 378, "IdleLeft", 98, 126, Vector2D(0, 0));
    addAnimation(1, 0, 504, "JumpRight", 98, 126, Vector2D(0, 0));
    addAnimation(1, 0, 630, "JumpLeft", 98, 126, Vector2D(0, 0));
    addAnimation(3, 0, 0, "RunUp", 98, 126, Vector2D(0, 0));
    addAnimation(3, 0, 126, "RunDown", 98, 126, Vector2D(0, 0));
}

void Player::animationDone(string currentAnimation) {
    if (currentAnimation == "RunRight" || currentAnimation == "RunLeft") {
        playAnimation("IdleRight");
    }
}

void Player::update(float elapsedTime) {
    AnimatedSprite::update(elapsedTime);
    
    // Normalize diagonal movement so it's not faster than cardinal movement
    float magnitude = sqrt(_dx * _dx + _dy * _dy);
    if (magnitude > 0) {
        float normalizedDx = _dx / magnitude;
        float normalizedDy = _dy / magnitude;
        
        // Calculate new position
        float newX = _x + normalizedDx * getCurrentSpeed() * elapsedTime;
        float newY = _y + normalizedDy * getCurrentSpeed() * elapsedTime;
        
        int spriteWidth = 98;
        int spriteHeight = 126;
        
        // Separate boundary checking for X and Y axes
        // Only update X if it's within bounds
        if (newX >= 0 && newX + spriteWidth <= globals::SCREEN_WIDTH) {
            _x = newX;
        } else {
            // If hitting boundary, clamp to boundary
            if (newX < 0) {
                _x = 0;
            } else if (newX + spriteWidth > globals::SCREEN_WIDTH) {
                _x = globals::SCREEN_WIDTH - spriteWidth;
            }
        }
        
        // Only update Y if it's within bounds
        if (newY >= 0 && newY + spriteHeight <= globals::SCREEN_HEIGHT) {
            _y = newY;
        } else {
            // If hitting boundary, clamp to boundary
            if (newY < 0) {
                _y = 0;
            } else if (newY + spriteHeight > globals::SCREEN_HEIGHT) {
                _y = globals::SCREEN_HEIGHT - spriteHeight;
            }
        }
    }
    
    // Reset velocity for next frame
    _dx = 0;
    _dy = 0;
}

void Player::draw(Graphics &graphics) {
    AnimatedSprite::draw(graphics, _x, _y);
}

void Player::moveLeft() {
    cout << "Moving left..." << endl;
    _dx -= 1.0f;  // Use -1 to 1 range, normalize in update
    _facing = LEFT;
    cout << "About to play RunLeft animation..." << endl;
    playAnimation("RunLeft");
    cout << "RunLeft animation started." << endl;
}

void Player::moveRight() {
    _dx += 1.0f;
    _facing = RIGHT;
    playAnimation("RunRight");
}

void Player::moveUp() {
    _dy -= 1.0f;
    // Keep current facing for animation
    if (_facing == RIGHT) {
        playAnimation("RunRight");
    } else {
        playAnimation("RunLeft");
    }
}

void Player::moveDown() {
    _dy += 1.0f;
    // Keep current facing for animation
    if (_facing == RIGHT) {
        playAnimation("RunRight");
    } else {
        playAnimation("RunLeft");
    }
}

void Player::stopMovingX() {
    _dx = 0;
    if (_dy == 0) { // Only go to idle if not moving vertically either
        if (_facing == RIGHT) {
            playAnimation("IdleRight");
        } else if (_facing == LEFT) {
            playAnimation("IdleLeft");
        }
    }
}

void Player::stopMovingY() {
    _dy = 0;
    if (_dx == 0) { // Only go to idle if not moving horizontally either
        if (_facing == RIGHT) {
            playAnimation("IdleRight");
        } else if (_facing == LEFT) {
            playAnimation("IdleLeft");
        }
    }
}

void Player::stop() {
    _dx = 0;
    _dy = 0;
    if (_facing == RIGHT) {
        playAnimation("IdleRight");
    } else if (_facing == LEFT) {
        playAnimation("IdleLeft");
    }
}

void Player::setSprinting(bool sprinting) {
    _isSprinting = sprinting;
}

bool Player::isSprinting() const {
    return _isSprinting;
}

float Player::getCurrentSpeed() const {
    return _isSprinting ? player_constants::SPRINT_SPEED : player_constants::WALK_SPEED;
}

Direction Player::getFacing() const {
    return _facing;
}


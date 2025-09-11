#include "player.h"
#include "graphics.h"
#include <cmath>
#include <iostream>

using namespace std;

namespace player_constants {
    const float WALK_SPEED = 0.2f; // Speed at which the player moves
    const float SPRINT_SPEED = 0.4f; // Speed at which the player sprints
    const float GRAVITY = 0.001f; // Gravity affecting the player
    const float GRAVITY_CAP = 0.5f; // Maximum downward speed
    const float AIR_FRICTION = 0.0005f; // Air resistance affecting the player
    const float GROUND_FRICTION = 0.0010f; // Friction when on the ground
    const float JUMP_FORCE = -0.5f; // Initial jump velocity
}

Player::Player() {

}

Player::Player(Graphics &graphics, float x, float y) 
    : AnimatedSprite(graphics, "..//content//spritesheet//Male adventurer//Tilesheet//character_maleAdventurer_sheet.png", 0, 0, 98, 126, x, y, 100) {
    graphics.loadTexture("..//content//spritesheet//Male adventurer//Tilesheet//character_maleAdventurer_sheet.png");
        
    // Initialize movement variables
    _dx = 0;
    _dy = 0;
    _facing = RIGHT;
    _isSprinting = false;
    _grounded = false; 

    setUpAnimations();
    playAnimation("IdleRight");
}

void Player::setUpAnimations() {
    // Walk animation: walk0-walk7 at y=512
    addAnimation(8, 0, 512, "WalkRight", 96, 128, Vector2D(0, 0));
    addAnimation(8, 0, 512, "WalkLeft", 96, 128, Vector2D(0, 0));
    
    // Run animation: run0-run2 at y=256, starting at x=576
    addAnimation(3, 576, 256, "RunRight", 96, 128, Vector2D(0, 0));
    addAnimation(3, 576, 256, "RunLeft", 96, 128, Vector2D(0, 0));
    
    // Idle animation: idle at x=0, y=0
    addAnimation(1, 0, 0, "IdleRight", 96, 128, Vector2D(0, 0));
    addAnimation(1, 0, 0, "IdleLeft", 96, 128, Vector2D(0, 0));

    addAnimation(2, 96, 0, "Jump", 96, 128, Vector2D(0, 0));
}

void Player::animationDone(string currentAnimation) {
    if (currentAnimation == "RunRight" || currentAnimation == "RunLeft") {
        playAnimation("IdleRight");
    }
}

void Player::update(float elapsedTime) {
    // applying gravity
    /*if (!_grounded) {
        _dy += player_constants::GRAVITY * elapsedTime;
        if (_dy > player_constants::GRAVITY_CAP) {
            _dy = player_constants::GRAVITY_CAP; // Cap downward speed
        }
        
        // Apply air friction
        if (_dx > 0) {
            _dx -= player_constants::AIR_FRICTION * elapsedTime;
            if (_dx < 0) _dx = 0;
        } else if (_dx < 0) {
            _dx += player_constants::AIR_FRICTION * elapsedTime;
            if (_dx > 0) _dx = 0;
        }
    } else {
        // Apply ground friction
        if (_dx > 0) {
            _dx -= player_constants::GROUND_FRICTION * elapsedTime;
            if (_dx < 0) _dx = 0;
        } else if (_dx < 0) {
            _dx += player_constants::GROUND_FRICTION * elapsedTime;
            if (_dx > 0) _dx = 0;
        }
    }*/ 

    AnimatedSprite::update(elapsedTime);
    
    // Normalize diagonal movement so it's not faster than cardinal movement
    float magnitude = sqrt(_dx * _dx + _dy * _dy);
    if (magnitude > 0) {
        float normalizedDx = _dx / magnitude;
        float normalizedDy = _dy / magnitude;
        
        // Calculate new position
        float newX = _x + normalizedDx * getCurrentSpeed() * elapsedTime;
        float newY = _y + normalizedDy * getCurrentSpeed() * elapsedTime;
        
        int spriteWidth = 96;
        int spriteHeight = 128;
        
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

const float Player::getX() const {
    return _x;
}   

const float Player::getY() const {
    return _y;
}

void Player::moveLeft() {
    _dx -= 1.0f;
    _facing = LEFT;
    setFlipped(true);
    // Choose animation based on sprint state
    if (_isSprinting) {
        playAnimation("RunLeft");
    } else {
        playAnimation("WalkLeft");
    }
}

void Player::moveRight() {
    _dx += 1.0f;
    _facing = RIGHT;
    setFlipped(false);
    // Choose animation based on sprint state
    if (_isSprinting) {
        playAnimation("RunRight");
    } else {
        playAnimation("WalkRight");
    }
}

void Player::moveUp() {
    _dy -= 1.0f;
    
    // Keep current facing for animation, but choose walk/run based on sprint
    if (_facing == RIGHT) {
        if (_isSprinting) {
            playAnimation("RunRight");
        } else {
            playAnimation("WalkRight");
        }
    } else {
        setFlipped(true);
        if (_isSprinting) {
            playAnimation("RunLeft");
        } else {
            playAnimation("WalkLeft");
        }
    }
}

void Player::moveDown() {
    _dy += 1.0f;
    
    // Keep current facing for animation, but choose walk/run based on sprint
    if (_facing == RIGHT) {
        if (_isSprinting) {
            playAnimation("RunRight");
        } else {
            playAnimation("WalkRight");
        }
    } else {
        setFlipped(true);
        if (_isSprinting) {
            playAnimation("RunLeft");
        } else {
            playAnimation("WalkLeft");
        }
    }
}

void Player::jump() {
    _dx = 0;
    _dy = 0;
    playAnimation("Jump");
}

void Player::stopMovingX() {
    _dx = 0;
    if (_dy == 0) { // Only go to idle if not moving vertically either
        if (_facing == RIGHT) {
            setFlipped(false);
            playAnimation("IdleRight");
        } else if (_facing == LEFT) {
            setFlipped(true);
            playAnimation("IdleLeft");
        }
    }
}

void Player::stopMovingY() {
    _dy = 0;
    if (_dx == 0) { // Only go to idle if not moving horizontally either
        if (_facing == RIGHT) {
            setFlipped(false);
            playAnimation("IdleRight");
        } else if (_facing == LEFT) {
            setFlipped(true);
            playAnimation("IdleLeft");
        }
    }
}

void Player::stop() {
    _dx = 0;
    _dy = 0;
    if (_facing == RIGHT) {
        setFlipped(false);
        playAnimation("IdleRight");
    } else if (_facing == LEFT) {
        setFlipped(true);
        playAnimation("IdleLeft");
    }
}

void Player::setSprinting(bool sprinting) {
    bool wasSprintingBefore = _isSprinting;
    _isSprinting = sprinting;
    
    // If sprint state changed and we're moving, update animation
    if (wasSprintingBefore != _isSprinting && (_dx != 0 || _dy != 0)) {
        if (_facing == RIGHT) {
            if (_isSprinting) {
                playAnimation("RunRight");
            } else {
                playAnimation("WalkRight");
            }
        } else {
            if (_isSprinting) {
                playAnimation("RunLeft");
            } else {
                playAnimation("WalkLeft");
            }
        }
    }
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


#include "player.h"
#include "graphics.h"
#include "level.h"
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
    const float JUMP_FORCE = 0.5f; // Initial jump velocity
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
    AnimatedSprite::update(elapsedTime);
    
    // Apply gravity
    if (!_grounded) {
        _dy += player_constants::GRAVITY * elapsedTime;
        if (_dy > player_constants::GRAVITY_CAP) {
            _dy = player_constants::GRAVITY_CAP;
        }
    }

    // Apply friction
    if (_grounded) {
        if (_dx > 0) {
            _dx -= player_constants::GROUND_FRICTION * elapsedTime;
            if (_dx < 0) _dx = 0;
        } else if (_dx < 0) {
            _dx += player_constants::GROUND_FRICTION * elapsedTime;
            if (_dx > 0) _dx = 0;
        }
    } else {
        if (_dx > 0) {
            _dx -= player_constants::AIR_FRICTION * elapsedTime;
            if (_dx < 0) _dx = 0;
        } else if (_dx < 0) {
            _dx += player_constants::AIR_FRICTION * elapsedTime;
            if (_dx > 0) _dx = 0;
        }
    }

    // Update position based on velocity
    _x += _dx * elapsedTime;
    _y += _dy * elapsedTime;

    // Update bounding box position
    _boundingBox = Rectangle(_x, _y, 96 * globals::SPRITE_SCALE, 128 * globals::SPRITE_SCALE);
}

void Player::update(float elapsedTime, Level& level) {
    AnimatedSprite::update(elapsedTime);
    
    // Apply gravity if not grounded
    if (!_grounded) {
        _dy += player_constants::GRAVITY * elapsedTime;
        if (_dy > player_constants::GRAVITY_CAP) {
            _dy = player_constants::GRAVITY_CAP;
        }
    }

    // Apply friction for horizontal movement
    if (_grounded) {
        if (_dx > 0) {
            _dx -= player_constants::GROUND_FRICTION * elapsedTime;
            if (_dx < 0) _dx = 0;
        } else if (_dx < 0) {
            _dx += player_constants::GROUND_FRICTION * elapsedTime;
            if (_dx > 0) _dx = 0;
        }
    } else {
        if (_dx > 0) {
            _dx -= player_constants::AIR_FRICTION * elapsedTime;
            if (_dx < 0) _dx = 0;
        } else if (_dx < 0) {
            _dx += player_constants::AIR_FRICTION * elapsedTime;
            if (_dx > 0) _dx = 0;
        }
    }
    
    // Store original position for collision detection
    float originalX = _x;
    float originalY = _y;
    
    // Apply horizontal movement
    if (_dx != 0) {
        float newX = _x + _dx * elapsedTime;
        
        // Check screen boundaries
        if (newX >= 0 && newX + 96 <= globals::SCREEN_WIDTH) {
            _x = newX;
            _boundingBox = Rectangle(_x, _y, 96 * globals::SPRITE_SCALE, 128 * globals::SPRITE_SCALE);
            
            vector<Rectangle> collisions = level.checkTileCollisions(_boundingBox);
            if (collisions.size() > 0) {
                _x = originalX; // Revert horizontal movement
                _dx = 0; // Stop horizontal velocity
                cout << "Horizontal collision!" << endl;
            }
        } else {
            // Clamp to screen boundaries
            if (newX < 0) _x = 0;
            else if (newX + 96 > globals::SCREEN_WIDTH) _x = globals::SCREEN_WIDTH - 96;
            _dx = 0; // Stop at boundary
        }
    }
    
    // Apply vertical movement (this is where gravity takes effect)
    if (_dy != 0) {
        float newY = _y + _dy * elapsedTime;
        
        // Check screen boundaries
        if (newY >= 0 && newY + 128 <= globals::SCREEN_HEIGHT) {
            _y = newY;
            _boundingBox = Rectangle(_x, _y, 96 * globals::SPRITE_SCALE, 128 * globals::SPRITE_SCALE);
            
            vector<Rectangle> collisions = level.checkTileCollisions(_boundingBox);
            if (collisions.size() > 0) {
                // Handle collision - determine if landing or hitting ceiling
                for (const auto& collision : collisions) {
                    if (_dy > 0) {
                        // Falling down - landed on something
                        _y = collision.getTop() - 128; // Land on top of collision object
                        _dy = 0;
                        _grounded = true;
                        cout << "Landed! Grounded = true" << endl;
                    } else if (_dy < 0) {
                        // Moving up - hit ceiling
                        _y = collision.getTop() + collision.getHeight(); // Position below collision object
                        _dy = 0; // Stop upward movement
                        cout << "Hit ceiling!" << endl;
                    }
                }
            } else {
                // No collision - check if we just left the ground
                if (_grounded && _dy > 0) {
                    _grounded = false;
                    cout << "Left ground! Grounded = false" << endl;
                }
            }
        } else {
            // Hit screen boundaries
            if (newY < 0) {
                _y = 0;
                _dy = 0;
            } else if (newY + 128 > globals::SCREEN_HEIGHT) {
                _y = globals::SCREEN_HEIGHT - 128;
                _dy = 0;
                _grounded = true;
            }
        }
    }
    
    // Final bounding box update
    _boundingBox = Rectangle(_x, _y, 96 * globals::SPRITE_SCALE, 128 * globals::SPRITE_SCALE);
    
    // DON'T reset velocity here - let physics handle it!
    // Remove these lines:
    // _dx = 0;
    // _dy = 0;
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
    _dx = -getCurrentSpeed(); // Move left at current speed
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
    _dx = getCurrentSpeed(); // Move right at current speed
    _facing = RIGHT;
    setFlipped(false);
    // Choose animation based on sprint state
    if (_isSprinting) {
        playAnimation("RunRight");
    } else {
        playAnimation("WalkRight");
    }
}

/*void Player::moveUp() {
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
}*/ 

void Player::jump() {
    if (!_grounded) {
        cout << "Can't jump, not grounded!" << endl;
        return; // Can't jump if not grounded, cannot double jump
    }
    else if (_isSprinting) {
        // if player is sprinting, carry speed throughout the whole jump even if player stops pressing movement keys
        if (_facing == RIGHT) {
            _dx = player_constants::SPRINT_SPEED;
        } else {
            _dx = -player_constants::SPRINT_SPEED;
        }
    } else {
        // if player is walking or standing still, carry speed throughout the whole jump even if player stops pressing movement keys
        if (_facing == RIGHT) {
            _dx = player_constants::WALK_SPEED;
        } else {
            _dx = -player_constants::WALK_SPEED;
        }
    }
    _dy = -player_constants::JUMP_FORCE; // Apply jump force
    _grounded = false; // Now in the air
    playAnimation("Jump");
    //}
    
}

void Player::stopMovingX() {
    _dx = 0;
    if (_dy == 0 || _grounded) { // Only go to idle if not moving vertically either
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

void Player::handleTileCollisions(const vector<Rectangle> &others) {
    _grounded = false; // Assume not grounded until a collision from below is detected

    for (const auto &other : others) {
        if (getBoundingBox().collidesWith(other)) {
            sides::Side collisionSide = getCollisionSide(other);
            switch (collisionSide) {
                case sides::TOP:
                    _dy = 0;
                    _y = other.getTop() - getBoundingBox().getHeight() - 1;
                    break;
                case sides::BOTTOM:
                    _dy = 0;
                    _y = other.getBottom() + 1; // Slight offset to prevent sticking
                    _grounded = true; // Landed on something
                    break;
                case sides::LEFT:
                    _dx = 0;
                    _x = other.getLeft() - getBoundingBox().getWidth() - 1;
                    break;
                case sides::RIGHT:
                    _dx = 0;
                    _x = other.getRight() + 1;
                    break;
                default:
                    break;
            }
        }
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

bool Player::isGrounded() const {
    return _grounded;
}

float Player::getCurrentSpeed() const {
    return _isSprinting ? player_constants::SPRINT_SPEED : player_constants::WALK_SPEED;
}

Direction Player::getFacing() const {
    return _facing;
}

/*Rectangle Player::getCollisionRect() const {
    // Make collision box smaller than sprite for better gameplay
    float margin = 10.0f;
    return Rectangle(_x + margin, _y + margin, 96 - (margin * 2), 128 - (margin * 2));
}

Rectangle Player::getPredictedCollisionRect(float deltaX, float deltaY) const {
    float margin = 10.0f;
    return Rectangle(_x + deltaX + margin, _y + deltaY + margin, 96 - (margin * 2), 128 - (margin * 2));
}

void Player::setPosition(float x, float y) {
    _x = x;
    _y = y;
}*/

